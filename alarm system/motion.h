/*
 * motion.h: Header file for motion sensor-related functions
 *
 * Lab 4 starter code
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

// PIR on Grove J16 at PC4 (Port C, Pin 4)
void MotionInit(void);          // Initialize motion sensor

uint32_t MotionReadAsm(void);   // ASM read: returns non-zero if motion

#endif /* MOTION_H_ */
