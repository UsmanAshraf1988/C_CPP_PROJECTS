#ifndef __light_h__
#define __light_h__


#include <iostream>
#include <utility>
#include "FSM.h"

extern const int INFO_PRINT_OUTS;

class Light {
	FSM fsm{ this, &Light::lightOn, &Light::lightOff, &Light::lightBroken };

public:

	Light() {
		if (INFO_PRINT_OUTS)
			std::cout << "Light::Light()" << std::endl;
	}

	~Light() {
		// No allocation with 'New', so no deallocation with 'delete'.
		if (INFO_PRINT_OUTS)
			std::cout << "Light::~Light()" << std::endl;
	}

	void update(Action action) {
		if(INFO_PRINT_OUTS)
			std::cout << "Light::update(Action)" << std::endl;
		fsm.execute(action);
	}

	void lightOn(void) { printf("light ON \n"); }
	void lightOff(void) { printf("light OFF \n"); }
	void lightBroken(void) { printf("light Broken \n"); }
};

#endif // !__light_h__
