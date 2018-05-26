#----------------------------------------------------------------------------------------
#
# SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
# Copyright(c) 2013 by LG Electronics Inc.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
#----------------------------------------------------------------------------------------

DDK_CFLAGS	:=
DDK_CFLAGS	+= -DCONFIG_LG_EXT
DDK_CFLAGS	+= -DLINUX -DDRIVER -DDBG=1 -DDEBUG -D_DEBUG
DDK_CFLAGS	+= -DUSE_PLATFORM_DRIVER=1
DDK_CFLAGS	+= -DVIVANTE_PROFILER=1
DDK_CFLAGS	+= -DVIVANTE_PROFILER_CONTEXT=1
DDK_CFLAGS	+= -DENABLE_GPU_CLOCK_BY_DRIVER=0
DDK_CFLAGS	+= -DUSE_NEW_LINUX_SIGNAL=0
DDK_CFLAGS	+= -DgcdPAGED_MEMORY_CACHEABLE=0
DDK_CFLAGS	+= -DgcdNONPAGED_MEMORY_CACHEABLE=0
DDK_CFLAGS	+= -DgcdNONPAGED_MEMORY_BUFFERABLE=1
DDK_CFLAGS	+= -DgcdCACHE_FUNCTION_UNIMPLEMENTED=0
DDK_CFLAGS	+= -DgcdENABLE_VG=0
DDK_CFLAGS	+= -DgcdSMP=1
DDK_CFLAGS	+= -DVIVANTE_NO_3D
DDK_CFLAGS	+= -DgcdENABLE_OUTER_CACHE_PATCH=1
DDK_CFLAGS	+= -DgcdFPGA_BUILD=0

DDK_INC_PATH:=
DDK_INC_PATH+= kernel
DDK_INC_PATH+= kernel/arch
DDK_INC_PATH+= kernel/inc
DDK_INC_PATH+= os/linux/kernel

DDK_OBJS	:=
DDK_OBJS	+= os/linux/kernel/gc_hal_kernel_os.o
DDK_OBJS	+= os/linux/kernel/gc_hal_kernel_driver.o
DDK_OBJS	+= os/linux/kernel/gc_hal_kernel_device.o
DDK_OBJS	+= os/linux/kernel/gc_hal_kernel_math.o
DDK_OBJS	+= os/linux/kernel/gc_hal_kernel_linux.o
DDK_OBJS	+= os/linux/kernel/gc_hal_kernel_allocator.o
DDK_OBJS	+= os/linux/kernel/gc_hal_kernel_debugfs.o
DDK_OBJS	+= kernel/gc_hal_kernel_debug.o
DDK_OBJS	+= kernel/gc_hal_kernel_video_memory.o
DDK_OBJS	+= kernel/gc_hal_kernel_heap.o
DDK_OBJS	+= kernel/gc_hal_kernel_event.o
DDK_OBJS	+= kernel/gc_hal_kernel_db.o
DDK_OBJS	+= kernel/gc_hal_kernel_command.o
DDK_OBJS	+= kernel/gc_hal_kernel_mmu.o
DDK_OBJS	+= kernel/gc_hal_kernel.o
DDK_OBJS	+= kernel/arch/gc_hal_kernel_context.o
DDK_OBJS	+= kernel/arch/gc_hal_kernel_hardware.o
DDK_OBJS	+= kernel/gc_hal_kernel_power.o

#DDK_OBJS	+= os/linux/kernel/gc_hal_kernel_sync.o
#DDK_OBJS	+= os/linux/kernel/gc_hal_kernel_security_channel.o
#DDK_OBJS	+= kernel/gc_hal_kernel_security.o

