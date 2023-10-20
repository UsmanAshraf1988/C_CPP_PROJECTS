// fsm_light.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <vector>

#include "light.h"

const int INFO_PRINT_OUTS = 0;

int main()
{
    std::cout << "Hello World!\n";

    Light light;

	unsigned int option = 0u;
	while (1) {
		printf("Enter:: 0 to OFF/INIT, 1 to ON, 2 to Break\n");
		fflush(stdout);
		scanf_s("%d", &option); /*Unfortunately scanf_s is not available in my installed compiler version.*/

		if (option >= 0 && option <= 2) {
			switch (option)
			{
			case 0:
				light.update(Action::CURRENT_0_AMP);
				break;
			case 1:
				light.update(Action::CURRENT_100_MILIAMP);
				break;
			case 2:
				light.update(Action::CURRENT_900_MILIAMP);
				break;
			default:
				printf("!!!Wrong Input: Try Again!!!\n");
				break;
			}
		}
		else {
			printf("Wrong input: Therefore same state: Try next time\n");
		}
	}

    return 0;
}

