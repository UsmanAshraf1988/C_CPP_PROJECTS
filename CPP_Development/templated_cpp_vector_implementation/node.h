//!@file Node.h
//!It is the declaration of Node class which
//!would be used for custom STL.
//!
//!
//!@author Usman Ashraf
//!@date 2018 
//!
#ifndef __NODE__H
#define __NODE__H


//! For debug messages, Please define this macro here.
//! Or, at compile time with macro flags
//! #define PROTOTYPE 1

namespace VVector{

#ifdef PROTOTYPE
#define PUT_LOG(msg) if(PROTOTYPE)std::cout<<msg;
#else
#define PUT_LOG(msg)
#endif


template<class T>
class Node{

public:
	template <class U>
	friend class DigVector;

	template<class U>
	friend class DigIterator;

	

private:

	T mData;
};

}
#endif
