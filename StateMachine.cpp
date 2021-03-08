#include "StateMachine.h"
#include <iostream>

using namespace std;

StateMachine::StateMachine(){
    cerr<<"StateMachine: Initialized, current state 0 (SM_STATE_NOTMAL)"<<endl;
    _iCurrentState=SM_STATE_NOTMAL;
    _iLastState=SM_STATE_NOTMAL;
}

StateMachine::~StateMachine(){

}

void StateMachine::SetCurrentState(int iNewState){
    if (iNewState<0 || iNewState>=SM_TOTLAL_STATE_COUNT){
        cerr<<"StateMachine: Unknown state value met, state value was not changed."<<endl;
        return;
    }
    _iLastState=_iCurrentState;
    _iCurrentState=iNewState;
    cerr<<"StateMachine: Entering state "<<_iCurrentState<<"."<<endl;
    return;
}

int StateMachine::GetCurrentState(){
    return _iCurrentState;
}

void StateMachine::RevertToLastState(){
    swap(_iCurrentState,_iLastState);
    cerr<<"StateMachine: Entering state "<<_iCurrentState<<"."<<endl;
    return;
}
