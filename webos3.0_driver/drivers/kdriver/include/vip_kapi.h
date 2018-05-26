
#ifndef	_VIP_KAPI_H_
#define	_VIP_KAPI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

#ifndef	BIT
#define BIT(nr)			(1UL << (nr))
#endif

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
//#define VIP_BUFFER_ALIGN		(1<<16)
#define VIP_BUFFER_ALIGN		(1<<6)
#define VIP_BUFFER_MASK			(VIP_BUFFER_ALIGN - 1)

#define	VIP_MODULE			"vip"
//#define VIP_DEV_FILENAME	"/dev/lg/"VIP_MODULE
#define	VIP_CLASS_NAME		"vip_class"
#define VIP_DEVICE_NAME		"vip_device"

#define VIP_DEV_MAJOR		280
#define VIP_DEV_MINOR		0

#define	VIP_MAX_NAME		32

/*
 *
 */
typedef enum
{
	LX_VIP_GR_MODE_OSD = 0,
	LX_VIP_GR_MODE_HOVER
} LX_VIP_GR_MODE;

typedef enum
{
	LX_VIP_GR_CAM_TYPE_BC500 = 0,
	LX_VIP_GR_CAM_TYPE_BC600
} LX_VIP_GR_CAM_TYPE;

/*
 *
 */
enum image_format {
	IMAGE_FORMAT_RGB888 = 0,
	IMAGE_FORMAT_GRAY8,
	IMAGE_FORMAT_MAX,
};

/**
 * VIP Allocated Device Index.
 */
typedef enum {
	LX_VIP_DEV_IMG = 0,
	LX_VIP_DEV_MAX,
} LX_VIP_DEV_T;

#define	VIP_NUM_MINORS		LX_VIP_DEV_MAX

typedef enum {
	LX_VIP_IMG_FUNC_DETECT = 0,
	LX_VIP_IMG_FUNC_GESTURE,
	LX_VIP_IMG_FUNC_MAX,
} LX_VIP_IMG_FUNC_T;

enum vip_img_func {
	VIP_IMG_FUNC_DETECT		= LX_VIP_IMG_FUNC_DETECT,
	VIP_IMG_FUNC_GESTURE	= LX_VIP_IMG_FUNC_GESTURE,
	VIP_IMG_FUNC_MAX		= LX_VIP_IMG_FUNC_MAX,
};

#define	_VIP_IMG_FUNC_ON_DETECT		1
#define	_VIP_IMG_FUNC_ON_GESTURE	0

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

/*
 * VIP ioctl argument
 */
/**
 * VIP Input.
 */
struct vip_magic
{
	unsigned int	magic;
};
typedef struct vip_magic LX_VIP_MAGIC_T;

/*
 *
 */
#define	VIP_MAX_WIDTH		640
#define	VIP_MAX_HEIGHT		480
#define	VIP_MAX_IMG_SIZE	(VIP_MAX_WIDTH * VIP_MAX_HEIGHT * 3)

struct vip_io_ifmt {
	unsigned int	width;
	unsigned int	height;
	unsigned int	stride;
	unsigned int	format;
	unsigned long	flags;
	int				func;
	int				index;
};
typedef struct vip_io_ifmt LX_VIP_IO_IFMT_T;

struct vip_out_buffer_header {
	unsigned int	objs;
	unsigned int	func;
	unsigned int	index;
	unsigned int	magic;
};
typedef struct vip_out_buffer_header LX_VIP_OUT_BUFFER_HEADER_T;
#define LX_VIP_OUT_BUFFER_HEADER_SIZE	sizeof(LX_VIP_OUT_BUFFER_HEADER_T)
#define VIP_OUT_BUFFER_HEADER_SIZE		LX_VIP_OUT_BUFFER_HEADER_SIZE

struct vip_io_buffer {
	unsigned long	addr;
	unsigned long	size;
	unsigned long	data;
	unsigned long	flags;
	int				func;
	int				index;
	void			*priv;
};
typedef struct vip_io_buffer LX_VIP_IO_BUFFER_T;

enum vip_io_buffer_flags {
	/* Clear buffer info. */
	VIP_IO_FLAG_RESET	= 0,	/* Reset buffer setting */
	/* Source buffer option */
	VIP_IO_FLAG_WRITE,			/* Write enable */
	VIP_IO_FLAG_IO,				/* IO address (not real buffer) */
	/* Source buffer info */
	VIP_IO_FLAG_USER	= 8,	/* User space */
	VIP_IO_FLAG_VADDR,			/* Virtual address  : virt_to_phys for DSP */
	VIP_IO_FLAG_PADDR,			/* Physical address : ioremap required */
	/* Use internal buffer : copy buffer */
	VIP_IO_FLAG_INT		= 16,	/* Use internal buffer : use vmalloc or kmalloc */
	VIP_IO_FLAG_INT_FIX,		/* Use internal buffer opt. : use fixed area - 0x3600_0000 */
	VIP_IO_FLAG_INT_CACHE,		/* Use internal buffer opt. : use kmem_cache_create(for boundary control) */
	VIP_IO_FLAG_INT_NO,			/* for output buffer : just memcpy, phys no required */
	/* for User space usage */
	VIP_IO_FLAG_MMAP	= 24,
	/* */
	VIP_IO_FLAG_NO_CONV = 28,
	VIP_IO_FLAG_MAX
};

#define VIP_IO_FLAG_MASK_RESET			BIT(VIP_IO_FLAG_RESET)
#define VIP_IO_FLAG_MASK_WRITE			BIT(VIP_IO_FLAG_WRITE)
#define VIP_IO_FLAG_MASK_IO				BIT(VIP_IO_FLAG_IO)

#define VIP_IO_FLAG_MASK_USER			BIT(VIP_IO_FLAG_USER)
#define VIP_IO_FLAG_MASK_VADDR			BIT(VIP_IO_FLAG_VADDR)
#define VIP_IO_FLAG_MASK_PADDR			BIT(VIP_IO_FLAG_PADDR)

#define VIP_IO_FLAG_MASK_INT			BIT(VIP_IO_FLAG_INT)
#define VIP_IO_FLAG_MASK_INT_FIX		BIT(VIP_IO_FLAG_INT_FIX)
#define VIP_IO_FLAG_MASK_INT_CACHE		BIT(VIP_IO_FLAG_INT_CACHE)
#define VIP_IO_FLAG_MASK_INT_NO			BIT(VIP_IO_FLAG_INT_NO)

#define VIP_IO_FLAG_MASK_MMAP			BIT(VIP_IO_FLAG_MMAP)
#define VIP_IO_FLAG_MASK_NO_CONV		BIT(VIP_IO_FLAG_NO_CONV)

#define VIP_IO_FLAG_MASK_USER_PTR		(VIP_IO_FLAG_MASK_USER)
#define VIP_IO_FLAG_MASK_USER_PTR_IO	(VIP_IO_FLAG_MASK_USER|VIP_IO_FLAG_MASK_IO)

#define	VIP_IO_FLAG_MASK_KERN_VADDR		(VIP_IO_FLAG_MASK_VADDR)
#define	VIP_IO_FLAG_MASK_KERN_VDDR_IO	(VIP_IO_FLAG_MASK_VADDR|VIP_IO_FLAG_MASK_IO)
#define	VIP_IO_FLAG_MASK_KERN_PADDR		(VIP_IO_FLAG_MASK_PADDR)
#define	VIP_IO_FLAG_MASK_KERN_PADDR_IO	(VIP_IO_FLAG_MASK_PADDR|VIP_IO_FLAG_MASK_IO)

#define	VIP_IO_FLAG_MASK_KERN_ADDR		(VIP_IO_FLAG_MASK_VADDR|VIP_IO_FLAG_MASK_PADDR)

#define	VIP_IO_FLAG_MASK_FIX			(VIP_IO_FLAG_INT|VIP_IO_FLAG_INT_FIX)
#define	VIP_IO_FLAG_MASK_CACHE			(VIP_IO_FLAG_INT|VIP_IO_FLAG_INT_CACHE)

struct vip_run_ctrl {
	int				index;
//	struct vip_io_ifmt		vfmt;
//	struct vip_io_buffer	vibuffer;
//	struct vip_io_buffer	vobuffer;
	unsigned long	flags;
};
typedef struct vip_run_ctrl LX_VIP_RUN_CTRL_T;

enum vip_run_ctrl_flags {
	VIP_RUN_CTRL_FLAG_RESET	= 0,
	VIP_RUN_CTRL_FLAG_SCENE_CHANGE,
	VIP_RUN_CTRL_FLAG_MAX,
};

#define VIP_RUN_CTRL_FLAG_MASK_RESET			BIT(VIP_RUN_CTRL_FLAG_RESET)
#define VIP_RUN_CTRL_FLAG_MASK_SCENE_CHANGE		BIT(VIP_RUN_CTRL_FLAG_SCENE_CHANGE)

struct vip_sub_ctrl {
	int			index;
	union {
		unsigned int	dbg_mask;
		unsigned int	lib_ver;
		unsigned long	set_mode;
		unsigned long	set_res;
		unsigned long	priv;
	} param;
};
typedef struct vip_sub_ctrl LX_VIP_SUB_CTRL_T;

enum vip_sub_ctrl_index {
	VIP_SUB_CTRL_IDX_SLEEP = 0,
	VIP_SUB_CTRL_IDX_RESUME,
	VIP_SUB_CTRL_IDX_SCENE_CHANGE,
	VIP_SUB_CTRL_IDX_GET_VER,
	VIP_SUB_CTRL_IDX_SET_MODE,
	VIP_SUB_CTRL_IDX_SET_RES,
	/* */
	VIP_SUB_CTRL_IDX_READ_REG = 8,
	VIP_SUB_CTRL_IDX_WRITE_REG,
	/* */
	VIP_SUB_CTRL_IDX_DBG_MASK = 16,
	/* */
	VIP_SUB_CTRL_IDX_MEM_TEST = 24,
	VIP_SUB_CTRL_IDX_MAX,
};

struct vip_show_curr {
	int				index;
	unsigned int	magic;
};
typedef struct vip_show_curr LX_VIP_SHOW_CURR_T;

enum vip_show_status_index {
	VIP_SHOW_STATUS_IDX_INFO	= 0,
	VIP_SHOW_STATUS_IDX_QUEUE,
	VIP_SHOW_STATUS_IDX_MAX,
};

struct vip_firm_down {
	unsigned long	addr;
	unsigned int	size;
	unsigned int	magic;
};
typedef struct vip_firm_down LX_VIP_FIRM_DOWN_T;

struct vip_run_err {
	int			func;
	int			index;
	int			result;
};
typedef struct vip_run_err LX_VIP_RUN_ERR_T;

struct vip_state {
	int					func;
	int					index;
	unsigned int		state;
	unsigned int		fault;
	unsigned int		error;
	unsigned int		elapsed;
	unsigned int		cpytime;
	struct vip_run_err	run_err;
};
typedef struct vip_state LX_VIP_STATE_T;

struct vip_abort_ctrl {
	unsigned int	code;
	int				func;
	int				index;
	unsigned long	flags;
};
typedef struct vip_abort_ctrl LX_VIP_ABORT_CTRL_T;

enum vip_abort_ctrl_flags {
	VIP_ABORT_CTRL_FLAG_REMOVE = 0,
	VIP_ABORT_CTRL_FLAG_MAX,
};

#define VIP_ABORT_CTRL_FLAG_MASK_REMOVE			BIT(VIP_ABORT_CTRL_FLAG_REMOVE)

/*
 * VIP ioctl argument
 */
union vip_arg {
	/* reset with magic */
	struct vip_magic			vmagic;
	/* Image format */
	struct vip_io_ifmt			vfmt;
	/* Input/Output buffer */
	struct vip_io_buffer		vbuffer;
	/* Run control */
	struct vip_run_ctrl			vrun_ctrl;
	/* Sub control */
	struct vip_sub_ctrl			vsub_ctrl;
	/* State check */
	struct vip_state			vstate;
	/* */
	struct vip_abort_ctrl		vabort;
	/* */
	struct vip_show_curr		vshow;
	/* */
	struct vip_firm_down		vfirm;
	/* */
	unsigned char				vargb[sizeof(struct vip_io_buffer)];
};
typedef union vip_arg LX_VIP_ARG_T;

/*
 *
 */
enum vip_gr_event_result {
	VIP_GR_NONE = 0,
	VIP_GR_EVENT_LEFT,
	VIP_GR_EVENT_RIGHT,
	VIP_GR_EVENT_UP,
	VIP_GR_EVENT_DOWN,
	VIP_GR_EVENT_WHEELUP,
	VIP_GR_EVENT_WHEELDOWN,
	VIP_GR_EVENT_VOLUP,
	VIP_GR_EVENT_VOLDOWN,
	VIP_GR_EVENT_CHUP,
	VIP_GR_EVENT_CHDOWN,
	VIP_GR_EVENT_BACK,
	VIP_GR_EVENT_HOME
};

enum vip_gr_mode {
	VIP_GR_MODE_OSD = 0,	//OSD 기반 Mode
	VIP_GR_MODE_HOVER		//Cursor 기반 Mode
};

enum vip_gr_cam_type {
	VIP_GR_CAM_TYPE_BC500 = 0,	//BC500, VC500 Camera 사용 시
	VIP_GR_CAM_TYPE_BC600		//BC600 Camera 사용 시
};

enum vip_gr_alter {
	VIP_GR_ALERT_NONE = 0,
    VIP_GR_ALERT_PALM_DETECTED,	// 초기 인식 시 손바닥이 인식 되었을 경우
	VIP_GR_ALERT_SHHH_DETECTED	// 쉿 제스처가 인식 되었을 경우
};

struct vip_msg_crop {
	int enable;			//활성화 : 1, 비활성화 : 0
	int offsetx;		//Crop 시작위치 X
	int offsety;		//Crop 시작 위치 Y
	int width;			//Crop 영역 너비
	int height;			//Crop 영역 높이
};
typedef struct vip_msg_crop LX_VIP_MSG_CROP_S_T;

struct vip_msg_gesture {
	int posx;				//Hover Mode 시 Cursor X 좌표, 인식된 손이 없으면 -1
	int posy;				//Hover Mode 시 Cursor Y 좌표, 인식된 손이 없으면 -1
	int handposx;			//Hand 초기 인식 좌표 X, 인식된 손이 없으면 -1
	int handposy;			//Hand 초기 인식 좌표 Y, 인식된 손이 없으면 -1
	int osd_button_num;		//OSD Mode에서 현재 가르키고 있는 OSD Button 번호 값(0 ~ 5), 인식된 손이 없으면 -1
	int click;				//Click Gesture 인식 결과, 손을가락을 구부려 클릭한 상태이면 1, 손가락을 펴고 있는 상태이면 0
	int event_result;		//기타 제스처 인식 값, 추후 기능 확장을 위해 만들어 놓은 Interface로 현재는 지원 제스처가 없음, GESTURE_EVENT_NONE
	int gesture_alert;		//기타 제스처 알림값, 아무 알림 없을 경우 GESTURE_ALERT_NONE
};
typedef struct vip_msg_gesture LX_VIP_MSG_GESTURE_S_T;

#define VIP_MSG_XY_REVERSE			1
#define VIP_MSG_WH_REVERSE			1

struct vip_msg_detect
{
#if (VIP_MSG_XY_REVERSE > 0)
	unsigned short	y16;
	unsigned short	x16;
#else
	unsigned short	x16;
	unsigned short	y16;
#endif
#if (VIP_MSG_WH_REVERSE > 0)
	unsigned short	h16;
	unsigned short	w16;
#else
	unsigned short	w16;
	unsigned short	h16;
#endif
	unsigned short	id16;
	unsigned short	recog_info16;
};
typedef struct vip_msg_detect	LX_VIP_MSG_DETECT_S_T;

#define	VIP_MSG_MAX_OBJ_DETECT		10
#define	VIP_MSG_BUF_SIZE_DETECT		128

#define	VIP_MSG_MAX_OBJ_GESTURE		1
#define	VIP_MSG_BUF_SIZE_GESTURE	16

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	LX_VIP_IOC_MAGIC				'I'
#define	LX_VIP_IMG_IOC_MAGIC			LX_VIP_IOC_MAGIC

#define	LX_VIP_IMG_MAGIC				(0x56495049)	/* "VIPI" */

/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
/* IO Command Name */
#define LX_VIP_IMG_IOW_RESET			_IOW(LX_VIP_IMG_IOC_MAGIC, 0, LX_VIP_MAGIC_T)

#define LX_VIP_IMG_IOW_SET_IFMT			_IOW(LX_VIP_IMG_IOC_MAGIC, 1, LX_VIP_IO_IFMT_T)
#define LX_VIP_IMG_IOW_SET_IBUFF		_IOW(LX_VIP_IMG_IOC_MAGIC, 2, LX_VIP_IO_BUFFER_T)
#define LX_VIP_IMG_IOW_SET_OBUFF		_IOW(LX_VIP_IMG_IOC_MAGIC, 3, LX_VIP_IO_BUFFER_T)

#define LX_VIP_IMG_IOW_RUN_DETECT		_IOW(LX_VIP_IMG_IOC_MAGIC, 4, LX_VIP_RUN_CTRL_T)
#define LX_VIP_IMG_IOW_RUN_GESTURE		_IOW(LX_VIP_IMG_IOC_MAGIC, 5, LX_VIP_RUN_CTRL_T)

#define LX_VIP_IMG_IOWR_STATE			_IOWR(LX_VIP_IMG_IOC_MAGIC, 8, LX_VIP_STATE_T)
#define LX_VIP_IMG_IOW_ABORT			_IOW(LX_VIP_IMG_IOC_MAGIC, 9, LX_VIP_ABORT_CTRL_T)

#define	LX_VIP_IMG_IOW_SUB_CTRL			_IOW(LX_VIP_IMG_IOC_MAGIC, 10, LX_VIP_SUB_CTRL_T)

#define LX_VIP_IMG_IOW_SHOW_CURR		_IOW(LX_VIP_IMG_IOC_MAGIC, 11, LX_VIP_SHOW_CURR_T)

#define	LX_VIP_IMG_IOW_FIRM_DOWN		_IOW(LX_VIP_IMG_IOC_MAGIC, 12, LX_VIP_FIRM_DOWN_T)

#define	LX_VIP_IMG_IOC_CHECK			_IO(LX_VIP_IMG_IOC_MAGIC, 0x80)

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* _VIP_KAPI_H_ */


