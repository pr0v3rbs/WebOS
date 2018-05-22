
DRV_DIR = \
            demod             \
            audsp                                                   \
            avd                                                     \
            vif                                                     \
            cmdq                                                     \
            bdma                                                    \
            cpu                                                     \
            ddc2bi                                                  \
            urdma		                                    \
            flash/serial                                            \
            flash/nor                                               \
            mpif                                                    \
            ge                                                      \
            gpio                                                    \
            hvd_v3                                                 \
            hwi2c                                                   \
            ipauth                                                  \
            ir                                                      \
            irq                                                     \
            jpd                                                     \
            mbx                                                     \
            miu                                                     \
            mjpeg_v3                                                     \
            mmio                                                    \
            mvd_v3                                                     \
            mvop                                                    \
            nds                                                     \
            nsk2                                                    \
            pcmcia                                                  \
            pwm                                                     \
            pq                                                      \
            rasp                                                    \
            rvd                                                     \
            sar                                                     \
            sc                                                      \
            sem                                                     \
            sys                                                     \
            tsp                                                     \
            tsp2                                                    \
            tsp3                                                    \
            tsp4                                                    \
            uart                                                    \
            vbi                                                     \
            ve                                                      \
            wdt                                                     \
            rtc																											\
            pws                                                     \
            wble                                                    \
            pm       	                                            \
            mfc                                                     \
            nga                                                     \
            dmd                                                     \
            dip                                                     \
            usb_ecos/newhost                                        \
            tso                                                     \
            tso2                                                    \
            mspi                                                    \
            seal                                                    \
            ch34                                                    \
            dvb_extdemod                                            \

API_DIR = \
            ace                                                     \
            dmx                                                     \
            gop                                              	    \
            gfx                                                     \
            cec                                                     \
            dlc                                                     \
            pnl                                                     \
            xc                                                      \
            vdec_v3                                                 \
            audio                                                   \
            swi2c                                                   \
            secure                                                  \
            hsl                                                     \
            gopsc                                                   \
            gopscd                                                  \
            gpd                                                     \
            mhl                                                     \

MMFI_CHIPS = t12 j2 a2 a1 k1 a5 a7 a3 k2 a5p amethyst e3 macaw12 emerald eden euler edison keres
ifeq ($(CHIP), $(filter $(CHIP),$(MMFI_CHIPS)))
DRV_DIR += mmfi
endif

MMFI2_CHIPS = agate einstein eiffel einstein3 keres monaco
ifeq ($(CHIP), $(filter $(CHIP),$(MMFI2_CHIPS)))
DRV_DIR += mmfi2
endif

ifeq ($(OS_TYPE), linux)
MFE_CHIPS = t8 j2 t12 a2 a5 a3 e3 kaiser
ifeq ($(CHIP), $(filter $(CHIP),$(MFE_CHIPS)))
API_DIR += mfe
endif
ifeq ($(PLATFORM), android) # android
API_DIR += mfe_ex
endif
endif

BOX_CHIPS = u3 u4 k1 k2 kaiser keltic keres

ifeq ($(CHIP), $(filter $(CHIP),$(BOX_CHIPS)))
API_DIR +=  hdmitx                                                  \
            hdmitx2                                                  \
            dac                                                     \
            acp                                                     \

endif

ifeq ($(CHIP), agate)
API_DIR +=  hdmitx                                                  \
            hdmitx2                                                  \

endif

NJPD_CHIPS = agate eagle e3 eiffel edison einstein nike madison monet miami einstein3 monaco clippers muji napoli celtics
ifeq ($(CHIP), $(filter $(CHIP),$(NJPD_CHIPS)))
API_DIR += njpeg_ex
else
API_DIR += jpeg_ex
endif

NEW_CA_CHIPS = kaiser keltic keres

ifeq ($(CHIP), $(filter $(CHIP),$(NEW_CA_CHIPS)))
DRV_DIR +=  dscmb2
DRV_DIR +=  cipher
DRV_DIR +=  ca2
else
DRV_DIR += dscmb
DRV_DIR += aesdma
DRV_DIR += ca
endif

# to avoid "make" implicitly becomes "make amazon"
all:

# external demod
amazon:
	@echo [CC] halDMD_EXTERN_ISDBT.c
	@$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) $(DRV_CC_INCS) -o libhalAMAZON.o $(ROOT)/mxlib/hal/amazon/halDMD_EXTERN_ISDBT.c
ifeq ($(LINK_TYPE),dynamic)
	@$(CC) $(CFG_CC_OPTS) $(LD_OPTS) libhalAMAZON.o -s -o libhalAMAZON.$(LIB_EXT)
else
	@$(AR) cru libhalAMAZON.$(LIB_EXT) libhalAMAZON.o
	@$(STRIP) -S libhalAMAZON.$(LIB_EXT)
endif
	@rm -rf libhalAMAZON.o
	@mv libhalAMAZON.$(LIB_EXT) $(DDI_PATH)

galileo:
	@echo [CC] halDMD_EXTERN_ATSC.c
	@$(CC) $(CC_OPTS) $(CC_DEFS) -Dgalileo $(CC_INCS) $(DRV_CC_INCS) -o libhalgalileo.o $(ROOT)/mxlib/hal/galileo/halDMD_EXTERN_ATSC.c
ifeq ($(LINK_TYPE),dynamic)
	@$(CC) $(CFG_CC_OPTS) $(LD_OPTS) libhalgalileo.o -s -o libhalgalileo.$(LIB_EXT)
else
	@$(AR) cru libhalgalileo.$(LIB_EXT) libhalgalileo.o
	@$(STRIP) -S libhalgalileo.$(LIB_EXT)
endif
	@rm -rf libhalgalileo.o
	@mv libhalgalileo.$(LIB_EXT) $(DDI_PATH)

cashmere:
	@echo [CC] halDMD_EXTERN_DTMB.c
	@$(CC) $(CC_OPTS) $(CC_DEFS) -Dcashmere $(CC_INCS) $(DRV_CC_INCS) -o libcashmere.o $(ROOT)/mxlib/hal/cashmere/halDMD_EXTERN_DTMB.c
ifeq ($(LINK_TYPE),dynamic)
	@$(CC) $(CFG_CC_OPTS) $(LD_OPTS) libcashmere.o -s -o libcashmere.$(LIB_EXT)
else
	@$(AR) cru libcashmere.$(LIB_EXT) libcashmere.o
	@$(STRIP) -S libcashmere.$(LIB_EXT)
endif
	@rm -rf libcashmere.o
	@mv libcashmere.$(LIB_EXT) $(DDI_PATH)
