#include<iostream>
#include<thread>
#include<vector>
#include<atomic>


void threadfunc(){
	std::cout<<"Hello From thread.\n";
}


class Counter{
	public:
		Counter(){ mValue.store(0, std::memory_order_relaxed);}
		void incr(){
			mValue.store( ( mValue.load(std::memory_order_relaxed)+1 ),  std::memory_order_relaxed );
		}


		std::atomic<int> mValue;
};



int main(int argc, char * argv[]){


	std::cout<<"Hello From Main 1.\n";
	Counter counter;
	std::vector<std::thread> threads;

	for(int i=0; i<5; i++){

		threads.push_back(

				std::thread(

					[&counter](){ for(int j=0; j<50000; j++) counter.incr(); }

					)
				);
	}


	std::cout<<"Hello From Main 2.\n";
	for(std::thread & th: threads){
		th.join();
	}

	
	std::cout<<"Hello From Main 3.\n";
	std::cout<<"counter mValue: "<< counter.mValue.load(std::memory_order_relaxed)<<"\n";
	
	
	
	
	return 0;
}
