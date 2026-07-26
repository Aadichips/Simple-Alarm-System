/*
 * Lab 5 starter code
 * ECE 266, fall 2025
 *
 * lab5_main.c: Implement the tasks for a motion detection alarm.
 *
 * Created by Zhao Zhang
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "launchpad.h"
#include "buzzer.h"
#include "motion.h"
#include "event.h"
#include <driverlib/gpio.h>
#include <driverlib/interrupt.h>

/********************************************************************************
 * Global declarations
 *******************************************************************************/

// The data structure for the system state
typedef struct
{
    // Activated state: if the system is activated or not
    bool activated;

    // Alert state: If the alert is on or off
    bool alerted;

    // Sound state: If the buzzer is turned on to make a sound or not
    // Note: When the system is activated and alerted, the buzzer is turned
    // on and off periodically
    bool buzzer_on;
} SysState;

// The system state
static SysState sys = {
false /* not activated */,
                        false /* not alerted */,
                        false /* buzzer not turned on */
};

// The events
Event buzzer_play_event;
Event check_push_button_event;
Event motion_sensor_event;   // declare event for MotionSensor function

/********************************************************************************
 * Task 1: If the system is activated and alerted, make the buzzer play
 * on/off sound. The buzzer will be repeatedly turned on and off.
 *
 * You may adjust the on and off times to change the sound pattern and intensity.
 *
 * Note: This is a callback function.
 *******************************************************************************/

void BuzzerPlay(Event *event)                   // the scheduled time
{
    uint32_t delay = 500;                       // default checking interval

    if (sys.alerted)
    {
        // Turn buzzer on and off
        if (sys.buzzer_on)
        {
            // The buzzer is on: turn it off
            BuzzerOff();
            sys.buzzer_on = false;
            delay = 2988;                       // off for 2988 ms
        }
        else
        {
            // The buzzer is off: turn it on
            BuzzerOn();
            sys.buzzer_on = true;
            delay = 12;                         // on for 12 ms
        }
    }
    else
    {
        // Make sure the buzzer is turned off
        if (sys.buzzer_on)
        {
            BuzzerOff();
            sys.buzzer_on = false;
        }
    }

    // schedule callback to buzzer play
    EventSchedule(event, event->time + delay);
}

/********************************************************************************
 * Task 2: check the motion sensor
 *
 * Check the motion sensor input. If motion is detected, put the system
 * in the alerted state. Otherwise, put it in the non-alerted state.
 *
 * If motion is detected then system enters alert state.
 * If no motion then system returns to normal state.
 *******************************************************************************/
void MotionSensor(Event *event)
{

    if (sys.activated)
    {
        // Read PIR sensor at PC4
        uint32_t motion = MotionReadAsm();

        if (motion)
        {
            if (!sys.alerted)
            {
                uprintf("%s\n\r", "motion detected");
            }
            sys.alerted = true;
            // Red ON, others OFF
            LedTurnOnOff(true /* red */, false /* blue */, false /* green */);
        }
        else
        {
            if (sys.alerted)
            {
                uprintf("%s\n\r", "motion not detected");
            }
            sys.alerted = false;
            // Green ON to indicate "armed, no motion"
            LedTurnOnOff(false /* red */, false /* blue */, true /* green */);
        }
    }
    else
    {
        // System not activated: LED off, no alert
        sys.alerted = false;
        LedTurnOnOff(false, false, false);
    }

}

/********************************************************************************
 * Task 3: Check push buttons
 *
 * SW1 activates the system
 * SW2 deactivates the system
 *
 * This function is called upon interrupts
 *******************************************************************************/

void CheckPushButton(Event *event)
{

    int code = PushButtonRead();        // read the pushbutton
    switch (code)
    {
    case 1:                     // SW1 is pushed
        if (!sys.activated)
        {
            // Activate the system
            sys.activated = true;
            sys.alerted = false; //changed from true to false (green is now armed)
            LedTurnOnOff( false/* red */, false /* blue */, true /* green */);
        }
        break;

    case 2:                     // SW2 is pushed
        if (sys.activated)
        {
            // De-activate the system and turn off LED
            sys.activated = false;
            sys.alerted = false;
            LedTurnOnOff(false /* red */, false /* blue */, false /* green */);
        }
        break;
    }
}

/********************************************************************************
 * The main function: Print out a message, schedule the first callback event(s),
 * and then run the callback scheduler.
 *******************************************************************************/

int main(void)
{
    // Initialize the Tiva C LaunchPad and 7-seg
    LaunchPadInit();
    BuzzerInit();
    MotionInit();
    //GPIOIntRegister(GPIO_PORTC_BASE, MotionSensorISR);
    uprintf("%s\n\r", "Lab 5: Motion Detection Alarm");

    // Initialize the event objects
    EventInit(&buzzer_play_event, BuzzerPlay);
    EventInit(&check_push_button_event, CheckPushButton);
    EventInit(&motion_sensor_event, MotionSensor);  //initialize motion sensor

    // Register ISR event
    PushButtonEventRegister(&check_push_button_event);
    PIREventRegister(&motion_sensor_event);

    // Schedule time events
    EventSchedule(&buzzer_play_event, 100);

    // Run the callback scheduler
    while (true)
    {

        asm("   wfi");

        // Run callback events
        EventExecute();
    }
}
