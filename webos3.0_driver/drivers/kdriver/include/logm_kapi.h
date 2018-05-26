/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file 
 *
 *  application interface header for logm device
 *
 *  author		hwang.hayun (hwang.hayun@lge.com)
 *  version		1.0 
 *  date		2013.10.29
 *
 *  @addtogroup lg115x_logm 
 *	@{
 */

#ifndef	_LOGM_KAPI_H_
#define	_LOGM_KAPI_H_

/*----------------------------------------------------------------------------------------
  Control Constants
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  File Inclusions
  ----------------------------------------------------------------------------------------*/
//#include "base_types.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
  Constant Definitions
  ----------------------------------------------------------------------------------------*/
#define	LOGM_IOC_MAGIC		'a'
/**
@name LOGM IOCTL List
ioctl list for logm device.

@{
@def LOGM_IO_RESET
Reset logm module.
When logm device receives above IOCTL, then reset itself.

@def LOGM_IOR_CHIP_REV_INFO
Reads chip revision information.
Just get chiop revision information. To support automatic changing behavior of device driver or middleware by this ioctl.

@def LOGM_IOW_WRITE_UINT32
write 32bit value to logm module.
detailed...[minimum two lines of comment required. one line of comment does not generated automatically]


*/
/*----------------------------------------------------------------------------------------
  IO comand naming rule  : MODULE_IO[R][W]_COMMAND
  ----------------------------------------------------------------------------------------*/
#define LOGM_IOW_MASK_CTRL		_IOWR(LOGM_IOC_MAGIC, 0 \
									, struct logm_mask_ctrl)
#define LOGM_IOR_OBJMAP_SIZE 	_IOR(LOGM_IOC_MAGIC,  1 \
									, unsigned int)
#define LOGM_IOWR_REG_OBJ 		_IOWR(LOGM_IOC_MAGIC, 2 \
									, struct logm_obj_reg)
#define LOGM_IOW_PATH_CTRL 		_IOW(LOGM_IOC_MAGIC,  3 \
									, unsigned int)
#define LOGM_IOW_USER_WRITE 	_IOW(LOGM_IOC_MAGIC,  4 \
									, struct logm_user_log)

#define LOGM_IOC_MAXNR				5
/** @} */

/*----------------------------------------------------------------------------------------
  Macro Definitions
  ----------------------------------------------------------------------------------------*/
#define LOGM_MODULE_NAME_MAX    (32)
#define LOGM_LOGOBJ_MAX         (200)

#define LOGM_PRINT(fd, l, f, a...)  do{ \
	LOGM_WriteLog(&fd, NULL, (l), __func__, __LINE__, (f), ##a); \
}while(0)

#define LOGM_PATH_CTRL_OFF 		(0)
#define LOGM_PATH_CTRL_ON 		(1)
#define LOGM_SHORT_FORMAT_OFF 	(2)
#define LOGM_SHORT_FORMAT_ON 	(3)
#define LOGM_LOG_COLOR_ON 		(4)
#define LOGM_LOG_COLOR_OFF 		(5)

/*----------------------------------------------------------------------------------------
  Type Definitions
  ----------------------------------------------------------------------------------------*/

typedef enum
{
	LX_LOGM_LOGTYPE_0 = 0,//normal ascii, head(bin) + user(ascii)
	LX_LOGM_LOGTYPE_1,
	LX_LOGM_LOGTYPE_2 = 0,
	LX_LOGM_LOGTYPE_3 = 0,
}LX_LOGM_LOGTYPE_T;

typedef enum
{
	LX_LOGM_LEVEL_ERROR = 0,
	LX_LOGM_LEVEL_WARNING,
	LX_LOGM_LEVEL_NOTI,
	LX_LOGM_LEVEL_INFO,
	LX_LOGM_LEVEL_DEBUG,
	LX_LOGM_LEVEL_TRACE,

	LX_LOGM_LEVEL_MAX = 31,

}LX_LOGM_LOGLEVEL_T;

typedef struct
{
	char name[LOGM_MODULE_NAME_MAX];//key value to know whis one is which
	unsigned int mask;//bit mask
	unsigned int user_mask;//bit mask, just debug to user, if '1' then don't print the log of bit
	LX_LOGM_LOGLEVEL_T level;//level mask
	unsigned int ref_cnt;//reference counter
	unsigned int has_proc;
	unsigned int log_level_cnt[LX_LOGM_LEVEL_TRACE+2];
}LX_LOGM_LOGOBJ_T;

struct logm_obj_bit_name
{
	char name[LX_LOGM_LEVEL_MAX+1][LOGM_MODULE_NAME_MAX];
};

struct logm_mask_ctrl
{
	int fd;
	unsigned int mask;
	unsigned int bit;//bit control
	unsigned int ctrl;
	char *name;
	unsigned int name_size;
	struct logm_obj_bit_name *bit_name;
#define LOGM_MASK_CTRL_WRITE 		0
#define LOGM_MASK_CTRL_READ  		1
#define LOGM_MASK_CTRL_ALL_WR 		2
#define LOGM_MASK_CTRL_BIT_ENABLE 	3
#define LOGM_MASK_CTRL_BIT_DISABLE 	4
#define LOGM_MASK_CTRL_BIT_NAME_WR  5
#define LOGM_MASK_CTRL_BIT_NAME_RD  6

#define LOGM_USER_MASK_CTRL_WRITE 			0x10
#define LOGM_USER_MASK_CTRL_READ  			0x11
#define LOGM_USER_MASK_CTRL_ALL_WR 			0x12
#define LOGM_USER_MASK_CTRL_BIT_ENABLE 		0x13
#define LOGM_USER_MASK_CTRL_BIT_DISABLE 	0x14
};

struct logm_obj_reg
{
	char name[LOGM_MODULE_NAME_MAX];
	int fd;
	unsigned int ctrl;
#define LOGM_OBJ_REGISTER 		0
#define LOGM_OBJ_DEREGISTER 	1
};

struct logm_user_log
{
	int fd;
	LX_LOGM_LOGTYPE_T type;
	LX_LOGM_LOGLEVEL_T level;
	const unsigned char *data;
	int size;
};

/*----------------------------------------------------------------------------------------
  Extern Function Prototype Declaration
  ----------------------------------------------------------------------------------------*/
int LOGM_ObjRegister(const char *name);
int LOGM_ObjDeregister(const char *name);

int LOGM_ObjMaskSet(int fd, unsigned int mask);
int LOGM_ObjMaskGet(int fd, unsigned int *mask);
int LOGM_ObjBitMaskEnable(int fd, int bit);
int LOGM_ObjBitMaskDisable(int fd, int bit);

/* below UserMaskxxxx functions used to debug */
int LOGM_ObjUserMaskSet(int fd, unsigned int mask);
int LOGM_ObjUserMaskGet(int fd, unsigned int *mask);
int LOGM_ObjBitUserMaskEnable(int fd, int bit);
int LOGM_ObjBitUserMaskDisable(int fd, int bit);

int LOGM_WriteLog(int *fd, char *name, LX_LOGM_LOGLEVEL_T level,
		const char *fn, int fl, const char *fmt, ...)
	__attribute__((__format__ (__printf__,6,7)));

int LOGM_RegBitName(int fd, unsigned int bit, char *name);
#define LOGM_REG_CUS_BIT_NAME(fd, bit) LOGM_RegBitName(fd, bit, #bit)
int LOGM_GetBitName(int fd, struct logm_obj_bit_name *bit_name);
/*----------------------------------------------------------------------------------------
  Extern Variables
  ----------------------------------------------------------------------------------------*/
//extern LX_LOGM_LOGOBJ_T g_logm_obj_map[LOGM_LOGOBJ_MAX];
extern LX_LOGM_LOGOBJ_T *g_logm_obj_map;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _LOGM_DRV_H_ */

/** @} */
