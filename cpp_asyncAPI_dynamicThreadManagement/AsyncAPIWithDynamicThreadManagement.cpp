//! @file AsyncAPIWithDynamicThreadManagement.cpp
//!
//!
//! @brief It is demo to c++ based async API development using dynamic managment of threads
//!        with smartpointers raw list.
//!        
//! 
//!
//! @Author Usman Ashraf 
//! @email  u.ashraf1988@gmail.com
//!
//! GNU Lesser General Public License v3.0, free to use, re-use, distriution and modification
//! provided this above header information. Author take no responsibility for any outcome of
//! this code. 
//!
//! If you need more information, please, contact me at my above email address.
//
//! Successfully executed under OS:
//!                                RedHat 7
//!                                RedHat 5
//!                                CentOS 7
//!                                Ubuntu 16
//!                                yocto-created minimal image
//!
//!
//! Compiled with GCC Version > 4.8
//! flags: -std=c++11 



#include<iostream>
#include<stdio.h>
#include<thread>
#include<mutex>
#include<memory>
#include<chrono>



#ifdef D1DEBUG
	#define LOG(msg) std::cout<<"msg: "<<msg<<std::endl;
#else
	#define LOG(msg) if(msg==msg) printf("\r");
#endif

template <typename T>
class ThreadHandler
{
public:

	ThreadHandler()
	{
		local_counter = global_counter;
		global_counter++;
		mth.reset();
		mNext.reset();
		LOG("ThreadHandler CONSTRUCTOR WITH COUNTER NUMBER: SEE NUMBER BELOW");
		LOG(local_counter);
	}

	ThreadHandler(const ThreadHandler<T> & thl) = delete;

	ThreadHandler(ThreadHandler<T>&& thl)= delete;


	virtual ~ThreadHandler()
	{
		LOG("~ThreadHandler DESTRUCTOR WITH COUNTER NUMBER: SEE NUMBER BELOW");
		LOG(local_counter);
	}


	void joinAllAndReset()
	{
		LOG("joinAllAndReset");
		if( mth.get() != nullptr )
		{
			if( mth->joinable() )
			{
				mth->join();
			}
			mth.reset();
		}

		while( mNext.get()!=nullptr )
		{
			if( mNext->mth.get() != nullptr )
			{
				if ( mNext->mth->joinable() )
				{
					mNext->mth->join();
				}
				mNext->mth.reset();
			}

			mNext = mNext->mNext;
		}

	}// joinAllAndReset

	void setThreadAndAddReqNode( std::thread * th )
	{
		LOG("setThreadAndAddReqNode");
		if( mth.get()==nullptr )
		{
			mth.reset( std::move(th) );
		}
		else
		{
			if( mNext.get() == nullptr)
			{
				mNext = std::make_shared< ThreadHandler<T> > ();
				mNext->mth.reset( std::move(th) );
				return;
			}
			std::shared_ptr< ThreadHandler<T> > tmpTHL = mNext;
			std::shared_ptr< ThreadHandler<T> > tmpTHL_valid;
			while( tmpTHL.get() != nullptr )
			{
				tmpTHL_valid = tmpTHL;
				tmpTHL = tmpTHL->mNext;
			}
			tmpTHL = std::make_shared< ThreadHandler<T> > (); 
			tmpTHL->mth.reset( std::move(th) );
			tmpTHL_valid->mNext = tmpTHL;
		}
	}// setThreadAndAddReqNode

private:
	std::unique_ptr< std::thread > mth;
	std::shared_ptr< ThreadHandler<T> > mNext;
	static int global_counter;
	int local_counter;
};
template <typename T>
int ThreadHandler<T>::global_counter=0;


template <typename T>
class Service
{
public:
	Service()
	{
		LOG("Service Constructor");
		mVal=(T)0;
		mSetValTHL.reset( std::move(new ThreadHandler<T>()) );
	}

	Service(const Service<T>& service) = delete;

	Service(Service<T> && service) = delete;

	virtual ~Service()
	{
		LOG("Service Destructor");
	}
	
	void asyncSetValue(T val)
	{
		LOG("asyncSetValue");
		mSetValTHL->setThreadAndAddReqNode( new std::thread(&Service<T>::setValue, this, val) );
	}

	void setValue(T val)
	{
		std::lock_guard< std::mutex > lck(mmtx);
		LOG("setValue");
		mVal=val;
	}

	T getValue()
	{
		std::lock_guard< std::mutex > lck(mmtx);
		LOG("getValue");
		mSetValTHL->joinAllAndReset();
		return mVal;
	}

private:
	T mVal;
	std::unique_ptr < ThreadHandler<T> > mSetValTHL;
	std::mutex mmtx;
};





int main()
{
	std::cout<<"Hello World1\n";
	LOG("Main 1");
	std::cout<<"Hello World2\n";

	Service<int> ser;
	ser.asyncSetValue(1);
	ser.asyncSetValue(9);
	ser.asyncSetValue(19);
	ser.asyncSetValue(19);
	ser.asyncSetValue(19);
	ser.asyncSetValue(19);
	ser.asyncSetValue(786);
	int getVal = ser.getValue();
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout<<"Main ser.getValue(): "<< getVal <<"\n";
	LOG("Main 2");



	std::cout<<"Hello World3\n";

	return 0;
}
