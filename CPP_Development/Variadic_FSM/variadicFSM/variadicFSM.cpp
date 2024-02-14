/*
* Dependencies:
*   C++ 17 --> std::variant
*/

#include <cassert>
#include <iostream>
#include <string>
#include <variant>
#include <tuple>

enum class State_Key : unsigned char { STATE_A = 0u, STATE_B = 1u, STATE_C = 2u };

class Event {};
class EventA : public Event {};
class EventB : public Event {};
class EventC : public Event {};

template<typename... TEvents>
class StateMachineBaseActionHandler
{
public:
    void action() = delete;
    State_Key m_curr_state_key;
    State_Key m_next_state_key;
};

template<typename TEvent, typename... TEventRest>
class StateMachineBaseActionHandler<TEvent, TEventRest...> : public StateMachineBaseActionHandler<TEventRest...>
{
public:
    virtual void action(const TEvent& event)
    {
        std::cout << "default" << std::endl;
    }
    using StateMachineBaseActionHandler<TEventRest...>::action;
};

template <class ...TEvents>
class State : public StateMachineBaseActionHandler<TEvents...>
{
public:
    virtual void enterAction() = 0;
    virtual void exitAction() = 0;
};

class StateA : public State<EventA, EventB, EventC>
{
public:
    StateA() { 
        m_curr_state_key = State_Key::STATE_A;
        m_next_state_key = m_curr_state_key;
    }
    using State<EventA, EventB, EventC>::action;
    void action(const EventB& event) override {
        std::cout << "new A B" << std::endl; 
        m_next_state_key =  State_Key::STATE_B;
    }
    void action(const EventC& event) override {
        std::cout << "new A C" << std::endl;
        m_next_state_key = State_Key::STATE_C;
    }

    void enterAction() override { std::cout << "enter A" << std::endl; }
    void exitAction() override { std::cout << "exit A" << std::endl; }
};

class StateB : public State<EventA, EventB, EventC>
{
public:
    StateB() { 
        m_curr_state_key = State_Key::STATE_B;
        m_next_state_key = m_curr_state_key;
    }
    using State<EventA, EventB, EventC>::action;
    void action(const EventA& event) override {
        std::cout << "new B A" << std::endl;
        m_next_state_key = State_Key::STATE_A;
    }
    void action(const EventC& event) override {
        std::cout << "new B C" << std::endl;
        m_next_state_key = State_Key::STATE_C;
    }

    void enterAction() override { std::cout << "enter B" << std::endl; }
    void exitAction() override { std::cout << "exit B" << std::endl; }
};

class StateC : public State<EventA, EventB, EventC>
{
public:
    StateC() { 
        m_curr_state_key = State_Key::STATE_C;
        m_next_state_key = m_curr_state_key;
    }
    using State<EventA, EventB, EventC>::action;
    void action(const EventA& event) override {
        std::cout << "new C A" << std::endl;
        m_next_state_key = State_Key::STATE_A;
    }
    void action(const EventB& event) override {
        std::cout << "new C B" << std::endl;
        m_next_state_key = State_Key::STATE_B;
    }

    void enterAction() override { std::cout << "enter C" << std::endl; }
    void exitAction() override { std::cout << "exit C" << std::endl; }
};


template <class ...TStates>
class StateMachineBase
{
public:
    StateMachineBase()
    {
        m_cur = std::get<0>(m_states);
        m_prev = m_cur;
        std::visit([](auto state) {state.enterAction(); }, m_cur);
    }

    virtual ~StateMachineBase() = default;

    template<typename TEvent>
    void handleEvent(const TEvent& event)
    {
        std::visit( [&](auto&& state) { 
            state.action(event);
            switch (state.m_next_state_key)
            {
            case State_Key::STATE_A: {m_cur = std::get<StateA>(m_states); break; }
            case State_Key::STATE_B: {m_cur = std::get<StateB>(m_states); break; }
            case State_Key::STATE_C: {m_cur = std::get<StateC>(m_states); break; }
            default: break;
            }
            }, 
            m_cur
        );
        if (m_cur.index() != m_prev.index()) {
            std::visit([](auto state) {state.exitAction(); }, m_prev);
            m_prev = m_cur;
            std::visit([](auto state) {state.enterAction(); }, m_cur);
        }
    }

private:
    std::tuple<TStates...> m_states;
    std::variant<TStates...> m_cur;
    std::variant<TStates...> m_prev;
};

class StateMachine : public StateMachineBase<StateB, StateA, StateC> {};


int main()
{
    StateMachine testSM;
    testSM.handleEvent(EventA());
}