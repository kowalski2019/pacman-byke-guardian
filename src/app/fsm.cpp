#include "fsm.h"
#include "freertos/FreeRTOS.h"
//#include "HardwareSerial.h"

/**
 * @brief Function to initialize the state machine
 *
 * @param machine
 */
void initStateMachine(StateMachine *machine)
{
    machine->previous_state = STATE_UNDEFINED;
    machine->current_state = STATE_READY;
    machine->start_time = 0;
}

/**
 * @brief Function to transition to a new state
 *
 * @param machine
 * @param new_state
 */
void transitionState(StateMachine *machine, State new_state)
{
    machine->previous_state = machine->current_state;
    machine->current_state = new_state;
    machine->start_time = portTICK_PERIOD_MS;
}

/**
 * @brief Function to handle events and transitions
 *
 * @param machine
 * @param event
 */
void handleEvent(StateMachine *machine, Event event)
{
    switch (machine->current_state)
    {
    case STATE_UNDEFINED:
        break;
    case STATE_IDLE: // byke is actualy moving
        if (event == EVENT_BIKE_IS_PARKING)
        {
            //Serial.println("Transitioning from Idle to Active");
            transitionState(machine, STATE_ACTIVE);
        }
        break;
    case STATE_READY: // system just started
        if (event == EVENT_BIKE_IS_MOVING)
        {
            //Serial.println("Transitioning from Ready to Idle");
            transitionState(machine, STATE_IDLE);
        }
        else if (event == EVENT_BIKE_IS_PARKING)
        {
            //Serial.println("Transitioning from Ready to Active");
            transitionState(machine, STATE_ACTIVE);
        }
        else if (event == EVENT_TIMEOUT)
        {
        }

        else
        {

            // nothing to do here
        }
        break;

    case STATE_ACTIVE:
        if (event == EVENT_BIKE_IS_MOVING)
        {
            //Serial.println("Transitioning from Active to Idle.");
            transitionState(machine, STATE_IDLE);
        }
        else if (event == EVENT_LOCK_BIKE)
        {
            //Serial.println("Transitioning from Active to Bike locked.");
            transitionState(machine, STATE_BIKE_LOCKED);
        }
        else
        {
            // nothing to do here
        }
        break;
    case STATE_BIKE_LOCKED:
        if (event == EVENT_UNLOCK_BIKE)
        {
            //Serial.println("Transitioning from Bike Locked to Active.");
            transitionState(machine, STATE_ACTIVE);
        }
        else if (event == EVENT_START_ALARM)
        {
            //Serial.println("Transitioning from Bike Locked to Alarm.");
            transitionState(machine, STATE_ALARM);
        }
        else
        {
        }
        break;
    case STATE_ALARM:
        if (event == EVENT_STOP_ALARM)
        {
            //Serial.println("Transitioning from Alarm to Bike Locked.");
            transitionState(machine, STATE_BIKE_LOCKED);
        }
        break;
    }
}