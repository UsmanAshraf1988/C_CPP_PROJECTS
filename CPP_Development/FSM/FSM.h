#ifndef FSM_H_
#define FSM_H_

#include <iostream>
#include <unordered_map>
#include <vector>
#include "transitions.h"

extern const int INFO_PRINT_OUTS;

class Light;

class FSM {
	State* mp_currState;
	std::map<State*, std::vector<Transition*> > m_stateTablePerStateMirror; // this will be used to delete resources.
	std::map<StateName, State*> m_states; // this is just to refer pointers, but would also be checked for deletion.
	std::map<State*, std::vector<Transition*> >::iterator it;
public:
	FSM(Light * pLight=nullptr, StateCallbackFunction callBackOn = nullptr, StateCallbackFunction callBackOff = nullptr, 
		StateCallbackFunction callBackBroken = nullptr ) {

		m_states[StateName::INIT] = new State(StateName::INIT, pLight, nullptr);
		m_states[StateName::ON] = new State(StateName::ON, pLight, callBackOn);
		m_states[StateName::OFF] = new State(StateName::OFF, pLight, callBackOff);
		m_states[StateName::BROKEN] = new State(StateName::BROKEN, pLight, callBackBroken);
		mp_currState = m_states[StateName::INIT];

		// state_table
		m_stateTablePerStateMirror[m_states[StateName::INIT]] = std::vector<Transition*>{
			new Transition(m_states[StateName::INIT], Action::CURRENT_100_MILIAMP, m_states[StateName::ON]),
			new Transition(m_states[StateName::INIT], Action::CURRENT_900_MILIAMP, m_states[StateName::BROKEN]),
		};

		m_stateTablePerStateMirror[m_states[StateName::ON]] = std::vector<Transition*>{
			new Transition(m_states[StateName::ON], Action::CURRENT_0_AMP, m_states[StateName::OFF]),
			new Transition(m_states[StateName::ON], Action::CURRENT_900_MILIAMP, m_states[StateName::BROKEN]),
		};

		m_stateTablePerStateMirror[m_states[StateName::OFF]] = std::vector<Transition*>{
			new Transition(m_states[StateName::OFF], Action::CURRENT_100_MILIAMP, m_states[StateName::ON]),
			new Transition(m_states[StateName::OFF], Action::CURRENT_900_MILIAMP, m_states[StateName::BROKEN]),
		};

		if (INFO_PRINT_OUTS)
			std::cout << "FSM::FSM()" << std::endl;
	}

	~FSM() {
		for (auto& p : this->m_stateTablePerStateMirror) {
			for (auto& q : p.second) {
				if (q != nullptr)
					delete q;
			}
			if (p.first != nullptr)
				delete p.first;
		}

		if (m_states[StateName::BROKEN] != nullptr)
			delete m_states[StateName::BROKEN];

		if (INFO_PRINT_OUTS)
			std::cout << "FSM::~FSM()" << std::endl;
	}

	void execute(Action action) {
		if (INFO_PRINT_OUTS)
			std::cout << "FSM::execute(Action)" << std::endl;
		it = this->m_stateTablePerStateMirror.find(this->mp_currState);
		if (it != this->m_stateTablePerStateMirror.end()) {
			for (auto& p : it->second) {
				if ((this->mp_currState == p->mp_currState) && (action == p->m_action)) {
					this->mp_currState = (State*)p->mp_nextState;
				}
			}
		}
		this->mp_currState->execute();
	}
};

#endif /* FSM_H_ */
