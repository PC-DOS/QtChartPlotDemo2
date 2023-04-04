#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#define SM_STATE_NOTMAL 0
#define SM_STATE_SAMPLINGRATE_ADJUSTING 1
#define SM_STATE_GAIN_ADJUSTING 2
#define SM_STATE_TIMESPAN_ADJUSTING 3

#define SM_TOTLAL_STATE_COUNT 4

class StateMachine{
private:
    int iCurrentState;
    int iLastState;

public:
    StateMachine();
    ~StateMachine();

    void SetCurrentState(int iNewState);
    int GetCurrentState();
    void RevertToLastState();
};

#endif // STATEMACHINE_H
