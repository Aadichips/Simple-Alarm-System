;******************************************************************************
; Motion sensor (PIR) simple read in assembly
; PIR on PC4. Returns non-zero if motion detected, else 0.
;******************************************************************************

        .cdecls "motion.h"
        .text

; Keep a copy of the base address like the buzzer example
_GPIO_PORTC_BASE     .field  GPIO_PORTC_BASE

        .global MotionReadAsm
        .asmfunc
; uint32_t MotionReadAsm(void);
MotionReadAsm

        PUSH    {LR}

        ; r0 = base, r1 = mask (PC4)
        LDR     r0, _GPIO_PORTC_BASE
        MOV     r1, #GPIO_PIN_4

        ; call TivaWare GPIOPinRead(base, mask)
        BL      GPIOPinRead

        ; r0 already has the return value (non-zero if PC4 high)
        POP     {PC}
        .endasmfunc
