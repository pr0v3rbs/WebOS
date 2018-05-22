/*-----------------------------------------------------------------------------
    Include Files
------------------------------------------------------------------------------*/
#include <linux/linkage.h>
#include <asm/assembler.h>
#include <asm/delay.h>

#define RIU_BASE   0XFD000000

    .text
    .align 4

//-------------------------------------------------
//Macro: RREG
//-------------------------------------------------
.macro RREG cpu_reg, mstar_reg
LDR     r12, =\mstar_reg
LDR     \cpu_reg, [r12]
.endm

//-------------------------------------------------
//Macro: REG_C2M //write cpu register to mstar register
//-------------------------------------------------
.macro REG_C2M cpu_reg, mstar_reg
LDR     r12, =\mstar_reg
STR     \cpu_reg, [r12]
.endm


//-------------------------------------------------
//Macro: WREG
//-------------------------------------------------
.macro WREG reg, value
    ldr         r12, =\reg
    ldr         r11, =\value
    str         r11, [r12]
.endm

//-------------------------------------------------
//Macro: mov32  loads a 32-bit value into a register without a data access
//-------------------------------------------------
.macro mov32, reg, val
        movw        \reg, #:lower16:\val
        movt        \reg, #:upper16:\val
.endm

//-------------------------------------------------
//Macro: READ_OUT_WRITE_BACK
//-------------------------------------------------
.macro READ_OUT_WRITE_BACK raddr waddr
    mov32 r1, \raddr
    ldr r0, [r1]
    mov32 r1, \waddr
    str r0, [r1]
.endm

//-------------------------------------------------
//Macro: WRITE_OR_2BYTE // do OR operation and write back
//-------------------------------------------------
.macro WRITE_OR_2BYTE   reg_addr  or_val
    mov32  r1, \reg_addr
    add      r1, r1, r1
    add      r1, #RIU_BASE
    ldr        r0, [r1]
    orr        r0, r0,  #\or_val
    str        r0, [r1]
.endm

//-------------------------------------------------
//Macro: WRITE_OR_UP_LOW_2BYTE
// do OR, high 2 byte and low 2 byte
//-------------------------------------------------
.macro WRITE_OR_UP_LOW_2BYTE           reg_addr  or_val_up or_val_low
    mov32  r1, \reg_addr
    add      r1, r1, r1
    add      r1, #RIU_BASE
    ldr         r0, [r1]
    orr         r0, r0,  #\or_val_up
    orr         r0, r0,  #\or_val_low
    str         r0, [r1]
.endm

//-------------------------------------------------
//Macro: WRITE_2BYTE
//-------------------------------------------------
.macro WRITE_2BYTE             reg_addr  val
    mov32  r1, \reg_addr
    add       r1, r1, r1
    add       r1, #RIU_BASE
    movw    r0, #\val
    str         r0, [r1]
.endm

//-------------------------------------------------
//Macro: WRITE_BYTE
//-------------------------------------------------
.macro WRITE_BYTE reg_addr val
    mov32 r1, \reg_addr
    add   r1, r1, r1
    add   r1, #RIU_BASE
    movw  r0, #\val
    strb  r0, [r1]
.endm

@//-------------------------------------------------
@//Macro: __loop_const_udelay
@//-------------------------------------------------
.macro __loop_const_udelay us
        @ us can not be more than 2000
        @ 0 <= r0 <= 0x7fffff06
        mov32 r0, (\us * UDELAY_MULT)
        @ r2: loops_per_jiffy
        mov r1, #-1
        @ max = 0x01ffffff
        add r0, r0, r1, lsr #32-14
        mov r0, r0, lsr #14     @ max = 0x0001ffff
        add r2, r2, r1, lsr #32-10
        mov r2, r2, lsr #10     @ max = 0x00007fff
        mul r0, r2, r0      @ max = 2^32-1
        add r0, r0, r1, lsr #32-6
        movs r0, r0, lsr #6
__loop_delay_\@:
        subs r0, r0, #1
        bhi __loop_delay_\@
.endm

//-------------------------------------------------
//Function: do_self_refresh_mode
//-------------------------------------------------
/* r0: sram_vaddr, r1: loops_per_jiffy */
ENTRY(do_self_refresh_mode)
    mov     r2, r1            @ save loops_per_jiffy to r2

    @//Switch SRAM Access Right to ARM
    RREG    r1, (RIU_BASE + (0x11135E << 1))
    orr          r1, r1, #0x0008
    bic          r1, r1, #0x0010
    REG_C2M     r1, (RIU_BASE + (0x11135E << 1))

    ldr       r10, =self_refresh_setting
    mov     r11, r0
    ldr       r12, =end_self_refresh_setting
    ldr       r9,  =self_refresh_setting
    sub     r12, r12, r9

#if 0
/* move self-refresh settng code to SRAM*/
MEMCPY32_:
    ldr     r9, [r10], #4
    str     r9, [r11], #4
    subs    r12, r12,  #4
    bne     MEMCPY32_

    /* jump to SRAM*/
    mov pc, r0
#endif

self_refresh_setting:
#if 0
    READ_OUT_WRITE_BACK 0XFD221AD8 0xFD007B64
    READ_OUT_WRITE_BACK 0XFD221ADC 0xFD007B6C
    READ_OUT_WRITE_BACK 0XFD2C2CD8 0xFD007B70
    READ_OUT_WRITE_BACK 0XFD2C2CDC 0xFD007B74

    WRITE_2BYTE 0x1012e0 0x0000
    WRITE_2BYTE 0x101246 0xFFFE
    WRITE_2BYTE 0x101266 0xFFFF
    WRITE_2BYTE 0x101286 0xFFFF
    WRITE_2BYTE 0x1012a6 0xFFFF
    WRITE_2BYTE 0x100646 0xFFFE
    WRITE_2BYTE 0x100666 0xFFFF
    WRITE_2BYTE 0x100686 0xFFFF
    WRITE_2BYTE 0x1006a6 0xFFFF
    __loop_const_udelay 100

    WRITE_2BYTE 0x101218 0x0400
    __loop_const_udelay 5
    WRITE_2BYTE 0x101200 0x002F
    __loop_const_udelay 5
    WRITE_2BYTE 0x101200 0x042F
    __loop_const_udelay 5
    WRITE_2BYTE 0x101200 0x052F
    __loop_const_udelay 5
    WRITE_2BYTE 0x101200 0x002F
    __loop_const_udelay 5
    WRITE_2BYTE 0x101200 0x022F
    __loop_const_udelay 5
    WRITE_2BYTE 0x101200 0x032F
    __loop_const_udelay 5
    WRITE_2BYTE 0x101200 0x002F
    __loop_const_udelay 5
    WRITE_2BYTE 0x101206 0x1E2B
    __loop_const_udelay 5
    WRITE_2BYTE 0x101246 0xFFFF
    __loop_const_udelay 5
    WRITE_2BYTE 0x101200 0X202F
    __loop_const_udelay 5
    WRITE_2BYTE 0x100618 0x0400
    __loop_const_udelay 5
    WRITE_2BYTE 0x100600 0x002F
    __loop_const_udelay 5
    WRITE_2BYTE 0x100600 0x042F
    __loop_const_udelay 5
    WRITE_2BYTE 0x100600 0x052F
    __loop_const_udelay 5
    WRITE_2BYTE 0x100600 0x002F
    __loop_const_udelay 5
    WRITE_2BYTE 0x100600 0x022F
    __loop_const_udelay 5
    WRITE_2BYTE 0x100600 0x032F
    __loop_const_udelay 5
    WRITE_2BYTE 0x100600 0x002F
    __loop_const_udelay 5
    WRITE_2BYTE 0x100606 0x1E2B
    __loop_const_udelay 5
    WRITE_2BYTE 0x100646 0xFFFF
    __loop_const_udelay 5
    WRITE_2BYTE 0x100600 0x202F
    __loop_const_udelay 5

    WRITE_OR_2BYTE 0x101202 0XF000
    WRITE_OR_2BYTE 0x100602 0XF000
    WRITE_OR_2BYTE 0x110d00 0X0008
    WRITE_OR_UP_LOW_2BYTE 0x110d00 0x6000 0x0010
    WRITE_OR_2BYTE 0x161600 0X0008
    WRITE_OR_UP_LOW_2BYTE 0x161600 0x6000 0x0010
    WRITE_2BYTE 0x110d54 0xc070
    WRITE_2BYTE 0x161654 0xc070
    WRITE_OR_2BYTE 0x110d7e 0X0001
    WRITE_OR_2BYTE 0x16167e 0X0001
    WRITE_OR_2BYTE 0x110d7e 0X0200
    WRITE_OR_2BYTE 0x16167e 0X0200
#endif
    /* write magic number to tell PM51 power off */
#    WRITE_BYTE 0x0E68 0x5A
    WRITE_2BYTE 0x0E68 0x5A5A

/* waiting for power off */
loop1:
    nop
    b loop1

end_self_refresh_setting:
