/*
 * motion.h: Header file for motion sensor-related functions
 *
 * Lab 5 starter code
 * ECE 266, spring 2026
 *
 * Created by Aadi
 *
 */
#ifndef MOTION_H_
#define MOTION_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/interrupt.h>
#include "event.h"


// PIR on Grove J16 at PC4 (Port C, Pin 4)

// Initialize motion sensor
void MotionInit(void);

// ASM read: returns non-zero if motion
uint32_t MotionReadAsm(void);

// Registers the new callback event that the ISR will schedule
void PIREventRegister(Event* callback_event);

//Port C ISR (registered via GPIOIntRegister in MotionInit)
void MotionSensorISR();

#endif /* MOTION_H_ */
