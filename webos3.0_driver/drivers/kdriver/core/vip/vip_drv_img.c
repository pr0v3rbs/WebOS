/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2014 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/**
 * @file
 *
 *  img driver implementation for vision processor device.
 *	vision processor device will teach you how to make device driver with new platform.
 *
 *  author		hyunho kim (hyunho747.kim@lge.com)
 *  version		1.0
 *  date		2014.02.06
 *  note		Additional information.
 *
 *  @addtogroup lg1210_vip
 * @{
 */

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include <linux/mm.h>
#include <linux/rmap.h>
#include <linux/poll.h>
#include <linux/version.h>
#include <linux/smp.h>
#include <linux/vmalloc.h>
#include <linux/file.h>
#include <linux/clk.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/pm_runtime.h>
#include <linux/suspend.h>
#include <asm/page.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/memory.h>

#ifdef KDRV_CONFIG_PM
#include <linux/platform_device.h>
#endif

#include "os_util.h"
#include "base_device.h"

#include "debug_util.h"

#include "vip_debug.h"
#include "vip_drv.h"
#include "vip_core.h"
#include "vip_queue.h"
#include "vip_mem.h"
#include "vip_proc.h"

#include "vip_drv_hal.h"
#include "vip_drv_img.h"

#if defined(VIP_PM_GATING)
#include "pm_kapi.h"
#endif

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

OS_SEM_T	g_vip_img_sema;		// renderer semaphore
static int	g_vip_img_sema_init = 0;

#define VIP_IMG_LOCK_INIT()		OS_InitMutex(&g_vip_img_sema, OS_SEM_ATTR_DEFAULT)
#define VIP_IMG_LOCK()			OS_LockMutex(&g_vip_img_sema)
#define VIP_IMG_UNLOCK()		OS_UnlockMutex(&g_vip_img_sema)

/*
 * Setup configuration used for vip_core_init which called in vip_img_fopen()
 *
 */
static const struct vip_init_link vip_img_init_link = {
	.buf_max		= VIP_MAX_IMG_SIZE,
	.buf_align		= VIP_BUFFER_ALIGN,
	/* */
	.max_func		= VIP_IMG_FUNC_MAX,
	/* */
	.get_hal_ops	= VIP_Get_IMG_Hal_Ops,
	.get_mem_ops	= VIP_Get_Mem_Ops,
	/* */
	.thread_handler	= (void *)VIP_Run_Thread,
	.irq_handler	= (void *)VIP_IRQ_Handler,
};

/*
 * File open handler
 *
 */
static int VIP_IMG_Open_Option(struct vip_dev *vip)
{
#if defined(USE_CTOP_CODES_FOR_H15)
#if 1
	{
		//Clock setting
		static void __iomem *vip_ctop_clk = NULL;
		unsigned int val;

		vip_ctop_clk = ioremap(0xC898C020, 0x40);
		if (vip_ctop_clk == NULL) {
			vipm_err("NULL vip_ctop_clk\n");
			return -ENOMEM;
		}
		val = vip_readl((volatile void *)(vip_ctop_clk));
		if (val & BIT(13)) {
			val &= ~BIT(13);	// 0 :468MHz, 1 : 396MHz
			vip_writel(val, (volatile void *)(vip_ctop_clk));
			vipm_noti("VIP clk changed : 396MHz -> 468MHz (0x%08x)\n",
					vip_readl((volatile void *)(vip_ctop_clk)));
		}
		iounmap(vip_ctop_clk);
	}
#endif

#if 1	//Temp for UART2
	{
		static void __iomem *uart_ctop_sel = NULL;

		uart_ctop_sel = ioremap(0xC8601084, 0x40);
		if (uart_ctop_sel == NULL) {
			vipm_err("NULL uart_ctop_sel\n");
			return -ENOMEM;
		}
		vip_writel(0x22600800, (volatile void *)(uart_ctop_sel));
	//	vip_writel(0x26200400, (volatile void *)(uart_ctop_sel));
		vipm_noti("UART sel changed : 0x%08x\n",
				vip_readl((volatile void *)(uart_ctop_sel)));
		iounmap(uart_ctop_sel);
	}
#endif

#if 1	//Temp for VIP JTAG
	{
		static void __iomem *vip_jtag_sel = NULL;

		vip_jtag_sel = ioremap(0xC8AFF09C, 0x40);
		if (vip_jtag_sel == NULL) {
			vipm_err("NULL vip_jtag_sel\n");
			return -ENOMEM;
		}
		vip_writel(0x00300000, (volatile void *)(vip_jtag_sel));
		vipm_noti("VIP JTAG sel changed : 0x%08x\n",
				vip_readl((volatile void *)(vip_jtag_sel)));
		iounmap(vip_jtag_sel);
	}
#endif
#endif

	return 0;
}

static int VIP_IMG_Open(struct inode *inode, struct file *filp)
{
	struct cdev *cdev;
	struct vip_top *vtop;
	struct vip_dev *vip;
	int ret = 0;

	if (g_vip_img_sema_init != LX_VIP_IMG_MAGIC) {
		VIP_IMG_LOCK_INIT();
		g_vip_img_sema_init = LX_VIP_IMG_MAGIC;
	}

	VIP_IMG_LOCK();

	if (inode == NULL) {
		vipm_err("NULL inode\n");
		ret = -EINVAL;
		goto out;
	}

	cdev = inode->i_cdev;
	if (cdev == NULL) {
		vipm_err("NULL cdev\n");
		ret = -EINVAL;
		goto out;
	}

	vtop = to_vip_top_cdev(cdev);
#if 0
	if (vtop == NULL) {
		vipm_err("NULL vtop\n");
		ret = -EINVAL;
		goto out;
	}
#endif

	vip = vip_top_priv(vtop);
	if (vip == NULL) {
		vipm_err("NULL vip\n");
		ret = -EINVAL;
		goto out;
	}

	if (!VIP_Is_Registered(vip)) {
		vipm_err("VIP not ready\n");
		ret = -EBUSY;
		goto out;
	}

//	vipm_debug_drv_img("vip[0x%p], inode:0x%p, filp:0x%p\n", vip, inode, filp);

	if (!vip->open_count) {
		VIP_IMG_Open_Option(vip);

	#if 1
		ret = VIP_Core_Init(vip, (struct vip_init_link *)&vip_img_init_link);
		if (ret) {
			vipm_err("VIP_Core_Init failed, %d\n", ret);
			goto out;
		}
	#endif

		VIP_Init_Mem_Test();
	}

	filp->private_data = vtop;
	vip->open_count++;

	VIP_Set_Opened(vip);
	VIP_Core_Resume(vip);

	vipm_debug_drv_img("Open VIP-IMG device file (%d:%d) cnt[%d]\n",
					imajor(inode), iminor(inode), vip->open_count);

out:
	VIP_IMG_UNLOCK();
	return ret;
}

/*
 * File close handler
 *
 */
static int VIP_IMG_Close(struct inode *inode, struct file *filp)
{
	struct vip_top *vtop;
	struct vip_dev *vip;
	int ret = 0;

	if (g_vip_img_sema_init != LX_VIP_IMG_MAGIC) {
		return -ENOENT;
	}

	VIP_IMG_LOCK();

	if (filp == NULL) {
		vipm_err("NULL filp\n");
		ret = -EINVAL;
		goto out;
	}

	vtop = (struct vip_top *)filp->private_data;
	if (vtop == NULL) {
		vipm_err("NULL vtop\n");
		ret = -EINVAL;
		goto out;
	}

	vip = vip_top_priv(vtop);
	if (vip == NULL) {
		vipm_err("NULL vip\n");
		ret = -EINVAL;
		goto out;
	}

//	vipm_debug_drv_img("vip[0x%p], inode:0x%p, filp:0x%p\n", vip, inode, filp);

	if (vip->open_count) {
		vip->open_count--;
		if (!vip->open_count) {
		//	VIP_Abort_And_Free_Queue_All(vip);
			VIP_Clr_Opened(vip);
			VIP_Core_Sleep(vip);
			VIP_Clk_Put(vip);
		}
    }

	filp->private_data = NULL;

	vipm_debug_drv_img("Close VIP-IMG device file(%d:%d) cnt[%d]\n",
					imajor(inode), iminor(inode), vip->open_count);

out:
	VIP_IMG_UNLOCK();
	return ret;
}

/*
 * Print output object information for debugging
 *
 */
#if 0
static void VIP_IMG_Prt_Obj_Info_Detect(struct vip_req *areq)
{
	struct vip_msg_detect detect;
	unsigned char *odata;
	unsigned int obj_num;
	int i;

	if (!areq->onum) {
		vipm_info("NO object\n");
		return;
	}

	obj_num = areq->onum;

	odata = (unsigned char *)areq->odata;
	for (i=0 ; i<obj_num ; i++) {
		memcpy(&detect, odata, sizeof(struct vip_msg_detect));
		vipm_debug_drv_img("Obj[%2d/%2d] : x[%4d] y[%4d] w[%4d] h[%4d] id[%2d] info[%2d]\n",
							i+1, obj_num,
							le16_to_cpu(detect.x16),
							le16_to_cpu(detect.y16),
							le16_to_cpu(detect.w16),
							le16_to_cpu(detect.h16),
							le16_to_cpu(detect.id16),
							le16_to_cpu(detect.recog_info16));
		odata += sizeof(struct vip_msg_detect);
	}
}

static void VIP_IMG_Prt_Obj_Info_Gesture(struct vip_req *areq)
{
	struct vip_msg_gesture *gesture;
	struct vip_msg_crop *crop;
	unsigned char *odata;
	int i;

	if (!areq->onum) {
		vipm_info("NO object\n");
		return;
	}

	odata = (unsigned char *)areq->odata;

	gesture = (struct vip_msg_gesture *)odata;
	vipm_debug_drv_img("gr : px[%3d] py[%3d] hx[%3d] hy[%3d] o[%d] c[%d] e[%d] a[%d]\n",
						gesture->posx, gesture->posy,
						gesture->handposx, gesture->handposy,
						gesture->osd_button_num, gesture->click,
						gesture->event_result, gesture->gesture_alert);

	odata += sizeof(struct vip_msg_gesture);

	crop = (struct vip_msg_crop *)odata;
	vipm_debug_drv_img("cr : e[%d] x[%3d] y[%3d] w[%3d] h[%3d]\n",
						crop->enable,
						crop->offsetx, crop->offsety,
						crop->width, crop->height);
}

static void VIP_IMG_Prt_Obj_Info(struct vip_req *areq)
{
	if (!vipm_debug_drv_img_enabled()) {
		return;
	}

	switch (areq->func) {
	case VIP_IMG_FUNC_DETECT :
		VIP_IMG_Prt_Obj_Info_Detect(areq);
		break;
	case VIP_IMG_FUNC_GESTURE :
		VIP_IMG_Prt_Obj_Info_Gesture(areq);
		break;
	}
}
#else
static void VIP_IMG_Prt_Obj_Info(struct vip_req *areq)
{

}
#endif

static int VIP_IMG_Copy_Msg(struct vip_req *areq)
{
	struct vip_dev *vip = areq->vip;
	struct vip_buf *buf;
	unsigned char *dst, *src;
	struct vip_out_buffer_header out_header;
	unsigned int size, vsize;
	int io;
	int from_user;
	int ret = 0;

	buf = areq->obuf;

	io = !!vip_test_bit(VIP_BUF_FLAG_IO, &buf->flags);
#if defined(_VIP_MEM_USE_COPY_USER)
	from_user = 1;
#else
	from_user = !!vip_test_bit(VIP_BUF_FLAG_NO_CONV, &buf->flags);
#endif

	vsize = (unsigned int)VIP_Mem_Get_Vsize(vip, buf);
	if (vsize < VIP_OUT_BUFFER_HEADER_SIZE) {
		vipm_err("Invalid size : %d\n", vsize);
		return -EINVAL;
	}
	vsize -= VIP_OUT_BUFFER_HEADER_SIZE;

	out_header.objs = areq->onum;
	out_header.func = areq->func;
	out_header.index = areq->index;
	out_header.magic = LX_VIP_IMG_MAGIC;

	if (from_user) {
		dst = VIP_Mem_Get_Uptr(vip, buf);
	} else {
		dst = VIP_Mem_Get_Vaddr(vip, buf);
	}
	ret = VIP_Copy_To((void *)dst, (void *)&out_header,
						sizeof(out_header),
						from_user,
						io);
	if (ret) {
		vipm_err("VIP_Copy_To error : [%d]\n", ret);
		goto out;
	}

	if (areq->onum) {
		dst += LX_VIP_OUT_BUFFER_HEADER_SIZE;
		src = areq->odata;
		size = min(areq->osize, vsize);

		ret = VIP_Copy_To((void *)dst,
						(void *)src,
						size,
						from_user,
						io);
		if (ret) {
			vipm_err("VIP_Copy_To error : [%d]\n", ret);
		}
	}

out:
	return ret;
}

static int VIP_IMG_Receive_Msg(struct vip_req *areq)
{
	struct vip_dev *vip = areq->vip;
	int func = areq->func;
	int ret;

	VIP_Set_Dsp_Lock(vip);

	ret = VIP_Get_Obj_Info(areq);
	if (ret) {
		goto out;
	}

	vipm_debug_drv_img("func[%d] : Receive Start\n", func);

	VIP_IMG_Prt_Obj_Info(areq);

	if (areq->obuf) {
		ret = VIP_IMG_Copy_Msg(areq);
	} else {
		vipm_warn("NULL areq->obuf\n");
	}

out:
	VIP_Clr_Dsp_Lock(vip);

	return ret;
}

static int VIP_IMG_Run_Post(struct vip_req *areq)
{
	int ret;

	vipm_debug_drv_img("areq[0x%p]\n", areq);

	ret = VIP_IMG_Receive_Msg(areq);
	return ret;
}

/*
 *
 */
static int VIP_IMG_Run_Func(struct vip_dev *vip,
									union vip_arg *varg,
									unsigned int func,
									unsigned int from_user)
{
	struct vip_req *areq = NULL;
	struct vip_run_ctrl *vrun_ctrl = &varg->vrun_ctrl;
	unsigned int index;
	int ret = 0;

	spin_lock(vip->lock);

	index = vrun_ctrl->index;

	if (vip_test_bit(VIP_FLAG_SLEEP, &vip->flags)) {
		vipm_err("VIP can't run : sleep state\n");
		ret = -EPERM;
		goto out;
	}
#if defined(VIP_PM_GATING)
	if (!vip_test_bit(VIP_FLAG_CLK_GET, &vip->flags)) {
		vipm_err("VIP can't run : no clk\n");
		ret = -EPERM;
		goto out;
	}
#endif

	vipm_debug_drv_img("f[%d] i[%d] : Trigger Start\n", func, index);

	areq = VIP_Get_Request(vip, func, index, &ret);
	if (areq == NULL) {
		goto out;
	}

	ret = VIP_Chk_Buf(areq->ibuf, VIP_BUF_INPUT);
	if (ret) {
		goto out;
	}

	ret = VIP_Chk_Buf(areq->obuf, VIP_BUF_OUTPUT);
	if (ret) {
		goto out;
	}

	spin_unlock(vip->lock);
	ret = VIP_Copy_Input_Buf(areq);
	spin_lock(vip->lock);
	if (ret) {
		goto out;
	}

	if (vip_test_bit(VIP_RUN_CTRL_FLAG_RESET, &vrun_ctrl->flags)) {
		VIP_Set_Areq_Flags(areq, VIP_REQ_FLAG_RESET);
	} else if (vip_test_bit(VIP_RUN_CTRL_FLAG_SCENE_CHANGE, &vrun_ctrl->flags)) {
		VIP_Set_Areq_Flags(areq, VIP_REQ_FLAG_SCENE_CHANGE);
	}

	VIP_Add_Request(areq, 0, VIP_IMG_Run_Post, from_user);

	spin_unlock(vip->lock);

	VIP_Wakeup_Thread(vip);

	vipm_debug_drv_img("f[%d] i[%d] : Trigger Done\n", func, index);

	return 0;

out:
	VIP_Run_Error(vip, func, index, ret);
	spin_unlock(vip->lock);
	return ret;
}

/*
 * ioctl handlers for vip device
 *
 * note: if you have some critial data, you should protect them using semaphore or spin lock.
 */
static int _VIP_IMG_Debug_Exception(unsigned int cmd)
{
	int ret = 0;
	switch(cmd) {
	case LX_VIP_IMG_IOWR_STATE :
		ret = 1;
		break;
	}
	return ret;
}

static int _VIP_IMC_Ioctl_Chk_Known(struct vip_ioctl_info *info,
									unsigned int cmd,
									unsigned int max)
{
	return ((_IOC_NR(cmd) < max)? (info[_IOC_NR(cmd)].ioctl == cmd) : 0);
}

static int _VIP_IMG_Ioctl_Chk_Arg(union vip_arg *varg,
											unsigned int magic,
											unsigned int cmd,
											unsigned long arg,
											unsigned int from_user)
{
	int ret;

	if (_IOC_TYPE(cmd) != magic) {
		vipm_err("Invalid magic[0x%02x] != Target Magic[0x%02x]\n",
				 	_IOC_TYPE(cmd), magic);
		return -ENOTTY;
	}
	if (from_user && !capable(CAP_SYS_ADMIN)) {
		vipm_err("capable(CAP_SYS_ADMIN) error\n");
		return -EPERM;
	}

	if (_IOC_DIR(cmd) & (_IOC_WRITE|_IOC_READ)) {
		if (!arg) {
			vipm_err("NULL arg\n");
			return -ENOTTY;
		}
	}

	/*
	* check if user memory is valid or not.
	* if memory can't be accessed from kernel, return error (-EFAULT)
	*/
	if (_IOC_DIR(cmd) & _IOC_WRITE) {
		int no_msg = _VIP_IMG_Debug_Exception(cmd);
		if (from_user) {
			ret = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
			if (ret) {
				vipm_err("access_ok failed(_IOC_WRITE) : arg[0x%lx]\n", arg);
				return -EFAULT;
			}
		}
		ret = VIP_Copy_From((void *)varg, (void *)arg, _IOC_SIZE(cmd), from_user, ((no_msg)<<16) | 0);
		if (!no_msg)	{
			vipm_debug_drv_img("copy_from_user : arg[0x%lx]\n", arg);
		}
	}
	if (_IOC_DIR(cmd) & _IOC_READ) {
		if (from_user) {
			ret = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
			if (ret) {
				vipm_err("access_ok failed(_IOC_READ) : arg[0x%lx]\n", arg);
				return -EFAULT;
			}
		}
	}
	return 0;
}

static int _VIP_IMG_Ioctl_Set_Format(struct vip_top *vtop,
												union vip_arg *varg,
												unsigned int from_user)
{
	struct vip_dev *vip = vip_top_priv(vtop);
	struct vip_fmt_list *ifmt_list;
	struct vip_io_ifmt *vformat = &varg->vfmt;
	int func = vformat->func;
	int reset;
	int found;
	int index;

	if (func >= vip->max_func) {
		vipm_err("Invalid func : func[%d] > max[%d]\n",
				func, vip->max_func);
		return -EINVAL;
	}

	reset = vip_test_bit(VIP_IO_FLAG_RESET, &vformat->flags);
	if (!reset) {
		if (VIP_Chk_Fmt(vformat)) {
			return -EINVAL;
		}
	}

	index = vformat->index;

	ifmt_list = VIP_Find_Fmt_List(vip, func, index);
	found = !!(ifmt_list);

	if (found) {
		if (VIP_Chk_Fmt_Usage(ifmt_list)) {
			vipm_err("Can't access : fmt_list[0x%p] is used!\n", ifmt_list);
			return -EFAULT;
		}
	}

	if (reset) {
		if (found) {
			VIP_Set_Fmt_On_Queue(vip, NULL, func, index);
			VIP_Clr_Fmt_List(ifmt_list);
		} else {
			vipm_warn("Format entry not found\n");
		}
		return 0;
	}

	if (!found) {
		ifmt_list = VIP_Add_Fmt_List(vip);
		if (ifmt_list == NULL) {
			return -ENOMEM;
		}
		VIP_Set_Fmt_On_Queue(vip, ifmt_list, func, index);
	}
	memcpy(&ifmt_list->format, vformat, sizeof(struct vip_io_ifmt));

	return 0;
}

static int _VIP_IMG_Ioctl_Set_Buffer(struct vip_dev *vip,
												union vip_arg *varg,
												unsigned int from_user,
												int input)
{
	struct vip_buf *buf;
	struct vip_io_buffer *buffer;
	int func;
	int index;
	int reset;
	int found;

	buffer = &varg->vbuffer;
	func = buffer->func;

	if (func >= vip->max_func) {
		vipm_err("Invalid func : func[%d] > max[%d]\n",
				func, vip->max_func);
		return -EINVAL;
	}
	vipm_debug_drv_img("func[%d]\n", func);

	if (input == VIP_BUF_OUTPUT) {
		vip_set_bit(VIP_IO_FLAG_INT_NO, &buffer->flags);
	}

	index = buffer->index;

	buf = VIP_Find_Buf(vip, func, index, input);
	found = !!(buf);
	reset = vip_test_bit(VIP_IO_FLAG_RESET, &buffer->flags);

	if (found) {
		if (VIP_Chk_Buf_Usage(buf)) {
			vipm_err("Can't access : buf[0x%p] is used!\n", buf);
			return -EFAULT;
		}

		VIP_Set_Buf_On_Queue(vip, NULL, func, index, input);
		VIP_Clr_Buf(vip, buf, input);
	}

	if (reset) {
		if (!found) {
			vipm_warn("%s Buffer entry not found\n",
					(input == VIP_BUF_INPUT)? "Input" : "Output");
		}
		return 0;
	}

	buf = VIP_Add_Buf(vip, buffer, input);
	if (buf == NULL) {
		return -ENOMEM;
	}
	VIP_Set_Buf_On_Queue(vip, buf, func, index, input);

	__vipm_debug_buffer(VIPM_DBG_SET_DRV_IMG, buffer, input);

	return 0;
}

static int _VIP_IMG_Ioctl_Set_Input_Buffer(struct vip_top *vtop,
												union vip_arg *varg,
												unsigned int from_user)
{
	struct vip_dev *vip = vip_top_priv(vtop);

	return _VIP_IMG_Ioctl_Set_Buffer(vip, varg, from_user, VIP_BUF_INPUT);
}

static int _VIP_IMG_Ioctl_Set_Output_Buffer(struct vip_top *vtop,
												union vip_arg *varg,
												unsigned int from_user)
{
	struct vip_dev *vip = vip_top_priv(vtop);

	return _VIP_IMG_Ioctl_Set_Buffer(vip, varg, from_user, VIP_BUF_OUTPUT);
}

static int _VIP_IMG_Ioctl_Run_Detect(struct vip_top *vtop,
											union vip_arg *varg,
											unsigned int from_user)
{
	struct vip_dev *vip = vip_top_priv(vtop);

	return VIP_IMG_Run_Func(vip, varg, VIP_IMG_FUNC_DETECT, from_user);
}

static int _VIP_IMG_Ioctl_Run_Gesture(struct vip_top *vtop,
											union vip_arg *varg,
											unsigned int from_user)
{
	struct vip_dev *vip = vip_top_priv(vtop);

	return VIP_IMG_Run_Func(vip, varg, VIP_IMG_FUNC_GESTURE, from_user);
}

static int _VIP_IMG_Ioctl_Sub_Ctrl(struct vip_top *vtop,
											union vip_arg *varg,
											unsigned int from_user)
{
	struct vip_dev *vip = vip_top_priv(vtop);
	struct vip_sub_ctrl *vsub_ctrl = &varg->vsub_ctrl;
	int ret = -EINVAL;

	switch (vsub_ctrl->index) {
	case VIP_SUB_CTRL_IDX_SLEEP :
		ret = VIP_Slumber(vip, 0);
		break;
	case VIP_SUB_CTRL_IDX_RESUME :
		ret = VIP_Wakeup(vip, 0);
		break;
	case VIP_SUB_CTRL_IDX_SCENE_CHANGE :
		ret = VIP_Core_Scene_Change(vip);
		break;
	case VIP_SUB_CTRL_IDX_GET_VER :
		ret = VIP_Copy_To((void *)vsub_ctrl->param.lib_ver,
						(void *)&vip->lib_ver,
						sizeof(vip->lib_ver),
						1, 0);
		break;
	case VIP_SUB_CTRL_IDX_SET_MODE :
		vip->set_mode_val = vsub_ctrl->param.set_mode;
		ret = VIP_Core_Set_Mode(vip);
		break;
	case VIP_SUB_CTRL_IDX_SET_RES :
		vip->set_res = vsub_ctrl->param.set_res;
		ret = VIP_Core_Set_Res(vip);
		break;
	/* */
	case VIP_SUB_CTRL_IDX_READ_REG :
	//	ret = VIP_Core_RegPrint(vip);
		break;
	case VIP_SUB_CTRL_IDX_WRITE_REG :
	//	ret = VIP_Core_WriteReg(vip);
		break;
	/* */
	case VIP_SUB_CTRL_IDX_DBG_MASK :
	#if 0
		spin_lock(vip->lock);
		_vip_debug = vsub_ctrl->param.dbg_mask;
		vipm_debug_drv_img("_vip_debug = 0x%08x\n", _vip_debug);
		spin_unlock(vip->lock);
	#else
		VIP_EnableLog(vsub_ctrl->param.dbg_mask);
	#endif
		ret = 0;
		break;
	case VIP_SUB_CTRL_IDX_MEM_TEST :
	//	ret = VIP_Mem_Test(vip);
		ret = 0;
		break;
	default :
		vipm_warn("Unknown index : %d\n", vsub_ctrl->index);
		break;
	}

	return ret;
}

static int _VIP_IMG_Ioctl_State(struct vip_top *vtop,
										union vip_arg *varg,
										unsigned int from_user)
{
	struct vip_dev *vip = vip_top_priv(vtop);
	struct vip_state *vstate = &varg->vstate;
	struct vip_req *areq;

	spin_lock(vip->lock);

	if (vstate->func == -1) {
		memcpy(&vstate->run_err, &vip->vrun_err, sizeof(struct vip_run_err));
		goto out;
	}

	areq = VIP_Find_Queue_Run(vip, vstate->func, VIPM_NO_MSG_FLAG | vstate->index);
	if (areq) {
		vstate->state = VIP_FLAG_MASK_RUN;	/* Running or Ready */
		goto out;
	}

	areq = VIP_Find_Queue_Done(vip, vstate->func, vstate->index);
	if (areq == NULL) {
		vstate->state = 0;
		vstate->fault = 0;
		vstate->error = 0;
		vstate->elapsed = 0;
		vstate->cpytime = 0;
		vipm_warn("Target[%d-%d] request not found\n",
				vstate->func, vstate->index);
		goto out;
	}

#if (__VIP_POST_AT_STATE == 1)
	if (areq->post && !areq->fault && !areq->error) {
		spin_unlock(vip->lock);
		areq->error = areq->post(areq);
		spin_lock(vip->lock);

		VIP_Run_Error(vip, areq->func, areq->index, areq->error);
	}
#endif

	vstate->state = 0;
	vstate->fault = areq->fault;
	vstate->error = areq->error;
	vstate->elapsed = areq->elapsed;
	vstate->cpytime = (areq->ibuf)? areq->ibuf->time_us : 0;

	vipm_debug_drv_img("Target[%d-%d] result : fault[%d], error[%d], elapsed[%d msec]\n",
						vstate->func, vstate->index,
						vstate->fault, vstate->error, vstate->elapsed);

out:
	spin_unlock(vip->lock);
	return 0;
}

static int _VIP_IMG_Ioctl_Abort(struct vip_top *vtop,
										union vip_arg *varg,
										unsigned int from_user)
{
	struct vip_dev *vip = vip_top_priv(vtop);
	struct vip_abort_ctrl *vabort = &varg->vabort;
	int remove;
	int ret = 0;

	if (vabort->code != LX_VIP_IMG_MAGIC) {
		vipm_err("Wrong magic number [0x%08x]\n", vabort->code);
		ret = -EINVAL;
		goto out;
	}

	remove = vip_test_bit(VIP_ABORT_CTRL_FLAG_REMOVE, &vabort->flags);
	vipm_debug_drv_img("func[%d] index[%d] stopped by user, remove(%d)\n",
						vabort->func, vabort->index, remove);
	VIP_Abort_And_Free_Queue(vip, vabort->func, vabort->index, remove);

out:
	return ret;
}

static int _VIP_IMG_Ioctl_Reset(struct vip_top *vtop,
										union vip_arg *varg,
										unsigned int from_user)
{
	struct vip_dev *vip = vip_top_priv(vtop);
	struct vip_magic *vmagic = &varg->vmagic;
	int ret = 0;

	if (vmagic->magic != LX_VIP_IMG_MAGIC) {
		vipm_err("Wrong magic number [0x%08x]\n", vmagic->magic);
		ret = -EINVAL;
		goto out;
	}

	ret = VIP_Core_Reset_Request(vip);

out:
	return ret;
}

static int _VIP_IMG_Ioctl_Show_Curr(struct vip_top *vtop,
									union vip_arg *varg,
									unsigned int from_user)
{
	struct vip_dev *vip = vip_top_priv(vtop);
	struct vip_show_curr *show_curr = &varg->vshow;
	int ret = 0;

	if (show_curr->magic != LX_VIP_IMG_MAGIC) {
		vipm_err("Wrong magic number [0x%08x]\n", show_curr->magic);
		ret = -EINVAL;
		goto out;
	}

	switch (show_curr->index) {
	case VIP_SHOW_STATUS_IDX_QUEUE :
		ret = VIP_Debug_Show_Queue(vip, NULL, 0);
		break;
	}

out:
	return ret;
}

static int _VIP_IMG_Ioctl_Firm_Down(struct vip_top *vtop,
									union vip_arg *varg,
									unsigned int from_user)
{
	int ret = 0;
#if defined(VIP_FIRM_DOWN_FROM_KADAP)
	struct vip_dev *vip = vip_top_priv(vtop);
	struct vip_firm_down *firm_ctrl = &varg->vfirm;
	void *firm_load_addr = NULL;

	if (firm_ctrl->magic != LX_VIP_IMG_MAGIC) {
		vipm_err("Wrong magic number [0x%08x]\n", firm_ctrl->magic);
		ret = -EINVAL;
		goto out;
	}
	if (!firm_ctrl->addr || !firm_ctrl->size) {
		vipm_err("addr [0x%lx], size[%d]\n",
				firm_ctrl->addr, firm_ctrl->size);
		ret = -EINVAL;
		goto out;
	}

	firm_load_addr = (void *)kmalloc(firm_ctrl->size, GFP_KERNEL);
	if (firm_load_addr == NULL) {
		vipm_err("NULL firm_load_addr\n");
		ret = -ENOMEM;
		goto out;
	}

	if (VIP_Copy_From(firm_load_addr, (void *)firm_ctrl->addr, firm_ctrl->size,
					from_user, VIPM_NO_MSG_FLAG | 0))
	{
		kfree(firm_load_addr);
		vipm_err("copy_from_user failed : [0x%lx]\n", firm_ctrl->addr);
		ret = -ENOMEM;
		goto out;
	}

	VIP_Core_FW_Init(vip, (void *)firm_load_addr, firm_ctrl->size);

	ret = VIP_Core_HW_Init(vip);
	if (ret) {
		vipm_err("VIP_Core_HW_Init failed (%d)\n", ret);
		goto out;
	}

out:
#endif

	return ret;
}

static struct vip_ioctl_info vip_img_ioctls[] = {
	VIP_IOCTL_FNC(LX_VIP_IMG_IOW_RESET, _VIP_IMG_Ioctl_Reset, NULL, 0),
	/* */
	VIP_IOCTL_FNC(LX_VIP_IMG_IOW_SET_IFMT, _VIP_IMG_Ioctl_Set_Format, NULL, 0),
	VIP_IOCTL_FNC(LX_VIP_IMG_IOW_SET_IBUFF, _VIP_IMG_Ioctl_Set_Input_Buffer, NULL, 0),
	VIP_IOCTL_FNC(LX_VIP_IMG_IOW_SET_OBUFF, _VIP_IMG_Ioctl_Set_Output_Buffer, NULL, 0),
	/* */
	VIP_IOCTL_FNC(LX_VIP_IMG_IOW_RUN_DETECT, _VIP_IMG_Ioctl_Run_Detect, NULL, 0),
	VIP_IOCTL_FNC(LX_VIP_IMG_IOW_RUN_GESTURE, _VIP_IMG_Ioctl_Run_Gesture, NULL, 0),
	/* */
	VIP_IOCTL_FNC(LX_VIP_IMG_IOWR_STATE, _VIP_IMG_Ioctl_State, NULL, 0),
	VIP_IOCTL_FNC(LX_VIP_IMG_IOW_ABORT, _VIP_IMG_Ioctl_Abort, NULL, 0),
	/* */
	VIP_IOCTL_FNC(LX_VIP_IMG_IOW_SUB_CTRL, _VIP_IMG_Ioctl_Sub_Ctrl, NULL, 0),
	/* */
	VIP_IOCTL_FNC(LX_VIP_IMG_IOW_SHOW_CURR, _VIP_IMG_Ioctl_Show_Curr, NULL, 0),
	/* */
	VIP_IOCTL_FNC(LX_VIP_IMG_IOW_FIRM_DOWN, _VIP_IMG_Ioctl_Firm_Down, NULL, 0),
};
#define VIP_IMG_IOCTL_NUM	(sizeof(vip_img_ioctls) / sizeof(vip_img_ioctls[0]))

static int VIP_IMG_Do_Ioctl(struct vip_top *vtop,
									unsigned int cmd,
									unsigned long arg,
									unsigned int from_user)
{
	struct vip_dev *vip = vip_top_priv(vtop);
	struct vip_ioctl_info *info;
	union vip_arg varg;
	int ret = 0;

	if ((vip == NULL) || !vip->open_count) {
		return -EINVAL;
	}

	if (!_VIP_IMG_Debug_Exception(cmd))	{
		vipm_debug_drv_img("Start : vip[0x%p], cmd = 0x%08x (cmd_idx=0x%x)\n",
						vip, cmd, _IOC_NR(cmd));
	}

	ret = _VIP_IMG_Ioctl_Chk_Arg(&varg, LX_VIP_IMG_IOC_MAGIC, cmd, arg, from_user);
	if (ret) {
		return ret;
	}

	if (_VIP_IMC_Ioctl_Chk_Known(vip_img_ioctls, cmd, VIP_IMG_IOCTL_NUM)) {
	#if 1
		ret = VIP_Core_Init(vip, (struct vip_init_link *)&vip_img_init_link);
		if (ret) {
			vipm_err("VIP_Core_Init failed, %d\n", ret);
			goto out;
		}
	#endif

		info = &vip_img_ioctls[_IOC_NR(cmd)];

		if (info->func) {
			VIP_IMG_LOCK();

			ret = info->func(vtop, &varg, from_user);
			VIP_IMG_UNLOCK();

			if (!_VIP_IMG_Debug_Exception(cmd))	{
				vipm_debug_drv_img("Done : vip[0x%p], cmd = 0x%08x (cmd_idx=0x%x)\n",
								vip, cmd, _IOC_NR(cmd));
			}

			if ((ret >= 0) && (_IOC_DIR(cmd) & _IOC_READ)) {
				int no_msg = _VIP_IMG_Debug_Exception(cmd);
				ret = VIP_Copy_To((void *)arg, (void *)&varg, _IOC_SIZE(cmd), from_user, ((no_msg)<<16) | 0);
				if (!no_msg) {
					vipm_debug_drv_img("copy_to_user : arg[0x%lx]\n", arg);
				}
				if (ret) {
					vipm_err("VIP_Copy_To failed, %d\n", ret);
				//	goto out;
				}
			}

			if (ret) {
				vip_kern_debug("vip_ioctl(%s)\n", info->name);
			}
			else if (!_VIP_IMG_Debug_Exception(cmd))	{
				vipm_debug_drv_img("vip_ioctl(%s)\n", info->name);
			}
		}

		if (_vip_debug & VIPM_DBG_SET_IOCTL) {
			if (info->debug) {
				info->debug(arg);
			}
		}
	} else {
		vipm_err("Unknown command : 0x%08x\n", cmd);
		ret = -ENOIOCTLCMD;
		goto out;
	}

out:
	return ret;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
static int VIP_IMG_Ioctl(struct inode *inode,
								struct file *filp,
								unsigned int cmd,
								unsigned long arg)
{
	return VIP_IMG_Do_Ioctl(filp->private_data, cmd, arg, 1);
}
#else
static long VIP_IMG_Unlocked_Ioctl(struct file *filp,
								unsigned int cmd,
								unsigned long arg)
{

	return (long)VIP_IMG_Do_Ioctl(filp->private_data, cmd, arg, 1);
}
#endif

#if	defined(CONFIG_COMPAT)
static long VIP_IMG_Compat_Ioctl(struct file *filp,
										unsigned int cmd,
										unsigned long arg)
{
	return (long)VIP_IMG_Do_Ioctl(filp->private_data, cmd, (unsigned long)compat_ptr(arg), 1);
}
#endif

#if	defined(CONFIG_COMPAT)
#define	_VIP_IMG_FOPS_COMPAT_IOCTL	VIP_IMG_Compat_Ioctl
#else
#define	_VIP_IMG_FOPS_COMPAT_IOCTL	NULL
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
#define	_VIP_IMG_FOPS_IOCTL			VIP_IMG_Ioctl
#else
#define	_VIP_IMG_FOPS_IOCTL			VIP_IMG_Unlocked_Ioctl
#endif

static const struct file_operations g_vip_img_fops[VIP_CHIP_MAX] =
{
	VIP_FOPS(VIP_CHIP_H15_A0,
			VIP_IMG_Open,
			VIP_IMG_Close,
			NULL,
			_VIP_IMG_FOPS_IOCTL,
			_VIP_IMG_FOPS_COMPAT_IOCTL),
};

void *VIP_IMG_Get_FOPS(void)
{
	void *fops = NULL;

	if (lx_chip_rev() >= LX_CHIP_REV(H15, A0)) {
		fops = (void *)&g_vip_img_fops[VIP_CHIP_H15_A0];
	} else {
		vipm_err("No FOPS in IMG\n");
	}
	return fops;
}



