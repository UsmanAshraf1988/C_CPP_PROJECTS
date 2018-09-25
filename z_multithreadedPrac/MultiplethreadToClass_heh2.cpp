#include<iostream>
#include<thread>
#include<vector>
#include<mutex>


void threadfunc(){
	std::cout<<"Hello From thread.\n";
}


class Counter{
	public:
		Counter():mValue(0){}
		void incr(){++mValue;}
		void show(){
			 mtx.lock();
			 std::cout<<"Hello From thread show(), this: "<< this <<"\n";
			 mtx.unlock();			
		}

		void operator ()(){
			//If you take lock and unlock here, this whole function would be locked
			//regardless of if you unlock before end of funtor.
			 for(int j=0; j<50000; j++){
				 incr();
			 }
			 show();
		}

		int mValue;
		std::mutex mtx;
};



int main(int argc, char * argv[]){


	std::cout<<"Hello From Main 1.\n";
	Counter counter;
	std::thread threads[5];

	std::cout<<"From Main &counter: "<<&counter<<std::endl;
	for(int i=0; i<5; i++){
		threads[i] = std::thread(std::ref(counter));
	}


	std::cout<<"Hello From Main 2.\n";
	for(int i=0; i<5; i++){
		threads[i].join();
	}

	
	std::cout<<"Hello From Main 3.\n";
	std::cout<<"counter mValue: "<< counter.mValue<<"\n";
	
	
	
	
	return 0;
}
