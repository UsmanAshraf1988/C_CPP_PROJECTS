//!@file genericSTL.cpp
//!It is application layer for custom STL prototype checking.
//!
//!
//!@author Usman Ashraf
//!@date 2018 
//!


//! system includes
#include<iostream>
#include<string>

//! custom includes
#include "digVector.h"
using namespace VVector;

int main(int argc, char* argv[])
{
	DigVector<std::string> digitalVec;
	std::cout<<"digitalVec size: "<< digitalVec.getSize()<<std::endl;
	std::cout<<"digitalVec capacity: "<< digitalVec.getCapacity()<<std::endl;

	digitalVec.push_back("1 one");
	digitalVec.push_back("2 two");
	digitalVec.push_back("3 three");
	digitalVec.push_back("4 four");
	digitalVec.push_back("5 five");
	digitalVec.push_back("6 six");
	digitalVec.push_back("7 seven");
	//digitalVec.push_back("8 eight");
	//digitalVec.push_back("9 nine");

	std::cout<<"digitalVec "<<digitalVec <<" cout main print out\n";

	std::cout<<"digitalVec size: "<< digitalVec.getSize()<<std::endl;
	std::cout<<"digitalVec capacity: "<< digitalVec.getCapacity()<<std::endl;

	DigIterator<std::string> iterStrt = digitalVec.begin();
        DigIterator<std::string> iterEnd = digitalVec.end();

	for(iterStrt; iterStrt!=iterEnd; ++iterStrt){
		std::cout<<*iterStrt<<std::endl;
	}

	return 0;
}
