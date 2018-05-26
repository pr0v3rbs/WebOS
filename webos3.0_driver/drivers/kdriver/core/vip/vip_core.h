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
 *  main driver implementation for vision processor device.
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

#ifndef _VIP_CORE_H_
#define _VIP_CORE_H_

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------*/
#ifdef CONFIG_64BIT
#define __BITS_PER_LONG 64
#else
#define __BITS_PER_LONG 32
#endif /* CONFIG_64BIT */

#ifndef unlikely
#define	unlikely(x)		x
#endif

#ifndef likely
#define likely(x)		x
#endif

#ifndef __used
#define __used
#endif

#if	0
static inline void vip_set_bit(int nr, unsigned long *addr)
{
	addr[nr / __BITS_PER_LONG] |= 1UL << (nr % __BITS_PER_LONG);
}

static inline void vip_clear_bit(int nr, unsigned long *addr)
{
	addr[nr / __BITS_PER_LONG] &= ~(1UL << (nr % __BITS_PER_LONG));
}

static inline int vip_test_bit(int nr, const volatile unsigned long *addr)
{
	return 1UL & (addr[nr/__BITS_PER_LONG] >> (nr & (__BITS_PER_LONG-1)));
}
#else
#define vip_set_bit(nr, addr)		set_bit(nr, addr)
#define vip_clear_bit(nr, addr)		clear_bit(nr, addr)
#define vip_test_bit(nr, addr)		test_bit(nr, addr)
#endif

/*----------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------*/
static inline void VIP_Delay_MS(unsigned int ms)
{
//	OS_MsecDelay(ms);
//	mdelay(ms);
	usleep_range(ms*1000, ms*1000);
}

static inline void VIP_Delay_US(unsigned int us)
{
/*
	if (us > 1000){
		VIP_Delay_MS(us/1000);
		us %= 1000;
	}
//	OS_UsecDelay(us);
	udelay(us);
*/
	usleep_range(us, us);
}

/*----------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------*/
static inline int VIP_Is_Busy(struct vip_dev *vip)
{
//	unsigned long flags;
	int busy = 0;

//	spin_lock_irqsave(vip->lock, flags);
	busy = (vip->flags & VIP_FLAG_MASK_OPERATION);
//	spin_unlock_irqrestore(vip->lock, flags);
	return busy;
}

/*----------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------*/
struct vip_ioctl_info {
	unsigned int		ioctl;
	unsigned int		flags;
	const char			*name;
	int					(*func)(struct vip_top *vtop,
								union vip_arg *varg,
								unsigned int from_user);
	void				(*debug)(unsigned long arg);
};

#define VIP_IOCTL_FNC(_ioctl, _func, _debug, _flags)	\
	[_IOC_NR(_ioctl)] = {		\
		.ioctl = _ioctl,		\
		.flags = _flags,		\
		.name = #_ioctl,		\
		.func = _func,			\
		.debug = _debug,		\
	}

/*
 * file I/O description for vip device
 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
#define	VIP_FOPS(_idx, _open, _release, _mmap, _ioctl, _cioctl)	\
	[_idx] = {						\
		.open			= _open,	\
		.release		= _release,	\
		.mmap			= _mmap,	\
		.lioctl			= _ioctl,	\
		.compat_ioctl	= _cioctl,	\
	}
#else
#define	VIP_FOPS(_idx, _open, _release, _mmap, _ioctl, _cioctl)	\
	[_idx] = {						\
		.open			= _open,	\
		.release		= _release,	\
		.mmap			= _mmap,	\
		.unlocked_ioctl	= _ioctl,	\
		.compat_ioctl	= _cioctl,	\
	}
#endif

/*----------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------*/
static inline int VIP_Is_Registered(struct vip_dev *vip)
{
	return vip_test_bit(VIP_FLAG_REGISTERED, &vip->flags);
}

static inline int VIP_Clr_Registered(struct vip_dev *vip)
{
	vip_clear_bit(VIP_FLAG_REGISTERED, &vip->flags);
	return 0;
}

static inline int VIP_Set_Registered(struct vip_dev *vip)
{
	vip_set_bit(VIP_FLAG_REGISTERED, &vip->flags);
	return 0;
}

static inline int VIP_Is_Opened(struct vip_dev *vip)
{
	return vip_test_bit(VIP_FLAG_OPENED, &vip->flags);
}

static inline int VIP_Clr_Opened(struct vip_dev *vip)
{
	vip_clear_bit(VIP_FLAG_OPENED, &vip->flags);
	return 0;
}

static inline int VIP_Set_Opened(struct vip_dev *vip)
{
	vip_set_bit(VIP_FLAG_OPENED, &vip->flags);
	return 0;
}

static inline int VIP_Is_Inited(struct vip_dev *vip)
{
	return vip_test_bit(VIP_FLAG_INITED, &vip->flags);
}

static inline int VIP_Clr_Inited(struct vip_dev *vip)
{
	vip_clear_bit(VIP_FLAG_INITED, &vip->flags);
	return 0;
}

static inline int VIP_Set_Inited(struct vip_dev *vip)
{
	vip_set_bit(VIP_FLAG_INITED, &vip->flags);
	return 0;
}

static inline int VIP_Is_HW_Inited(struct vip_dev *vip)
{
	return vip_test_bit(VIP_FLAG_HW_INITED, &vip->flags);
}

static inline int VIP_Clr_HW_Inited(struct vip_dev *vip)
{
	vip_clear_bit(VIP_FLAG_HW_INITED, &vip->flags);
	return 0;
}

static inline int VIP_Set_HW_Inited(struct vip_dev *vip)
{
	vip_set_bit(VIP_FLAG_HW_INITED, &vip->flags);
	return 0;
}

static inline int VIP_Is_Dsp(struct vip_dev *vip)
{
	return vip_test_bit(VIP_FLAG_DSP, &vip->flags);
}

static inline int VIP_Clr_Dsp(struct vip_dev *vip)
{
	vip_clear_bit(VIP_FLAG_DSP, &vip->flags);
	return 0;
}

static inline int VIP_Set_Dsp(struct vip_dev *vip)
{
	vip_set_bit(VIP_FLAG_DSP, &vip->flags);
	return 0;
}

static inline int VIP_Clr_Dsp_Lock(struct vip_dev *vip)
{
	spin_lock(vip->lock);
	vip_clear_bit(VIP_FLAG_DSP, &vip->flags);
	spin_unlock(vip->lock);
	return 0;
}

static inline int VIP_Set_Dsp_Lock(struct vip_dev *vip)
{
	spin_lock(vip->lock);
	vip_set_bit(VIP_FLAG_DSP, &vip->flags);
	spin_unlock(vip->lock);
	return 0;
}

static inline int VIP_Is_Run(struct vip_dev *vip)
{
	return vip_test_bit(VIP_FLAG_RUN, &vip->flags);
}

static inline int VIP_Clr_Run(struct vip_dev *vip)
{
	vip_clear_bit(VIP_FLAG_RUN, &vip->flags);
	return 0;
}

static inline int VIP_Set_Run(struct vip_dev *vip)
{
	vip_set_bit(VIP_FLAG_RUN, &vip->flags);
	return 0;
}

static inline int VIP_Test_Areq_Flags(struct vip_req *areq, int bit)
{
	return vip_test_bit(bit, &areq->aflags);
}

static inline int VIP_Clr_Areq_Flags(struct vip_req *areq, int bit)
{
	vip_clear_bit(bit, &areq->aflags);
	return 0;
}

static inline int VIP_Set_Areq_Flags(struct vip_req *areq, int bit)
{
	vip_set_bit(bit, &areq->aflags);
	return 0;
}

static inline int VIP_Rst_Areq_Flags(struct vip_req *areq)
{
	areq->aflags = 0;
	return 0;
}

static inline void VIP_HW_Top_Writel(struct vip_dev *vip,
											unsigned int val,
											unsigned int idx)
{
	if (vip->vhal_ops && vip->vhal_ops->top_writel) {
		vip->vhal_ops->top_writel(vip, val, idx);
	}
}

static inline int VIP_HW_Top_Readl(struct vip_dev *vip,
											unsigned int idx)
{
	if (vip->vhal_ops && vip->vhal_ops->top_readl) {
		return vip->vhal_ops->top_readl(vip, idx);
	}
	return -1;
}

static inline void VIP_HW_IPC_Writel(struct vip_dev *vip,
											unsigned int val,
											unsigned int idx)
{
	if (vip->vhal_ops && vip->vhal_ops->top_writel) {
		vip->vhal_ops->top_writel(vip, val, idx);
	}
}

static inline int VIP_HW_IPC_Readl(struct vip_dev *vip,
											unsigned int idx)
{
	if (vip->vhal_ops && vip->vhal_ops->ipc_readl) {
		return vip->vhal_ops->ipc_readl(vip, idx);
	}
	return -1;
}

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern int vip_io_lock(void);
extern void vip_io_unlock(void);

extern int vip_mutex_lock(struct vip_dev *vip);
extern void vip_mutex_unlock(struct vip_dev *vip);

/*
 *
 */
extern int VIP_Core_HW_Init(struct vip_dev *vip);

extern int VIP_Core_Reset(struct vip_dev *vip);
extern int VIP_Core_Reset_Check(struct vip_dev *vip);
extern int VIP_Core_Reset_Request(struct vip_dev *vip);

extern int VIP_Core_Sleep(struct vip_dev *vip);
extern int VIP_Core_Resume(struct vip_dev *vip);

extern int VIP_Core_Scene_Change(struct vip_dev *vip);

extern int VIP_Core_Set_Mode(struct vip_dev *vip);
extern int VIP_Core_Set_Res(struct vip_dev *vip);

/*
 *
 */
extern int VIP_Clk_Put(struct vip_dev *vip);
extern int VIP_Clk_Get(struct vip_dev *vip);

extern int VIP_Disable_IRQ(struct vip_dev *vip);
extern int VIP_Enable_IRQ(struct vip_dev *vip);

extern int VIP_Slumber(struct vip_dev *vip, int opt);
extern int VIP_Wakeup(struct vip_dev *vip, int opt);

/*
 *
 */
extern int VIP_Unregister_Device(struct vip_top *vtop);
extern int VIP_Register_Device(struct vip_top *vtop,
									struct file_operations *fops,
									dev_t devNo);

extern void VIP_Chrdev_Free(dev_t devt, int count);
extern int VIP_Chrdev_Region(dev_t *devt, int *major,
									int count, char *name);

extern void VIP_Free_Device(struct vip_top *vtop);
extern struct vip_top *VIP_Alloc_Device(int idx);

/*
 *
 */
extern void VIP_Thread_Stop(struct vip_dev *vip);
extern int VIP_Thread_Init(struct vip_dev *vip, void *thread);

extern int VIP_Run_Thread(void *d);
extern void VIP_Wakeup_Thread(struct vip_dev *vip);

extern int VIP_IRQ_Status(struct vip_dev *vip);
extern void VIP_IRQ_Clear(struct vip_dev *vip, unsigned int intmask);
extern int VIP_IRQ_For_Request(struct vip_req *areq);
extern irqreturn_t VIP_IRQ_Handler(int irq, void *dev_id);

/*
 *
 */
extern int VIP_Set_Completion(struct vip_req *areq, int fault);
extern int VIP_Done_Completion(struct vip_req *areq, int fault);
extern void VIP_Finish_Request(struct vip_req *areq);

extern void VIP_Reset_Gesture_Data(struct vip_msg_gesture *gesture);
extern void VIP_Reset_Crop_Data(struct vip_msg_crop *crop);

extern void VIP_Run_Error(struct vip_dev *vip,
						int func,
						int index,
						int result);
extern void VIP_Run_Err_Rst(struct vip_dev *vip);
extern int VIP_Run_Request(struct vip_req *areq);
extern void VIP_Add_Request(struct vip_req *areq,
							unsigned long timeout,
							int (*post)(struct vip_req *),
							int from_user);

extern int VIP_Chk_Fmt(struct vip_io_ifmt *format);

extern struct vip_req *VIP_Get_Request(struct vip_dev *vip,
										int func,
										int index,
										int *result);

extern int VIP_Gen_Request(struct vip_req *areq,
							unsigned long timeout,
							int (*post)(struct vip_req *));

extern int VIP_Get_Obj_Info(struct vip_req *areq);

extern int VIP_Core_FW_Cleanup(struct vip_dev *vip);
extern int VIP_Core_FW_Init(struct vip_dev *vip, void *addr, unsigned int size);

/*----------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------*/
extern void VIP_Core_Cleanup(struct vip_dev *vip);
extern int VIP_Core_Init(struct vip_dev *vip,
						struct vip_init_link *link);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* _VIP_CORE_H_ */



