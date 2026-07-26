/*
 * motion.c:  file for PIR setup
 *
 * Lab 5
 * ECE 266, spring 2026
 *
 * Created by Aadi
 *
 */

#include "motion.h"
#include <assert.h>
#include "launchpad.h"
#include "event.h"

typedef struct
{
    Event *callback_event;          // pointer to Event object
    uint32_t pin_value;             // Converts pin voltage to 0 or 1 from PIR pin
    bool new_input;                 // flag for new input data
} MotionSensorState;

// Global variable
volatile MotionSensorState motion_sensor;

//Initialize the motion sensor with interrupt enabled
void MotionInit(void)
{
    motion_sensor.callback_event = NULL;
    motion_sensor.pin_value = 0;
    motion_sensor.new_input = false;

    // Enable Port C peripheral and configure PC4 as input
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
    {
    }

    // Plain digital input is enough for Grove PIR (has onboard conditioning)
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);

    //new block of code

    // Register the interrupt
    GPIOIntRegister(GPIO_PORTC_BASE, MotionSensorISR);

    // Both edges so detects when on and off
    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_BOTH_EDGES);

    // set interrupt level to 0 (0 is the highest for programmable interrupts)
    IntPrioritySet(INT_GPIOC, 0);

    //enable GPIO port
    IntEnable(INT_GPIOC);

    //Enable PC4 for interrupt
    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_4);

}

void PIREventRegister(Event *callback_event)
{
    assert(EventInitialized(callback_event));
    motion_sensor.callback_event = callback_event;
}

//Function for interrupt
void MotionSensorISR(void)
{

    GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_4);

    // Read Port C.
    uint32_t pin_valueread = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4);

    // Record the push button pin value
    motion_sensor.pin_value = (pin_valueread ? 1 : 0);
    motion_sensor.new_input = true;

    // Schedule callback event
    if (motion_sensor.callback_event != NULL)
    {
        EventSchedule(motion_sensor.callback_event, EventGetCurrentTime());
    }
    // IMPORTANT: Clear interrupt flag
    GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_4);
}
