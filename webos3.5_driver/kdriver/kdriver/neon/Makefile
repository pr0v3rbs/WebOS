# Version: debug or retail
export VERSION      ?= retail

CROSS_COMPILE = arm-none-linux-gnueabi-
AR = $(CROSS_COMPILE)ar
AS = $(CROSS_COMPILE)as
#CC = $(CROSS_COMPILE)gcc

CSRCS = src/mapi_ycs.c
ASRCS = src/asm_yc_separate.s

AOBJS		= $(ASRCS:.s=.o)
COBJS		= $(CSRCS:.c=.o)

OBJS		= $(AOBJS) $(COBJS)


LIBS = libapiYCS.so
INCLUDE = ./ \
          ./inc


ASFLAGS = -march=armv7-a -mfpu=neon -k
CFLAGS = -DMSOS_TYPE_LINUX -fPIC
CFLAGS += $(addprefix -I, $(INCLUDE))

all: $(OBJS)
	@echo "[LD] $(LIBS)"
	@$(CC) -shared -o $(LIBS) $(OBJS)
	cp $(LIBS) ../
          
clean:
	rm -f $(OBJS) $(LIBS)          

%.o: %.s
	@echo "[AS] $^"
	@$(AS) -c $< -o $@ $(ASFLAGS)

%.o: %.c
	@echo "[CC] $^"
	@$(CC) -c $< -o $@ $(CFLAGS)

