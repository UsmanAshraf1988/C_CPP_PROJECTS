#include<iostream>
#include<thread>


void threadfunc(){
	std::cout<<"Hello From thread.\n";
}


int main(int argc, char * argv[]){


	std::cout<<"Hello From Main 1.\n";
	std::thread th1(threadfunc);
	std::cout<<"Hello From Main 2.\n";
	th1.join();
	std::cout<<"Hello From Main 3.\n";
	return 0;
}
