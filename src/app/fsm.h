/**
 * 
*/
#ifndef _FSM_H
#define _FSM_H

#include <stdio.h>
#include <time.h>

// Define the states
typedef enum {
    STATE_UNDEFINED = -1,
    STATE_IDLE, // byke is actualy moving
    STATE_READY, // system just started
    STATE_ACTIVE, // like UNLOCKED , bike is not moving
    STATE_BIKE_LOCKED, // bike parked and is not moving
    STATE_ALARM, 
} State;

// Define the events
typedef enum {
    EVENT_TRIGGER,
    EVENT_TIMEOUT,
    EVENT_BIKE_IS_PARKING,
    EVENT_BIKE_IS_MOVING,
    EVENT_LOCK_BIKE,
    EVENT_UNLOCK_BIKE,
    EVENT_START_ALARM,
    EVENT_STOP_ALARM
} Event;

// Define the state machine structure
typedef struct {
    State previous_state;
    State current_state;
    clock_t start_time;
} StateMachine;


void initStateMachine(StateMachine *machine);
void transitionState(StateMachine *machine, State new_state);
void handleEvent(StateMachine *machine, Event event);

#endif /*_GSM_H*/