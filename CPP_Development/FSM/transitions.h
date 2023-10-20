#ifndef TRANSITION_H_
#define TRANSITION_H_

#include <iostream>
#include<utility>
#include "state.h"

extern const int INFO_PRINT_OUTS;

class FSM;

class Transition {
	const State * mp_currState;
	Action m_action;
	const State * mp_nextState;
public:
	Transition() : mp_currState{ nullptr }, m_action{ Action::CURRENT_0_AMP }, mp_nextState{ nullptr } { if (INFO_PRINT_OUTS) std::cout << "Transition::Transition()" << std::endl;  }
	Transition(const State* mPcurrState, Action action, const State* mPnextState) : mp_currState{ mPcurrState },
		m_action{ action }, mp_nextState{ mPnextState } { if (INFO_PRINT_OUTS)std::cout << "Transition::Transition(...)" << std::endl; }
	
	Transition(const Transition& transition) {
		this->mp_currState = transition.mp_currState;
		this->m_action = transition.m_action;
		this->mp_nextState = transition.mp_nextState;
		if (INFO_PRINT_OUTS)
			std::cout << "Transition::Transition(coonst Transition&)" << std::endl;
	}
	Transition& operator=(const Transition& transition) {
		this->mp_currState = transition.mp_currState;
		this->m_action = transition.m_action;
		this->mp_nextState = transition.mp_nextState;
		if (INFO_PRINT_OUTS)
			std::cout << "Transition::operator=(const Transition&)" << std::endl;
		return *this;
	}
	Transition(Transition&& transition) noexcept {
		this->mp_currState = std::move(transition.mp_currState);
		transition.mp_currState = nullptr;
		this->m_action = transition.m_action;
		transition.m_action = Action::CURRENT_0_AMP;
		this->mp_nextState = std::move(transition.mp_nextState);
		if (INFO_PRINT_OUTS)
			std::cout << "Transition::Transition(Transition&&)" << std::endl;
		transition.mp_nextState = nullptr;
	}
	Transition& operator=(Transition&& transition)  noexcept {
		this->mp_currState = std::move(transition.mp_currState);
		transition.mp_currState = nullptr;
		this->m_action = transition.m_action;
		transition.m_action = Action::CURRENT_0_AMP;
		this->mp_nextState = std::move(transition.mp_nextState);
		transition.mp_nextState = nullptr;
		if (INFO_PRINT_OUTS)
			std::cout << "Transition::operator=(Transition&&)" << std::endl;
		return *this;
	}
	
	~Transition() {
		// No allocation with 'New', so no deallocation with 'delete'.
		if (INFO_PRINT_OUTS)
			std::cout << "Transition::~Transition()" << std::endl; 
	};

	friend class FSM;
};


#endif /* TRANSITION_H_ */
