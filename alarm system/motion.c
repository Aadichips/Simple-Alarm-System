/*
 * motion.c:  file for PIR sensor setup
 *
 * Lab 4
 * ECE 266, spring 2026
 *
 * Created by Aadi
 *
 */

#include "motion.h"

void MotionInit(void)
{
    // Enable Port C peripheral and configure PC4 as input
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
    {
    }

    // Plain digital input is enough for Grove PIR (has onboard conditioning)
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);

}
