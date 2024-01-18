#ifndef FSM_H_
#define FSM_H_

#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include "transitions.h"


extern const int INFO_PRINT_OUTS;

class Light;

class FSM {
	std::shared_ptr<State> mp_currState;
	std::map<StateName, std::shared_ptr<State>> m_states; // this is just to refer pointers, but would also be checked for deletion.
	std::map<std::shared_ptr<State>, std::vector<std::shared_ptr<Transition>> > m_stateTablePerStateMirror; // this will be used to delete resources.
	
public:
	FSM(Light * pLight=nullptr, StateCallbackFunction callBackOn = nullptr, StateCallbackFunction callBackOff = nullptr, 
		StateCallbackFunction callBackBroken = nullptr ) {

		m_states[StateName::INIT] = std::make_shared<State>(StateName::INIT, pLight, nullptr);
		m_states[StateName::ON] = std::make_shared<State>(StateName::ON, pLight, callBackOn);
		m_states[StateName::OFF] = std::make_shared<State>(StateName::OFF, pLight, callBackOff);
		m_states[StateName::BROKEN] = std::make_shared<State>(StateName::BROKEN, pLight, callBackBroken);

		// state_table
		m_stateTablePerStateMirror[m_states[StateName::INIT]] = std::vector<std::shared_ptr<Transition>>{
			std::make_shared<Transition>(m_states[StateName::INIT], Action::CURRENT_100_MILIAMP, m_states[StateName::ON]),
			std::make_shared<Transition>(m_states[StateName::INIT], Action::CURRENT_900_MILIAMP, m_states[StateName::BROKEN])
		};

		m_stateTablePerStateMirror[m_states[StateName::ON]] = std::vector<std::shared_ptr<Transition>>{
			std::make_shared<Transition>(m_states[StateName::ON], Action::CURRENT_0_AMP, m_states[StateName::OFF]),
			std::make_shared<Transition>(m_states[StateName::ON], Action::CURRENT_900_MILIAMP, m_states[StateName::BROKEN])
		};

		m_stateTablePerStateMirror[m_states[StateName::OFF]] = std::vector<std::shared_ptr<Transition>>{
			std::make_shared<Transition>(m_states[StateName::OFF], Action::CURRENT_100_MILIAMP, m_states[StateName::ON]),
			std::make_shared<Transition>(m_states[StateName::OFF], Action::CURRENT_900_MILIAMP, m_states[StateName::BROKEN])
		};

		if (INFO_PRINT_OUTS)
			std::cout << "FSM::FSM()" << std::endl;

		mp_currState = m_states[StateName::INIT];
	}

	~FSM() {
		if (INFO_PRINT_OUTS)
			std::cout << "FSM::~FSM()" << std::endl;
	}

	void execute(Action action) {
		if (INFO_PRINT_OUTS)
			std::cout << "FSM::execute(Action)" << std::endl;
		std::map<std::shared_ptr<State>, std::vector<std::shared_ptr<Transition>> >::iterator it =
			this->m_stateTablePerStateMirror.find(this->mp_currState);
		if (it != this->m_stateTablePerStateMirror.end()) {
			for (auto& p : it->second) {
				std::shared_ptr<State> spt1 = p->mp_currState.lock();
				if (spt1.use_count()>=1) {
					if (( this->mp_currState->m_stateName == spt1->m_stateName) && (action == p->m_action)) {
						//this->mp_currState.reset();
						this->mp_currState = p->mp_nextState.lock();
					}
				}
			}
		}
		this->mp_currState->execute();
	}
};

#endif /* FSM_H_ */
