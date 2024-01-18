#ifndef TRANSITION_H_
#define TRANSITION_H_

#include <iostream>
#include<utility>
#include <memory>
#include "state.h"


extern const int INFO_PRINT_OUTS;

class FSM;

class Transition {
	const std::weak_ptr<State> mp_currState;
	Action m_action;
	const std::weak_ptr<State> mp_nextState;
public:
	Transition() : m_action{ Action::CURRENT_0_AMP } { if (INFO_PRINT_OUTS) std::cout << "Transition::Transition()" << std::endl;  }
	Transition(const std::weak_ptr<State> mPcurrState, Action action, const std::weak_ptr<State> mPnextState) : mp_currState{ mPcurrState },
		m_action{ action }, mp_nextState{ mPnextState } { if (INFO_PRINT_OUTS)std::cout << "Transition::Transition(...)" << std::endl; }
	
	Transition(const Transition& transition) = delete;
	Transition& operator=(const Transition& transition) = delete;
	Transition(Transition&& transition) = delete;
	Transition& operator=(Transition&& transition) = delete;
	
	~Transition() {
		// No allocation with 'New', so no deallocation with 'delete'.
		if (INFO_PRINT_OUTS)
			std::cout << "Transition::~Transition()" << std::endl; 
	};

	friend class FSM;
};


#endif /* TRANSITION_H_ */
