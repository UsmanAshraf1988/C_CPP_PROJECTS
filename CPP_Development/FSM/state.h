
#ifndef STATE_H_
#define STATE_H_

#include <iostream>

extern const int INFO_PRINT_OUTS;

class FSM;
class Light;

typedef void (Light::* StateCallbackFunction)(void);

enum class Action {
	CURRENT_0_AMP,
	CURRENT_100_MILIAMP,
	CURRENT_900_MILIAMP
};


enum class StateName {
	INIT,
	ON,
	OFF,
	BROKEN
};

class State {
	friend class FSM;
	StateName m_stateName;
	Light* mp_light;
	StateCallbackFunction m_StateCallbackFunction;
public:
	State() :m_stateName{ StateName::INIT }, mp_light{ nullptr }, m_StateCallbackFunction{ nullptr } { if (INFO_PRINT_OUTS) std::cout << "State::State()" << std::endl; }
	State(StateName stateName, Light *pLight, StateCallbackFunction StateCallbackFunction=nullptr) : m_stateName{ stateName }, mp_light{ pLight },
		m_StateCallbackFunction{ StateCallbackFunction } {if (INFO_PRINT_OUTS) std::cout << "State::State(...)" << std::endl; }
	State(const State& state) {
		this->m_stateName = state.m_stateName;
		this->m_StateCallbackFunction = state.m_StateCallbackFunction;
		this->mp_light = state.mp_light;
		if (INFO_PRINT_OUTS)
			std::cout << "State::State(const State&)" << std::endl;
	}
	State& operator=(const State& state) {
		this->m_stateName = state.m_stateName;
		this->m_StateCallbackFunction = state.m_StateCallbackFunction;
		this->mp_light = state.mp_light;
		if (INFO_PRINT_OUTS)
			std::cout << "State::operator=(const State&)" << std::endl;
		return *this;
	}
	State(State&& state) noexcept {
		this->m_stateName = state.m_stateName;
		state.m_stateName = StateName::INIT;
		this->m_StateCallbackFunction = state.m_StateCallbackFunction;
		state.m_StateCallbackFunction = nullptr;
		this->mp_light = state.mp_light;
		state.mp_light = nullptr;
		if (INFO_PRINT_OUTS)
			std::cout << "State::State(State&&)" << std::endl;
	}
	State& operator=(State&& state) noexcept  {
		this->m_stateName = state.m_stateName;
		state.m_stateName = StateName::INIT;
		this->m_StateCallbackFunction = state.m_StateCallbackFunction;
		state.m_StateCallbackFunction = nullptr;
		this->mp_light = state.mp_light;
		state.mp_light = nullptr;
		if (INFO_PRINT_OUTS)
			std::cout << "State::operator=(State&&)" << std::endl;
		return *this;
	}

	~State() {
		// No allocation with 'New', so no deallocation with 'delete'.
		if (INFO_PRINT_OUTS)
			std::cout << "State::~State()" << std::endl; 
	};

	void execute() {
		if (INFO_PRINT_OUTS)
			std::cout << "State::execute()" << std::endl;
		if (this->mp_light != nullptr && m_StateCallbackFunction!=nullptr) {
			(this->mp_light->*m_StateCallbackFunction)();
		}
	}
};

#endif /* STATE_H_ */
