;******************************************************************************
; Motion sensor (PIR) simple read in assembly
; PIR on PC4. Returns non-zero if motion detected, else 0.
;******************************************************************************

        .cdecls "motion.h"
        .text

		.ref	motion_sensor
_motion_sensor  .field  motion_sensor

; Keep a copy of the base address like the buzzer example
;_GPIO_PORTC_BASE     .field  GPIO_PORTC_BASE

        .global MotionReadAsm
        .asmfunc

MotionReadAsm
        LDR     r1, _motion_sensor    ; r1 = &motion_sensor

        ; Check new_input (byte at +8)
        LDRB    r2, [r1, #8]
        CMP     r2, #0				;compares r2 with 0
        BEQ     no_input            ; if no new input, just return pin_value

        ; clear new_input -> 0
        MOVS    r3, #0
        STRB    r3, [r1, #8]

        ; r0 = pin_value (word at +4), already 0/1 from ISR
        LDR     r0, [r1, #4]
        BX      lr

no_input
		MOVS	r0, #0	; return 0 if no motion recorded
		BX		lr		; return
		.endasmfunc
