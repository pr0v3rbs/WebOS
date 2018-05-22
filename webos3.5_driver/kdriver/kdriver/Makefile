CHDIR_SHELL := $(SHELL)
UTOPIA_FOLDER := ./utopia
#	make all CONFIG_MSTAR_UTOPIA2K=n USERSPACE_STRIP=1
export JOBS ?= 1
ERROR_STRING = \x1b[31;01mNo More Than 8 Cores\x1b[0m
define examine_core_count
    if [ $(JOBS) -gt 8 ] ; then \
        echo -e "$(ERROR_STRING)" ; \
        exit 1 ; \
    fi
endef

define chdir
    $(eval _D=$(firstword $(1) $(@D)))
    $(info $(MAKE): cd $(_D)) $(eval SHELL = cd $(_D); $(CHDIR_SHELL))
endef

_all: _release

_build u2k_lib:
	@$(call examine_core_count)
	$(call chdir, build)
#	make all CONFIG_MSTAR_UTOPIA2K=n USERSPACE_STRIP=1
	make all CONFIG_MSTAR_UTOPIA2K=n USERSPACE_STRIP=1 -j$(JOBS)
#	make kdrv
#	make bsp
#	mkdir -p ../output/
#	cp ./utpa2k.ko ../output/kdrv_msd93xbg.ko
kdrv: _build
	$(call chdir, build)
	make kdrv CONFIG_MSTAR_UTOPIA2K=n
bsp: kdrv
bsp bsp_only:
	$(call chdir, build)
	make bsp CONFIG_MSTAR_UTOPIA2K=n

build: bsp

_clean:
	$(call chdir, build)
	make clean CONFIG_MSTAR_UTOPIA2K=n

_release: build

all: _all

clean :_clean


_release: build
_release release_bsp:
ifeq ($(KDRV_MODULE_BUILD),)
	$(call chdir, ./)
	mkdir -p output
	cp -av ./build/kdrv_msd93xbg.ko ./output/kdrv_msd93xbg.ko 2>/dev/null || :
	mkdir -p $(UTOPIA_FOLDER)/bin $(UTOPIA_FOLDER)/lib $(UTOPIA_FOLDER)/include
	rm -rf $(UTOPIA_FOLDER)/bin/*
	#cp -rf ./build/bsp/bin/* $(UTOPIA_FOLDER)/bin/
	rm -rf $(UTOPIA_FOLDER)/lib/*
	cp -rf ./build/bsp/utpa2k.a $(UTOPIA_FOLDER)/lib/
	cp -rf ./build/bsp/libutopia.so $(UTOPIA_FOLDER)/lib/
	rm -rf $(UTOPIA_FOLDER)/include/*
	cp -rf ./build/bsp/include/* $(UTOPIA_FOLDER)/include/
	cp -rf ./build/include/generated/autoconf.h $(UTOPIA_FOLDER)/include/
	cp -rf ./mxlib/include/pmlite.dat $(UTOPIA_FOLDER)/include/
endif

