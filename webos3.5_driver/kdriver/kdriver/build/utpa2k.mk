
CFG_2K_DEFS += -D'CHIP_$(CHIP_NAME)'
CFG_2K_DEFS += -D'MSOS_TYPE_LINUX_KERNEL'

ifeq ($(MCU_TYPE),arm9)
CFG_2K_DEFS += -D'MCU_ARM_9'
endif

ifeq ($(MCU_TYPE),arm_ca12)
CFG_2K_DEFS += -D'MCU_ARM_CA12'
endif

ifeq ($(MCU_TYPE),arm_ca53)
CFG_2K_DEFS += -D'MCU_ARM_CA53'
endif

ifeq ($(FW_TYPE),external)
CFG_2K_DEFS += -D'FW_EXTERNAL_BIN'
endif
ifeq ($(FW_TYPE),embedded)
CFG_2K_DEFS += -D'FW_EMBEDDED_ASC'
endif

CFG_2K_DEFS += -D'DMX_DEBUG=0'

ifeq ($(DMX_UTOPIA_20_ENABLE),y)
CFG_2K_DEFS += -D'DMX_UTOPIA_20'
CFG_2K_DEFS += -D'TSP_UTOPIA_20'
CFG_2K_DEFS += -D'MMFI_UTOPIA_20'
CFG_2K_DEFS += -D'RASP_UTOPIA_20'
CFG_2K_DEFS += -D'TSO_UTOPIA_20'
endif

CFG_2K_DEFS += -D'VDEC_UTOPIA_2K'
CFG_2K_DEFS += -D'VDEC_UTOPIA_2K_KDRV'

CFG_2K_DEFS += -D'JPEG_UTOPIA_2K'
CFG_2K_DEFS += -D'JPEG_UTOPIA_2K_KDRV'

ifeq ($(STC64_ENABLE),y)
CFG_2K_DEFS += -D'STC64_SUPPORT'
endif
ifeq ($(VQ_ENABLE),y)
CFG_2K_DEFS += -D'VQ_ENABLE'
endif
ifeq ($(FILE_VQ_ENABLE),y)
CFG_2K_DEFS += -D'FIVQ_ENABLE'
endif
ifeq ($(EXT_FILTER_ENABLE),y)
CFG_2K_DEFS += -D'EXT_FILTER_SUPPORT'
endif
ifeq ($(TS2_ENABLE),y)
CFG_2K_DEFS += -D'TS2_IF_SUPPORT'
endif
ifeq ($(TS3_ENABLE),y)
CFG_2K_DEFS += -D'TS3_IF_SUPPORT'
endif
ifeq ($(MMFILEIN_ENABLE),y)
CFG_2K_DEFS += -D'MMFILEIN'
endif
ifeq ($(MMFI3D_ENABLE),y)
CFG_2K_DEFS += -D'MMFI_VD3D'
endif
ifeq ($(MMFI2_EXTRA_AUFIFO_ENABLE),y)
CFG_2K_DEFS += -D'MMFI2_EXTRA_AUFIFO'
endif
ifeq ($(MOBF_ENABLE),y)
CFG_2K_DEFS += -D'MOBF_ENABLE'
endif
ifeq ($(MMFIMOBF_ENABLE),y)
CFG_2K_DEFS += -D'MM_MOBF'
endif
ifeq ($(DMX_DEBUG_TABLE_ENABLE),y)
CFG_2K_DEFS += -D'DEBUG_TABLE_SUPPORT'
endif
ifeq ($(EXTRA_PVR1_FLT_ENABLE),y)
CFG_2K_DEFS += -D'EXT_PVR1_FLT'
endif

ifeq ($(TS2_LIVE_ENABLE),y)
CFG_2K_DEFS += -D'TS2_LIVE_SUPPORT'
endif

ifeq ($(TSO_SUPPORT),y)
CFG_2K_DEFS += -D'TSO_ENABLE'
endif

ifeq ($(TSO_20_SUPPORT),y)
CFG_2K_DEFS += -D'TSO_20_ENABLE'
endif

ifeq ($(TSP3_DULFLT_ENABLE),y)
CFG_2K_DEFS += -D'DUP_PIDFLT'
endif

ifeq ($(DSCMB_CSA_ENABLE),y)
CFG_2K_DEFS += -D'DSCMB_CSA_ENABLE'
endif

ifeq ($(DSCMB_MULTI2_ENABLE),y)
CFG_2K_DEFS += -D'DSCMB_MULTI2_ENABLE'
endif

ifeq ($(USE_HWPCR_ENABLE),y)
    ifeq ($(CHIP),$(filter $(CHIP),$(TSP4_CHIPS)))
    CFG_2K_DEFS += -D'HW_PCRFLT_ENABLE=1'
    else
    CFG_2K_DEFS += -D'HWPCR_ENABLE'
    endif
endif

ifeq ($(GENERAL_DSCMBTYPE_SUPPORT),y)
CFG_2K_DEFS += -D'GENERAL_DSCMB'
endif

ifeq ($(MERGE_STR_SUPPORT),y)
CFG_2K_DEFS += -D'MERGE_STR_SUPPORT'
endif

###VDEC related define###
VDEC_USE_NEW_LAYOUT = nike madison miami edison eiffel einstein einstein3 napoli kaiser monaco munich mustang keres clippers muji
ifeq ($(CHIP), $(filter $(CHIP),$(VDEC_USE_NEW_LAYOUT)))
CFG_2K_DEFS += -DSUPPORT_NEW_MEM_LAYOUT
#CFG_2K_DEFS += -DSUPPORT_NEW_VDEC_FLOW   # Use old layout but with new share mem handshake flow
endif

#ifeq ($(V3_DECODE),true)
CFG_2K_DEFS += -DVDEC3
CFG_2K_DEFS += -DVDEC3_FB
#endif
CFG_2K_DEFS += -DSUPPORT_VDEC_STR=1
CFG_2K_DEFS += -DSUPPORT_DISP_INFO_EXT=1

# Note: Use old layout *CANNOT* enable EVD
VDEC_SUPPORT_EVD = einstein napoli madison monaco munich mustang keres clippers muji
ifeq ($(CHIP), $(filter $(CHIP),$(VDEC_SUPPORT_EVD)))
CFG_2K_DEFS += -DSUPPORT_EVD=1
else
CFG_2K_DEFS += -DSUPPORT_EVD=0
endif

VDEC_SUPPORT_G2VP9 = monaco muji
ifeq ($(CHIP), $(filter $(CHIP),$(VDEC_SUPPORT_G2VP9)))
CFG_2K_DEFS += -DSUPPORT_G2VP9=1
else
CFG_2K_DEFS += -DSUPPORT_G2VP9=0
endif

VDEC_SUPPORT_MSVP9 = munich mustang
ifeq ($(CHIP), $(filter $(CHIP),$(VDEC_SUPPORT_MSVP9)))
CFG_2K_DEFS += -DSUPPORT_MSVP9=1
else
CFG_2K_DEFS += -DSUPPORT_MSVP9=0
endif

###VDEC related define###

LKM2K :=                                                                \
            $(PROJ)														\
			$(ROOTLIB)/utopia_core/linux_kernel                         \
			$(ROOTLIB)/msos/linux_kernel                                \
			$(ROOTLIB)/msos/$(MCU_TYPE)                                 \
			$(ROOTLIB)/drv/mmio                                         \
			$(ROOTLIB)/hal/$(CHIP)										\
			$(ROOTLIB)/hal/$(CHIP)/irq                                  \
			$(ROOTLIB)/hal/$(CHIP)/mmio                                 \
			$(ROOTLIB)/hal/$(CHIP)/msos/linux							\
			$(ROOTLIB)/utopia_adaption/utopia							\

ifeq ($(CONFIG_DRV_AESDMA),m)
LKM2K +=	$(ROOTLIB)/drv/aesdma											\
			$(ROOTLIB)/hal/$(CHIP)/aesdma								\
			$(ROOTLIB)/utopia_adaption/aesdma

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/aesdma 							\
				$(ROOTLIB)/drv/aesdma
endif

ifeq ($(CONFIG_DRV_BDMA),m)
LKM2K +=	$(ROOTLIB)/drv/bdma											\
			$(ROOTLIB)/hal/$(CHIP)/bdma								\
			$(ROOTLIB)/utopia_adaption/bdma

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/bdma 							\
				$(ROOTLIB)/drv/bdma
endif

ifeq ($(CONFIG_DRV_MIU),m)
LKM2K +=	$(ROOTLIB)/drv/miu			 							\
                $(ROOTLIB)/hal/$(CHIP)/miu								\
                $(ROOTLIB)/utopia_adaption/miu

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/miu 							\
               $(ROOTLIB)/drv/miu
endif

ifeq ($(CONFIG_DRV_VE),m)
LKM2K +=	$(ROOTLIB)/drv/ve											\
			$(ROOTLIB)/hal/$(CHIP)/ve								    \
			$(ROOTLIB)/utopia_adaption/ve

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/ve/include 						\
			   $(ROOTLIB)/drv/ve/include                                \
			   $(ROOTLIB)/drv/ve                                        \
			   $(ROOTLIB)/drv/miu
endif

ifeq ($(CONFIG_API_ACE),m)
LKM2K +=	$(ROOTLIB)/api/ace										    \
            $(ROOTLIB)/drv/ace										    \
			$(ROOTLIB)/hal/$(CHIP)/ace								    \
			$(ROOTLIB)/utopia_adaption/ace

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/ace/include 						\
			   $(ROOTLIB)/api/ace                                       \
			   $(ROOTLIB)/drv/ace/include                               \
			   $(ROOTLIB)/drv/ace
endif

ifeq ($(CONFIG_API_XC),m)
CFG_2K_DEFS    += -D'UTOPIA_V2'
CFG_2K_DEFS    += -D'TEST_NEW_DYNAMIC_NR=0'
CFG_2K_DEFS    += -D'FPGA=0'
LKM2K +=	$(ROOTLIB)/api/xc											\
            $(ROOTLIB)/drv/xc                                           \
			$(ROOTLIB)/hal/$(CHIP)/xc								    \
			$(ROOTLIB)/utopia_adaption/xc

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/xc/include 						\
			   $(ROOTLIB)/drv/xc/include                                \
			   $(ROOTLIB)/api/xc
endif

ifeq ($(CONFIG_API_MFE_EX),m)
LKM2K +=	$(ROOTLIB)/api/mfe_ex											\
			$(ROOTLIB)/drv/mfe_ex											\
			$(ROOTLIB)/drv/mfe_ex/cModel									\
			$(ROOTLIB)/hal/$(CHIP)/mfe_ex									\
			$(ROOTLIB)/utopia_adaption/mfe_ex

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/mfe_ex								\
			$(ROOTLIB)/api/mfe_ex											\
            $(ROOTLIB)/drv/mfe_ex/cModel									\
			$(ROOTLIB)/drv/mfe_ex
endif

ifeq ($(CONFIG_API_PNL),m)

LKM2K +=	$(ROOTLIB)/api/pnl											\
            $(ROOTLIB)/drv/pnl                                          \
			$(ROOTLIB)/hal/$(CHIP)/pnl								    \
			$(ROOTLIB)/utopia_adaption/pnl

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/pnl       						\
			   $(ROOTLIB)/drv/pnl/include                               \
			   $(ROOTLIB)/drv/pnl/                                      \
			   $(ROOTLIB)/api/pnl
endif

ifeq ($(CONFIG_DRV_AUDIO),m)
LKM2K +=	$(ROOTLIB)/api/audio											\
            $(ROOTLIB)/drv/audio 									    \
            $(ROOTLIB)/drv/audsp 									    \
			$(ROOTLIB)/hal/$(CHIP)/audio  								\
			$(ROOTLIB)/hal/$(CHIP)/audsp  								\
			$(ROOTLIB)/utopia_adaption/audio

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/audio    							\
               $(ROOTLIB)/hal/$(CHIP)/audsp    							\
               $(ROOTLIB)/drv/audio                                       \
               $(ROOTLIB)/drv/audsp                                       \
			   $(ROOTLIB)/api/audio
endif

# **********************************************
# DMX LIB PATH SETTING START
# **********************************************
TSP3_CHIPS = eiffel monaco miami clippers muji munich mustang
TSP4_CHIPS = kaiser keltic keres
FQ_CHIPS = kappa keltic keres monaco muji
TSO_CHIP = napoli kaiser keres
TSO2_CHIP = monaco miami muji munich mustang

TSP_PATH = $(ROOTLIB)/hal/$(CHIP)/tsp

ifeq ($(CHIP),$(filter $(CHIP),$(TSP3_CHIPS)))
    TSP_PATH +=     $(ROOTLIB)/drv/tsp3
    DMX_OPT_PATH = $(TSP_PATH)                                          \
                   $(ROOTLIB)/drv/mmfi2
else ifeq ($(CHIP),$(filter $(CHIP),$(TSP4_CHIPS)))
    TSP_PATH +=     $(ROOTLIB)/drv/tsp4
    DMX_OPT_PATH = $(TSP_PATH)                                          \
                   $(ROOTLIB)/drv/mmfi
else
    TSP_PATH +=     $(ROOTLIB)/drv/tsp
    DMX_OPT_PATH = $(TSP_PATH)                                          \
                   $(ROOTLIB)/drv/mmfi
endif

ifeq ($(CHIP),$(filter $(CHIP),$(TSO_CHIP)))
DMX_OPT_PATH +=    $(ROOTLIB)/drv/tso                                   \
                   $(ROOTLIB)/hal/$(CHIP)/tso
endif

ifeq ($(CHIP),$(filter $(CHIP),$(TSO2_CHIP)))
DMX_OPT_PATH +=    $(ROOTLIB)/drv/tso2                                  \
                   $(ROOTLIB)/hal/$(CHIP)/tso
endif

ifeq ($(CHIP),$(filter $(CHIP),$(FQ_CHIPS)))
DMX_OPT_PATH +=    $(ROOTLIB)/drv/fq									\
                   $(ROOTLIB)/hal/$(CHIP)/fq
endif

ifeq ($(CONFIG_API_DMX),m)
LKM2K +=	$(ROOTLIB)/api/dmx											\
			$(ROOTLIB)/hal/$(CHIP)/mmfi  								\
			   $(ROOTLIB)/utopia_adaption/dmx			                \
               $(DMX_OPT_PATH)

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/mmfi    							\
			   $(ROOTLIB)/api/dmx                                       \
               $(DMX_OPT_PATH)
endif

ifeq ($(CONFIG_DRV_DSCMB),m)
LKM2K +=       $(ROOTLIB)/drv/dscmb											\
                        $(ROOTLIB)/hal/$(CHIP)/dscmb								\
                        $(ROOTLIB)/utopia_adaption/dscmb            \
               $(TSP_PATH)

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/dscmb 							\
                        $(ROOTLIB)/drv/dscmb                                                    \
               $(TSP_PATH)
endif

# **********************************************
# DMX LIB PATH SETTING END
# **********************************************

ifeq ($(CONFIG_DRV_HWI2C),m)
LKM2K +=	$(ROOTLIB)/drv/hwi2c											\
			$(ROOTLIB)/hal/$(CHIP)/hwi2c								\
			$(ROOTLIB)/utopia_adaption/hwi2c

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/hwi2c 							\
				$(ROOTLIB)/drv/hwi2c
endif

ifeq ($(CONFIG_DRV_GPIO),m)
LKM2K +=	$(ROOTLIB)/drv/gpio											\
			$(ROOTLIB)/hal/$(CHIP)/gpio    							\
			$(ROOTLIB)/utopia_adaption/gpio

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/gpio 							\
				$(ROOTLIB)/drv/gpio
endif

ifeq ($(CONFIG_DRV_PM),m)
LKM2K +=	$(ROOTLIB)/drv/pm											\
			$(ROOTLIB)/hal/$(CHIP)/pm    							\
			$(ROOTLIB)/utopia_adaption/pm

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/pm							\
				$(ROOTLIB)/drv/pm
endif

ifeq ($(CONFIG_DRV_WDT),m)
LKM2K +=	$(ROOTLIB)/drv/wdt											\
			$(ROOTLIB)/hal/$(CHIP)/wdt    							\
			$(ROOTLIB)/utopia_adaption/wdt

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/wdt 							\
				$(ROOTLIB)/drv/wdt
endif

ifeq ($(CONFIG_DRV_RTC),m)
LKM2K +=	$(ROOTLIB)/drv/rtc											\
			$(ROOTLIB)/hal/$(CHIP)/rtc    							\
			$(ROOTLIB)/utopia_adaption/rtc

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/rtc 							\
				$(ROOTLIB)/drv/rtc
endif

ifeq ($(CONFIG_API_GOP),m)
LKM2K +=    $(ROOTLIB)/api/gop 				\
            $(ROOTLIB)/drv/gop            		\
            $(ROOTLIB)/hal/$(CHIP)/gop                  \
            $(ROOTLIB)/utopia_adaption/gop

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/gop            	\
               $(ROOTLIB)/drv/gop                     	\
               $(ROOTLIB)/api/gop
endif

ifeq ($(CONFIG_API_GFX),m)
LKM2K +=    $(ROOTLIB)/api/gfx                          \
            $(ROOTLIB)/drv/ge                          \
            $(ROOTLIB)/hal/$(CHIP)/ge                  \
            $(ROOTLIB)/utopia_adaption/gfx

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/ge               \
               $(ROOTLIB)/drv/ge                       \
               $(ROOTLIB)/api/gfx
endif

ifeq ($(CONFIG_DRV_FLASH),m)
ifeq ($(CONFIG_DRV_SERIAL_FLASH),y)
LKM2K +=	$(ROOTLIB)/drv/flash/serial		\
			$(ROOTLIB)/hal/$(CHIP)/flash/serial	\
			$(ROOTLIB)/utopia_adaption/spi

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/flash/serial		\
				$(ROOTLIB)/drv/flash/serial
endif
endif

ifeq ($(CONFIG_DRV_SAR),m)
LKM2K +=	$(ROOTLIB)/drv/sar											\
			$(ROOTLIB)/hal/$(CHIP)/sar    							\
			$(ROOTLIB)/utopia_adaption/sar

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/sar							\
				$(ROOTLIB)/drv/sar
endif

ifeq ($(CONFIG_DRV_PWM),m)
LKM2K +=	$(ROOTLIB)/drv/pwm											\
			$(ROOTLIB)/hal/$(CHIP)/pwm    							\
			$(ROOTLIB)/utopia_adaption/pwm

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/pwm							\
				$(ROOTLIB)/drv/pwm
endif

ifeq ($(CONFIG_DRV_CPU),m)
LKM2K +=        $(ROOTLIB)/drv/cpu                      \
                        $(ROOTLIB)/hal/$(CHIP)/cpu      \
                        $(ROOTLIB)/utopia_adaption/cpu

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/cpu               \
                                $(ROOTLIB)/drv/cpu
endif

ifeq ($(CONFIG_DRV_SYS),m)
LKM2K +=        $(ROOTLIB)/drv/sys                      \
                $(ROOTLIB)/hal/$(CHIP)/sys               \


ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/sys               \
               $(ROOTLIB)/drv/sys
endif

ifeq ($(CONFIG_DRV_MSPI),m)
LKM2K +=        $(ROOTLIB)/drv/mspi                      \
                        $(ROOTLIB)/hal/$(CHIP)/mspi      \
                        $(ROOTLIB)/utopia_adaption/mspi

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/mspi               \
                                $(ROOTLIB)/drv/mspi      \
                                $(ROOTLIB)/drv/gpio
endif

ifeq ($(CONFIG_DRV_VBI),m)
LKM2K +=        $(ROOTLIB)/drv/vbi                       \
                        $(ROOTLIB)/hal/$(CHIP)/vbi       \
                        $(ROOTLIB)/utopia_adaption/vbi

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/vbi                \
               $(ROOTLIB)/drv/vbi
endif

ifeq ($(CONFIG_DRV_AVD),m)
LKM2K +=	$(ROOTLIB)/drv/avd											\
			$(ROOTLIB)/hal/$(CHIP)/avd								\
			$(ROOTLIB)/utopia_adaption/avd

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/avd 							\
				$(ROOTLIB)/drv/avd
endif


ifeq ($(CONFIG_API_JPEG_EX),m)
LKM2K +=	$(ROOTLIB)/api/jpeg_ex											\
			$(ROOTLIB)/drv/jpd_ex											\
			$(ROOTLIB)/hal/$(CHIP)/jpd_ex									\
			$(ROOTLIB)/utopia_adaption/jpeg_ex

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/jpd_ex								\
			$(ROOTLIB)/api/jpeg_ex											\
      $(ROOTLIB)/drv/jpd_ex
endif

ifeq ($(CONFIG_API_VDEC_EX),m)
LKM2K +=	$(ROOTLIB)/api/jpeg_ex                  \
			$(ROOTLIB)/drv/jpd_ex                     \
			$(ROOTLIB)/api/vdec_ex                     \
			$(ROOTLIB)/drv/mjpeg_ex											  \
			$(ROOTLIB)/drv/mvd_ex											  \
			$(ROOTLIB)/drv/hvd_ex											  \
			$(ROOTLIB)/drv/ipauth											  \
			$(ROOTLIB)/hal/$(CHIP)/mvd_ex    						\
			$(ROOTLIB)/hal/$(CHIP)/hvd_ex    						\
			$(ROOTLIB)/hal/$(CHIP)/jpd_ex  						\
			$(ROOTLIB)/hal/$(CHIP)/vpu_ex							  \
			$(ROOTLIB)/utopia_adaption/vdec_ex

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/mvd_ex			\
				$(ROOTLIB)/hal/$(CHIP)/hvd_ex							\
				$(ROOTLIB)/hal/$(CHIP)/jpd_ex  				  \
				$(ROOTLIB)/hal/$(CHIP)/vpu_ex							\
				$(ROOTLIB)/hal/$(CHIP)/mvop							\
				$(ROOTLIB)/drv/mvd_ex											\
				$(ROOTLIB)/drv/hvd_ex											\
				$(ROOTLIB)/drv/ipauth											  \
				$(ROOTLIB)/drv/mjpeg_ex									  \
				$(ROOTLIB)/drv/jpd_ex                    \
				$(ROOTLIB)/api/jpeg_ex                  \
				$(ROOTLIB)/api/vdec_ex
endif

ifeq ($(CONFIG_API_VDEC_V3),m)
LKM2K +=	$(ROOTLIB)/api/jpeg_ex                 \
			$(ROOTLIB)/drv/jpd_ex                      \
			$(ROOTLIB)/api/vdec_v3                      \
			$(ROOTLIB)/drv/mjpeg_v3											\
			$(ROOTLIB)/drv/mvd_v3											  \
			$(ROOTLIB)/drv/hvd_v3											  \
			$(ROOTLIB)/drv/ipauth											  \
			$(ROOTLIB)/hal/$(CHIP)/mvd_v3    						\
			$(ROOTLIB)/hal/$(CHIP)/hvd_v3    						\
			$(ROOTLIB)/hal/$(CHIP)/jpd_ex  						\
			$(ROOTLIB)/hal/$(CHIP)/vpu_v3							  \
			$(ROOTLIB)/utopia_adaption/vdec_v3

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/mvd_v3			\
				$(ROOTLIB)/hal/$(CHIP)/hvd_v3							\
				$(ROOTLIB)/hal/$(CHIP)/jpd_ex  				  \
				$(ROOTLIB)/hal/$(CHIP)/vpu_v3							\
				$(ROOTLIB)/hal/$(CHIP)/mvop							  \
				$(ROOTLIB)/drv/mvd_v3											\
				$(ROOTLIB)/drv/hvd_v3											\
				$(ROOTLIB)/drv/ipauth									    \
				$(ROOTLIB)/drv/mjpeg_v3									  \
				$(ROOTLIB)/drv/jpd_ex                    \
				$(ROOTLIB)/api/jpeg_ex                   \
				$(ROOTLIB)/api/vdec_v3
endif

ifeq ($(CONFIG_API_GPD),m)
LKM2K +=	$(ROOTLIB)/api/gpd                  \
			$(ROOTLIB)/drv/gpd                     \
			$(ROOTLIB)/hal/$(CHIP)/gpd    						\
			$(ROOTLIB)/utopia_adaption/gpd

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/gpd			\
				$(ROOTLIB)/drv/gpd											\
				$(ROOTLIB)/api/gpd
endif

ifeq ($(CONFIG_DRV_MVOP),m)
LKM2K +=	$(ROOTLIB)/drv/mvop									\
			$(ROOTLIB)/hal/$(CHIP)/mvop								\
			$(ROOTLIB)/utopia_adaption/mvop

ALL_INC_DIR += $(ROOTLIB)/hal/$(CHIP)/mvop 							\
				$(ROOTLIB)/drv/mvop
endif

#if 1
# for STR
LKM2K +=	$(ROOTLIB)/drv/str

ALL_INC_DIR += $(ROOTLIB)/drv/str
#endif

ALL_INC_DIR	+= $(ROOTLIB)/include

#EXTRA_CFLAGS +=  -mfloat-abi=softfp
EXTRA_CFLAGS += $(CFG_2K_DEFS)
EXTRA_CFLAGS += -D'__STDINT_HEADER'
EXTRA_CFLAGS += -D'__SYS_TYPES_HEADER'
EXTRA_CFLAGS += -D'BUILDTYPE_EXE'
EXTRA_CFLAGS += -D'IN_U4=1'
EXTRA_CFLAGS += -Wno-declaration-after-statement -Wno-unused-variable -Wno-unused-function -Wno-unused-value -Wframe-larger-than=3072
EXTRA_CFLAGS += -mlong-calls


# Kernel path
KERNEL_PATH ?= ../../../kernel/

