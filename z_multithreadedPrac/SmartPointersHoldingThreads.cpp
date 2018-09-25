
#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <chrono>

std::mutex mutex;




void thread_function(int value)
{
    	std::unique_lock<std::mutex> uniqlck(mutex);
 	std::cout<<"Inside Thread :: ID  = "<<std::this_thread::get_id()<<"\n";
    	std::cout<<"Inside Thread :: ID: "<<std::this_thread::get_id() <<"==> value  = "<< value <<"\n";
	std::flush(std::cout);
}




class ThreadNode{

public:	
	ThreadNode(){
		mJoinAllRequired = true;
	}

	virtual ~ThreadNode(){
	}

	void join_all_t(){
		std::lock_guard<std::mutex> uniqlck(mmutext);
		if(mJoinAllRequired){
			if (this->mThread->joinable()){
			  this->mThread->join();
			}
			std::shared_ptr<ThreadNode> tmpPtr = this->mNextThreadNode;
			while(tmpPtr){
				tmpPtr->mThread->join();
				tmpPtr=tmpPtr->mNextThreadNode;
			}
			mJoinAllRequired = false;
		}

	}


	void push(){
		std::unique_lock<std::mutex> uniqlck(mmutex);
		std::shared_ptr<ThreadNode> tmpPtr = this->mNextThreadNode;
		std::shared_ptr<ThreadNode> tmpPtrValid = this->mNextThreadNode;
		while(tmpPtr){
			tmpPtrValid = tmpPtr;
			tmpPtr=tmpPtr->mNextThreadNode;
		}

		if (tmpPtrValid){
			tmpPtrValid->mNextThreadNode.reset(new ThreadNode);
			tmpPtr->mNextThreadNode->mThread.reset(new std::thread(&ThreadNode::thread_function_t,this,9));
		} else {
			this->mThread.reset(new std::thread(&ThreadNode::thread_function_t, this,33));
		}
				
	}


	void thread_function_t(int value)
	{
	    	std::unique_lock<std::mutex> uniqlck(mmutex);
	 	std::cout<<"Inside Thread :: ID  = "<<std::this_thread::get_id()<<"\n";
	    	std::cout<<"Inside Thread :: ID: "<<std::this_thread::get_id() <<"==> value  = "<< value <<"\n";
		std::flush(std::cout);
	}

	std::shared_ptr<std::thread> mThread;
	std::shared_ptr<ThreadNode> mNextThreadNode;
	bool mJoinAllRequired;
	std::mutex mmutext;
	std::mutex mmutex;

};



template<class T>
class Worker{


public:
	Worker():mValue( (T)0 ){}
	virtual ~Worker(){}

	operator ()(T value){
		std::unique_lock<std::mutex> uniqlck(mmutex);
		mValue=value;
		std::cout<<"From worker()()  ==> mValue: "<<mValue<<"\n";
	}

	void run(T value){
		std::lock_guard<std::mutex> lckGrd(mmutex);
		mValue=value;
		std::cout<<"From worker::run(int)  ==> mValue: "<<mValue<<"\n";
	}



private:
	T mValue;
	std::mutex mmutex;
};






int main(int argc, char * argv[])  
{
	std::cout<<"main Thread :: ID  = "<<std::this_thread::get_id()<<"\n";

	
    	std::shared_ptr<ThreadNode> shrdPtr(new ThreadNode);
	shrdPtr->push();


	Worker<int> worker;
	shrdPtr->mNextThreadNode.reset(new ThreadNode);	
    	shrdPtr->mNextThreadNode->mThread.reset(new std::thread(std::ref(worker),33) );
	shrdPtr->mNextThreadNode->mNextThreadNode.reset(new ThreadNode);
	shrdPtr->mNextThreadNode->mNextThreadNode->mThread.reset(new std::thread(&Worker<int>::run,std::ref(worker),99));

    	if(shrdPtr->mThread->get_id() != shrdPtr->mNextThreadNode->mThread->get_id())
        	std::cout<<"Both Threads have different IDs\n";
        std::cout<<"From Main Thread :: ID of Thread 1 = "<<shrdPtr->mThread->get_id()<<"\n"; 
    	std::cout<<"From MainThread::ID of Thread2="<<shrdPtr->mNextThreadNode->mThread->get_id()<<"\n";    
	shrdPtr->join_all_t();
	
	return 0;

}
