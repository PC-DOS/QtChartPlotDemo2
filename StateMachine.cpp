#include "StateMachine.h"
#include <iostream>

using namespace std;

StateMachine::StateMachine(){
    cerr<<"StateMachine: Initialized, current state 0 (SM_STATE_NOTMAL)"<<endl;
    _iCurrentState=SM_STATE_NOTMAL;
}

StateMachine::~StateMachine(){

}

void StateMachine::SetCurrentState(int iNewState){
    if (iNewState<0 || iNewState>=SM_TOTLAL_STATE_COUNT){
        cerr<<"StateMachine: Unknown state value met, state value was not changed."<<endl;
        return;
    }
    cerr<<"StateMachine: Entering state "<<iNewState<<"."<<endl;
    _iCurrentState=iNewState;
    return;
}

int StateMachine::GetCurrentState(){
    return _iCurrentState;
}
