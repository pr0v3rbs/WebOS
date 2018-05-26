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
 *  main configuration file for lgbus device
 *  lgbus device will teach you how to make device driver with new platform.
 *
 *  @addtogroup lg115x_lgbus
 *  @{
 */

/*
	DOMAIN(L/G/E/CCI)式式式式成式式式式CMN(common control)
                         戍式式式式BTOP(bus top)
                         戌式式式式MTOP(memory top)
*/
#ifndef __LGBUS_H15_H__
#define __LGBUS_H15_H__

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "lgbus_kapi.h"
#include <linux/completion.h>

#ifdef  __cplusplus
extern "C"
{
#endif /* __cplusplus */


/*----------------------------------------------------------------------------------------
    Constant Definitions
----------------------------------------------------------------------------------------*/
#define GPU_REQ			lgbus_ctrl.mon_info.result[LBUS_IDX].raw[MNT_GPU_IDX].requestd
#define MHZ 1000000

#define	BUS_MON_MAJOR			261
#define	BUS_MON_NODE_NAME		"/dev/lg/busmon"

#ifdef FPGA
#define LGEBUS_FREQ				5
#else
#define LGEBUS_FREQ				396
#endif

#define CCI_FREQ				468

/* DO NOT use this value. DDR Total BW is calculated ! */
#ifdef FPGA
#define H15_DDR_TOTAL_BW		124800000 //936MHz/80 * 2 * 4byte = 93.6MB/sec , 93.6MB*4/3 = 124.8MB/Sync
#else
#define H15_DDR_TOTAL_BW		7488000000 //936MHz * 2(ddr) * 4byte(32bit) = 7488MB/Sync
#endif

#define STR_H15_DDR_TOTAL_BW	"124,800,000"

/*
| Error Code Definitions
*/
#define	RET_OK					0			///< success
#define RET_ERROR				-EIO		///< general error
#define RET_INVALID_PARAMS		-EINVAL		///< invalid paramter
#define RET_INVALID_IOCTL		-ENOTTY		///< invalid ioctl request
#define RET_OUT_OF_MEMORY		-ENOMEM		///< out ot memory
#define RET_TIMEOUT				-ETIME		///< timeout
#define RET_TRY_AGAIN			-EAGAIN		///< try again
#define RET_INTR_CALL			-EINTR		///< interrupted system call

#define BLK_MON_NOT_REQED			0
#define BLK_MON_REQED				1

//NOC_L_QOS MASTERS
#define LG1210_LBUS_REG_CMN         (0xC880C000)    //common control register for timer interval
#define LG1210_LBUS_REG_BUS_GPU 	(0xC880A000)
#define LG1210_LBUS_REG_BUS_GFX 	(0xC880A200)
#define LG1210_LBUS_REG_BUS_TE		(0xC880A400)
#define LG1210_LBUS_REG_BUS_ICOD	(0xC880A600)
#define LG1210_LBUS_REG_BUS_VENC	(0xC880A800)
#define LG1210_LBUS_REG_BUS_AUD		(0xC880AA00)
#define LG1210_LBUS_REG_BUS_VIP		(0xC880AC00)

// NOC_G_QOS MASTERS
#define LG1210_GBUS_REG_CMN 	    (0xC8457000)    //common control register for timer interval
#define LG1210_GBUS_REG_BUS_GPU 	(0xC8455000)
#define LG1210_GBUS_REG_BUS_GFX 	(0xC8455200)
#define LG1210_GBUS_REG_BUS_VDEC0_M0 (0xC8455400)
#define LG1210_GBUS_REG_BUS_VDEC0_M1 (0xC8455600)
#define LG1210_GBUS_REG_BUS_VDEC1 	(0xC8455800)
#define LG1210_GBUS_REG_BUS_VDEC2 	(0xC8455A00)
#define LG1210_GBUS_REG_BUS_OSD		(0xC8455C00)

// NOC_E_QOS MASTERS
#define LG1210_EBUS_REG_CMN 	   		(0xC8217000)    //common control register for timer interval
#define LG1210_EBUS_REG_BUS_VDEC1		(0xC8213000)
#define LG1210_EBUS_REG_BUS_VDEC2		(0xC8213200)
#define LG1210_EBUS_REG_BUS_OSD 	    (0xC8213400)
#define LG1210_EBUS_REG_BUS_VDM0 	    (0xC8213600)
#define LG1210_EBUS_REG_BUS_VDM1 	    (0xC8213800)
#define LG1210_EBUS_REG_BUS_SMX0 	    (0xC8213A00)
#define LG1210_EBUS_REG_BUS_SMX1 	    (0xC8213C00)
#define LG1210_EBUS_REG_BUS_MCU 	    (0xC8213E00)
#define LG1210_EBUS_REG_BUS_CVD 	    (0xC8214000)
#define LG1210_EBUS_REG_BUS_ND0 	    (0xC8214200)
#define LG1210_EBUS_REG_BUS_ND1 	    (0xC8214400)
#define LG1210_EBUS_REG_BUS_GSC0 	    (0xC8214600)
#define LG1210_EBUS_REG_BUS_GSC1 	    (0xC8214800)
#define LG1210_EBUS_REG_BUS_VCP0 	    (0xC8214A00)
#define LG1210_EBUS_REG_BUS_VCP1 	    (0xC8214C00)
#define LG1210_EBUS_REG_BUS_SUB 	    (0xC8214E00)

//CCI MASTERS jaehyun add
#define LG1210_CCI_REG_BUS_GPU			(0xC8C00000)
#define LG1210_CCI_REG_BUS_PERI			(0xC8C00100)
#define LG1210_CCI_REG_BUS_CPU			(0xC8C00200)


//MTOP_L_MASTER
#define LG1210_LBUS_MTOP_BUS_M00 	    (0xC880AE00)
#define LG1210_LBUS_MTOP_BUS_M01 	    (0xC880AE40)
#define LG1210_LBUS_MTOP_BUS_M02 	    (0xC880AE80)
#define LG1210_LBUS_MTOP_BUS_M03 	    (0xC880AEC0)
//MTOP_G_MASTER
#define LG1210_GBUS_MTOP_BUS_M00 	    (0xC8455E00)
#define LG1210_GBUS_MTOP_BUS_M01 	    (0xC8455E40)
#define LG1210_GBUS_MTOP_BUS_M02 	    (0xC8455E80)
#define LG1210_GBUS_MTOP_BUS_M03 	    (0xC8455EC0)
//MTOP_E_MASTER
#define LG1210_EBUS_MTOP_BUS_M00 	    (0xC8215000)
#define LG1210_EBUS_MTOP_BUS_M01 	    (0xC8215040)
#define LG1210_EBUS_MTOP_BUS_M02 	    (0xC8215080)
#define LG1210_EBUS_MTOP_BUS_M03 	    (0xC82150C0)

//MTOP_CCI_MASTER
#define LG1210_CCI_MTOP_BUS_M00			(0xC8C00300)
#define LG1210_CCI_MTOP_BUS_M01			(0xC8C00340)
#define LG1210_CCI_MTOP_BUS_M02			(0xC8C00380)


//2014.03.20
#define DE_REG_SMX_BASE			   		(0xC8003000)
#define DE_REG_SMX0_R_MIF		   		(0xC8003C10)
#define DE_REG_SMX0_W_MIF		   		(0xC8003CB0)
#define DE_REG_SMX1_R_MIF		   		(0xC8003e10)
#define DE_REG_SMX1_W_MIF		   		(0xC8003eB0)

#define DE_REG_BUS_ND0_W_MIF			(0xC8004f30)
#define DE_REG_BUS_ND1_W_MIF			(0xC8005f30)
#define DE_REG_BUS_ND0_R_MIF			(0xC8004e14)
#define DE_REG_BUS_ND1_R_MIF			(0xC8005e14)
#define DE_REG_GSC0_SM_W_MIF			(0xC8007CB4)
#define DE_REG_GSC1_SM_W_MIF			(0xC8007eB4)

//DE_PATH
#define DE_REG_BASE			    		(0xC8004000)
#define DE_REG_BUS_ND0_PIC_INIT			(0xC8004108)
#define DE_REG_BUS_ND0					(0xC800410C)
#define DE_REG_BUS_ND1_PIC_INIT			(0xC8005108)
#define DE_REG_BUS_ND1					(0xC800510C)
#define DE_REG_BUS_NDS_PIC_INIT			(0xC8006108)
#define DE_REG_BUS_NDS					(0xC800610C)
#define DE_REG_SUB_MIF					(0xC8006F34)
#define DE_REG_BUS_DNR0					(0xC800710C)
#define DE_REG_BUS_DNR1					(0xC800730C)
#define DE_REG_GSC0_PE0_W_MIF			(0xC8007CB0)
#define DE_REG_GSC1_PE0_W_MIF			(0xC8007EB0)

//VCP
#define VCP_REG_BUS_U1Y					(0xC800C094)
#define VCP_REG_BUS_U1C					(0xC800C134)
#define VCP_REG_BUS_F1Y					(0xC800C1D0)
#define VCP_REG_BUS_F1C					(0xC800C1D4)

//OSD
#define OSD_REG_BUS_0					(0xC8024300)
#define OSD_REG_BUS_1					(0xC8024380)
#define OSD_REG_BUS_2					(0xC8024400)
#define OSD_REG_BUS_3					(0xC8024480)
#define OSD_REG_BUS_4					(0xC8024500)
#define OSD_REG_BUS_5					(0xC8024580)
#define OSD_REG_BUS_6					(0xC8024600)
#define OSD_REG_BUS_7					(0xC8024680)

//APB
#define APB_REG_BASE					(0xFD300300)
#define MNT_APB_FAST_PREADY		0xE0


/*
// PORT MASTER
*/
#define MNT_PORT_ENABLE_R		0x30
#define MNT_PORT_NUM_R			0x38
#define MNT_PORT_ENABLE_W		0x30
#define MNT_PORT_NUM_W			0x34

/*
; Monitor register offset for each MASTERS
*/
// CMN TOP
#define MNT_RF_TIMER_INIT_VALUE     0x08

// BUS TOP
#define MNT_RF_BW_MON_BW0_PREV_W	0x80	//LM
#define MNT_RF_BW_MON_BW0_PREV_R	0x84
#define MNT_RF_BW_MON_BW1_PREV_W	0x90	//GM
#define MNT_RF_BW_MON_BW1_PREV_R	0x94
#define MNT_RF_BW_MON_BW2_PREV_W	0xA0	//EM
#define MNT_RF_BW_MON_BW2_PREV_R	0xA4


// MTOP
#define MNT_MTOP_BW_MON_BW0_PREV_W	0x10	//LM
#define MNT_MTOP_BW_MON_BW0_PREV_R	0x14
#define MNT_MTOP_BW_MON_BW1_PREV_W	0x20	//LM
#define MNT_MTOP_BW_MON_BW1_PREV_R	0x24
#define MNT_MTOP_BW_MON_BW2_PREV_W	0x30	//LM
#define MNT_MTOP_BW_MON_BW2_PREV_R	0x34



#define BASE_PREV_W MNT_RF_BW_MON_BW0_PREV_W
#define BASE_PREV_R MNT_RF_BW_MON_BW0_PREV_R
#define BASE_MTOP_PREV_W MNT_MTOP_BW_MON_BW0_PREV_W
#define BASE_MTOP_PREV_R MNT_MTOP_BW_MON_BW0_PREV_R


/*
Pending Monitor
*/
#define MNT_BTOP_PENDING_MAX	0xB8
#define MNT_CTOP_PENDING_MAX	MNT_BTOP_PENDING_MAX
#define MNT_BTOP_CCI_PENDING_MAX	MNT_BTOP_PENDING_MAX
#define MNT_MTOP_PENDING_MAX	0x08


/*
Monitor Latency offset
*/
#define MNT_RD_LATENCY_PEAK		0xCC
#define MNT_RD_LATENCY_CMD		0xDC
#define MNT_RD_LATENCY_SUM0		0xD4
#define MNT_RD_LATENCY_SUM1		0xD8
#define MNT_WR_LATENCY_PEAK		0xEC
#define MNT_WR_LATENCY_CMD	 	0xFC
#define MNT_WR_LATENCY_SUM0		0xF4
#define MNT_WR_LATENCY_SUM1		0xF8


/*
BW limiter offset
*/
#define MNT_BW_LIMITER_WINDOW	0xB0
#define MNT_BW_LIMITER_WR		0x40
#define MNT_BW_LIMITER_RD		0x44


/*
; MNT_INTR_CTRL Register field value
*/
#define GIC400_USE
#ifdef GIC400_USE
//nalmnalm
#define H15_PERIPH_BASE			0xC2000000
#define LG1210_GIC_DIST_BASE		H15_PERIPH_BASE + 0x1000
#define LG1210_GIC_CPU_BASE			H15_PERIPH_BASE + 0x2000


 /*
 * Description:
 * Reserved address space of the GIC controller (size expressed in 32 bit words)
 */
#define GIC_RESERVED_BEFORE_ENABLE_SET          30
#define GIC_RESERVED_BEFORE_PRIORITY            32
#define GIC_RESERVED_BEFORE_IDENTIFICATION      55
#define GIC_IMPLEMENTATION_DEFINED_DISTR        64
#define GIC_RESERVED_BEFORE_SOFT_INT            128
#endif

/*
; MNT_INTR_CTRL Register field value
*/
#define WR_BW_INTR_EN				(0x1<<0)
#define WR_LATENCY_INTR_EN			(0x1<<1)
#define RD_BW_INTR_EN				(0x1<<2)
#define RD_LATENCY_INTR_EN			(0x1<<3)
#define TMR_DONE_INTR_EN			(0x1<<4)

#define BLK_INTR_EN					(WR_BW_INTR_EN|WR_LATENCY_INTR_EN|RD_BW_INTR_EN|RD_LATENCY_INTR_EN)

#define WR_BW_INTR_STATUS			(0x1<<8)
#define WR_LATENCY_INTR_STATUS		(0x1<<9)
#define RD_BW_INTR_STATUS			(0x1<<10)
#define RD_LATENCY_INTR_STATUS		(0x1<<11)
#define TMR_DONE_INTR_STATUS		(0x1<<12)

#define WR_BW_INTR_CLR				(0x1<<16)
#define WR_LATENCY_INTR_CLR			(0x1<<17)
#define RD_BW_INTR_CLR				(0x1<<18)
#define RD_LATENCY_INTR_CLR			(0x1<<19)
#define TMR_DONE_INTR_CLR			(0x1<<20)
#define TMR_START					(0x1<<24)
#define TMR_AUTO_START 				(0x1<<25)

#define ABNORMAL_INTR_MASK			(WR_BW_INTR_STATUS \
								 	|WR_LATENCY_INTR_STATUS \
								 	|RD_BW_INTR_STATUS \
								 	|RD_LATENCY_INTR_STATUS )


// DISPLAY MASK
#define RAWDATA_EN               (0x1<<0)
#define LATENCY_EN          (0x1<<1)
#define PORTNUM_EN               (0x1<<2)
#define DE_EN               (0x1<<3)
#define DISPLAY_MASK			(RAWDATA_EN |LATENCY_EN |PORTNUM_EN|DE_EN)

/*
 PORT STATUS
*/
#define PICK_RD_EN_BIT(value,m)		(value >> 16+m) & 0b1
#define PICK_WR_EN_BIT(value,m)		(value >> m) & 0b1
#define PICK_PORT_NUM(value,m)		(value >> (4+m*8)) & 0b11

/*
; Interrupt information for lgbus
*/
#define IRQ_GIC_START   		32

#define IRQ_GIC_START   		32
#define USE_GENERIC_32BIT_TIMER

#ifdef	H15_IRQ_TIMER_32
#undef	H15_IRQ_TIMER_32
#define H15_IRQ_TIMER_32	    6
#endif

#ifdef H15_IRQ_CCI400_TIMER
#undef H15_IRQ_CCI400_TIMER
#define H15_IRQ_CCI400_TIMER    89
#endif

#ifdef	H15_IRQ_LBUS_TIMER
#undef	H15_IRQ_LBUS_TIMER
#define H15_IRQ_LBUS_TIMER      90
#endif

/*
;0 ~ 15 : SGI(Soft Generate Interrupt), 16 ~ 31 : PPI(Private Peripheral Interrupt), 32 ~ : SPI(Shared Programmable Interrupt)
*/
#define IRQ_TIMER						(H15_IRQ_TIMER_32 + IRQ_GIC_START)
#define IRQ_CCI_TIMER					(H15_IRQ_CCI400_TIMER + IRQ_GIC_START)
#define IRQ_LBUS_TIMER					(H15_IRQ_LBUS_TIMER + IRQ_GIC_START)
#define IRQ_GBUS_TIMER					(H15_IRQ_GBUS_TIMER + IRQ_GIC_START)
#define IRQ_EBUS_TIMER					(H15_IRQ_EBUS_TIMER + IRQ_GIC_START)



#define PICK_BIT(value,offset)		(value >> offset) & 0x1
#define M0_ENABLED(value)			PICK_BIT(value,M0_PORT_ENABLE_BIT)
#define M1_ENABLED(value)			PICK_BIT(value,M1_PORT_ENABLE_BIT)
#define M2_ENABLED(value)			PICK_BIT(value,M2_PORT_ENABLE_BIT)



/*
;0 ~ 15 : SGI(Soft Generate Interrupt), 16 ~ 31 : PPI(Private Peripheral Interrupt), 32 ~ : SPI(Shared Programmable Interrupt)
*/
#define IRQ_TIMER						(H15_IRQ_TIMER_32 + IRQ_GIC_START)
#define IRQ_CCI_TIMER					(H15_IRQ_CCI400_TIMER + IRQ_GIC_START)
#define IRQ_LBUS_TIMER					(H15_IRQ_LBUS_TIMER + IRQ_GIC_START)
#define IRQ_GBUS_TIMER					(H15_IRQ_GBUS_TIMER + IRQ_GIC_START)
#define IRQ_EBUS_TIMER					(H15_IRQ_EBUS_TIMER + IRQ_GIC_START)

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*
; LG BUS monitoring structure.
*/

typedef enum {
	MNT_GPU0_IDX,
	MNT_GFX0_IDX,
	MNT_TE_IDX,
	MNT_ICOD_IDX,
	MNT_VENC0_IDX,
	MNT_AUD_IDX,
	MNT_VIP_IDX,
	MNT_GPU1_IDX,
	MNT_GFX1_IDX,
	MNT_VDEC0_M0_IDX,
	MNT_VDEC0_M1_IDX,
	MNT_VDEC1_M0_IDX,
	MNT_VDEC2_M0_IDX,
	MNT_OSD0_IDX,
	MNT_VDEC1_M1_IDX,
	MNT_VDEC2_M1_IDX,
	MNT_OSD1_IDX,
	MNT_VDM0_IDX,
	MNT_VDM1_IDX,
	MNT_SMX0_IDX,
	MNT_SMX1_IDX,
	MNT_MCU_IDX,
	MNT_CVD_IDX,
	MNT_ND0_IDX,
	MNT_ND1_IDX,
	MNT_GSC0_IDX,
	MNT_GSC1_IDX,
	MNT_VCP0_IDX,
	MNT_VCP1_IDX,
	MNT_SUB_IDX,
	MNT_BTOP_MAX_IDX,

	LGBUS_MAX_BLK = MNT_BTOP_MAX_IDX,
}
btop_idx_e, LGBUS_H15_BLK_T;

/*typedef enum {
	MNT_LTN_WPEAK_IDX,
	MNT_LTN_RPEAK_IDX,
	MNT_LTN_WAVG_IDX,
	MNT_LTN_RAVG_IDX,
	MNT_LTN_MAX_IDX
}	ltn_idx_e;
*/
//jaehyun add
typedef enum {
	MNT_CCI_GPU_IDX,
	MNT_CCI_PERI_IDX,
	MNT_CCI_CPU_IDX,
	MNT_CCI_MAX_IDX
}	cci_idx_e;

typedef enum {
	MNT_M00_IDX,
	MNT_M01_IDX,
	MNT_M02_IDX,
	MNT_M03_IDX,
	MNT_MTOP_MAX_IDX
}	mtop_idx_e;

typedef enum {
	MNT_CCI_M00_IDX,
	MNT_CCI_M01_IDX,
	MNT_CCI_M02_IDX,
	MNT_CCI_MTOP_MAX_IDX
}	mtop_cci_idx_e;

typedef enum {  //nalm
	L_QOS_IDX,
	G_QOS_IDX,
	E_QOS_IDX,
	CCI_QOS_IDX,
	QOS_MAX_IDX
} qos_idx_e;

typedef enum {  //nalm
	L_MEM_IDX,
	G_MEM_IDX,
	E_MEM_IDX,
	MEM_MAX_IDX,
	LGBUS_MAX_DDR_BLK = MEM_MAX_IDX,
} mem_idx_e, LGBUS_H15_DDR_BLK_T;

typedef enum {  //nalm
	PORT0_IDX,
	PORT1_IDX,
	PORT2_IDX,
	PORT3_IDX,
	PORT_MAX_IDX
} port_idx_e;

typedef struct {
	int (*init)(void);
	void (*read)(void);
	void (*port)(void);
	void (*stop)(void);
	int (*isr)(void);
} lgbus_ops_t;


typedef enum {
	MNT_WR_BW_IDX,
	MNT_RD_BW_IDX
} bus_mon_raw_idx_e;


typedef struct {
	long long int wr_avg_ltn;
	long long int rd_avg_ltn;
} btop_raw_ltn_avg_t;

typedef struct { //add jaehyun for ltn
	unsigned int	wr_peak_ltn;
	unsigned int	wr_cmd_ltn;
	unsigned int	wr_sum0_ltn;
	unsigned int	wr_sum1_ltn;
	unsigned int	rd_peak_ltn;
	unsigned int 	rd_cmd_ltn;
	unsigned int 	rd_sum0_ltn;
	unsigned int	rd_sum1_ltn;
} btop_ltn_raw_t;

typedef struct {
	unsigned int 	wr_bw;
	unsigned int 	rd_bw;
} btop_raw_t;

typedef struct {
	btop_raw_t		raw[MNT_BTOP_MAX_IDX];			//btop
	btop_raw_t		raw_cci[MNT_CCI_MAX_IDX];		//ctop = cci_btop
	double wr_total_bw;
	double rd_total_bw;
} domain_result_t;

typedef struct {
	domain_result_t		result[MEM_MAX_IDX];
	btop_ltn_raw_t 		ltn_raw[MNT_BTOP_MAX_IDX];
	btop_ltn_raw_t 		ltn_raw_cci[MNT_CCI_MAX_IDX];
	btop_raw_t		praw[MNT_BTOP_MAX_IDX];				//btop temp debug
	btop_raw_t		praw_cci[MNT_CCI_MAX_IDX];		//ctop = cci_btop temp debug
} btop_reply_t;







//DE_PATH, VCP, OSD
typedef struct {
	unsigned int	bw;
} status_raw_t;




typedef struct {
	unsigned int	wr_bw;
	unsigned int	rd_bw;
} mtop_raw_t;

typedef struct { //jaehyun
	mtop_raw_t		raw[MNT_MTOP_MAX_IDX];				//mtop
	mtop_raw_t		raw_cci[MNT_CCI_MTOP_MAX_IDX];		//cci_mtop
	mtop_raw_t		to_bus[MEM_MAX_IDX];//add
	mtop_raw_t		praw[MNT_MTOP_MAX_IDX];				//mtop temp debug
	double wr_total_bw;
	double rd_total_bw;
} domain_mtop_result_t;

typedef struct { //jaehyun
	domain_mtop_result_t	result[MEM_MAX_IDX];
	mtop_raw_t			praw_cci[MNT_CCI_MTOP_MAX_IDX];		//cci_mtop temp debug
} mtop_reply_t;

typedef struct {
	btop_reply_t btop;
	mtop_reply_t mtop;
	status_raw_t 	de[21];		//DE_PATH
	status_raw_t 	vcp[4];		//VCP
	status_raw_t 	osd[8];		//OSD
} bw_reply_t;


typedef struct {
	double wr_avg_ltn;
	double rd_avg_ltn;
} btop_avg_ltn_avg_t;


typedef struct {
	double	wr_peak_ltn;
	double	wr_cmd_ltn;
	double	wr_sum0_ltn;
	double	wr_sum1_ltn;
	double	rd_peak_ltn;
	double 	rd_cmd_ltn;
	double 	rd_sum0_ltn;
	double	rd_sum1_ltn;
} btop_avg_ltn_t;



typedef struct {
	double 	wr_bw;
	double 	rd_bw;
	double 	rw_bw;
} btop_avg_raw_t;

typedef struct {
	btop_avg_raw_t		raw[MNT_BTOP_MAX_IDX];
	btop_avg_raw_t		raw_cci[MNT_CCI_MAX_IDX];
	btop_avg_raw_t		raw_total;
} domain_btop_avg_t;

typedef struct {
	domain_btop_avg_t	result[MEM_MAX_IDX];
	btop_avg_ltn_t 		ltn_raw[MNT_BTOP_MAX_IDX];
	btop_avg_ltn_t 		ltn_raw_cci[MNT_CCI_MAX_IDX];
} btop_avg_t;


typedef struct {
	double	wr_bw;
	double	rd_bw;
	double	rw_bw;
} mtop_avg_raw_t;

typedef struct { //jaehyun
	mtop_avg_raw_t		raw[MNT_MTOP_MAX_IDX];
	mtop_avg_raw_t		raw_cci[MNT_CCI_MTOP_MAX_IDX];//add
	mtop_avg_raw_t		raw_total;
} domain_mtop_avg_t;

typedef struct { //jaehyun
	domain_mtop_avg_t	result[MEM_MAX_IDX];
} mtop_avg_t;

typedef struct {
	btop_avg_t btop_avg;
	mtop_avg_t mtop_avg;
} bw_avg_t;



typedef struct {
	long long int wr_avg_ltn;
	long long int rd_avg_ltn;
} btop_max_ltn_avg_t;

typedef struct {
	unsigned int	wr_peak_ltn;
	unsigned int	wr_cmd_ltn;
	unsigned int	wr_sum0_ltn;
	unsigned int	wr_sum1_ltn;
	unsigned int	rd_peak_ltn;
	unsigned int 	rd_cmd_ltn;
	unsigned int 	rd_sum0_ltn;
	unsigned int	rd_sum1_ltn;
} btop_max_ltn_t;

typedef struct {
	unsigned int 	wr_bw;
	unsigned int 	rd_bw;
	unsigned int 	rw_bw;
} btop_max_raw_t;

typedef struct {
	btop_max_raw_t		raw[MNT_BTOP_MAX_IDX];
	btop_max_raw_t		raw_cci[MNT_CCI_MAX_IDX];
	btop_max_raw_t		raw_total;
} domain_btop_max_t;

typedef struct {
	domain_btop_max_t	result[MEM_MAX_IDX];
	btop_max_ltn_t 		ltn_raw[MNT_BTOP_MAX_IDX];
	btop_max_ltn_t 		ltn_raw_cci[MNT_CCI_MAX_IDX];
} btop_max_t;


typedef struct {
	unsigned int	wr_bw;
	unsigned int	rd_bw;
	double		 	rw_bw;
} mtop_max_raw_t;

typedef struct { //jaehyun
	mtop_max_raw_t		raw[MNT_MTOP_MAX_IDX];
	mtop_max_raw_t		raw_cci[MNT_CCI_MTOP_MAX_IDX];//add
	mtop_max_raw_t		raw_total;
} domain_mtop_max_t;

typedef struct { //jaehyun
	domain_mtop_max_t	result[MEM_MAX_IDX];
} mtop_max_t;

typedef struct {
	btop_max_t btop_max;
	mtop_max_t mtop_max;
} bw_max_t;




typedef struct {
	unsigned int	wr_enable;
	unsigned int	wr_num;
	unsigned int	rd_enable;
	unsigned int	rd_num;
} port_raw_t;

typedef struct {
	port_raw_t raw[MNT_BTOP_MAX_IDX];
} port_t;

typedef struct {
	unsigned char wr_num[MEM_MAX_IDX];
	unsigned char rd_num[MEM_MAX_IDX];
} port_raw_num_t;

#if 0
port_raw_num_t portEnTbl[MNT_BTOP_MAX_IDX];
#endif

/*=========================================================================================*
| Global Variables
*==========================================================================================*/
#if 0
char cci_blk_name[MNT_CCI_MAX_IDX][12] =  //add jaehyun
{
	"gpu",
  	"peri",
	"cpu"
};

char from_bus_blk_name[MEM_MAX_IDX][12] =  //add jaehyun
{
	"From L",
	"From G",
	"From E"
};

char mtop_blk_name[MNT_MTOP_MAX_IDX][12] = //add jaehyun
{
	"port0",
	"port1",
	"port2",
	"port3"
};

char btop_blk_name[MNT_BTOP_MAX_IDX][12] =
{
	"gpu0",
  	"gfx0",
	"te",
	"icod",
	"venc0",
	"aud",
	"vip",
	"gpu1",
	"gfx1",
	"vdec0_m0",
	"vdec0_m1",
	"vdec1_m0",
	"vdec2_m0",
	"osd0",
	"vdec1_m1",
	"vdec2_m1",
	"osd1",
	"vdm0",
	"vdm1",
	"smx0",
	"smx1",
	"mcu",
	"cvd",
	"nd0",
	"nd1",
	"gsc0",
	"gsc1",
	"vcp0",
	"vcp1",
	"sub"
};

int port_default_num[MNT_BTOP_MAX_IDX] =
{
	1, //"gpu0"
  	2, //"gfx0",
	0, //"te",
	0, //"icod",
	3, //"venc0",
	0, //"aud",
	3, //"vip",
	1, //"gpu1",
	0, //"gfx1",
	2, //"vdec0_m0",
	3, //"vdec0_m1",
	2, //"vdec1_m0",
	3, //"vdec2_m0",
	2, //"osd0",
	1, //"vdec1_m1",
	2, //"vdec2_m1",
	3, //"osd1",
	2, //"vdm0",
	1, //"vdm1",
	1, //"smx0",
	2, //"smx1",
	0, //"mcu",
	0, //"cvd",
	2, //"nd0",
	1, //"nd1",
	3, //"gsc0",
	0, //"gsc1",
	1, //"vcp0",
	2, //"vcp1",
	3 //"sub"
};

#define NUM_OF_LMASTERS	7
#define NUM_OF_GMASTERS	7
#define NUM_OF_EMASTERS	16

unsigned int cci_addr[MNT_CCI_MAX_IDX] = //add jaehyun
{
	0xC8C00000,
	0xC8C00100,
	0xC8C00200
};

unsigned int btop_addr[MNT_BTOP_MAX_IDX] =
{
	0xc880A000,
	0xc880A200,
	0xc880A400,
	0xc880A600,
	0xc880A800,
	0xc880AA00,
	0xc880AC00,

	0xc8455000,
	0xc8455200,
	0xc8455400,
	0xc8455600,
	0xc8455800,
	0xc8455A00,
	0xc8455C00,

	0xc8213000,
	0xc8213200,
	0xc8213400,
	0xc8213600,
	0xc8213800,
	0xc8213A00,
	0xc8213C00,
	0xc8213E00,
	0xc8214000,
	0xc8214200,
	0xc8214400,
	0xc8214600,
	0xc8214800,
	0xc8214A00,
	0xc8214C00,
	0xc8214E00
};
#endif

/*----------------------------------------------------------------------------------------
    Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __LGBUS_H15_H__ */

