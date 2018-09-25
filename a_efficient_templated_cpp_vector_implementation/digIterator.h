//!@file digIterator.h
//!@brief It is the digIterator implementation for sequential container vector
//!
//!
//!@author Usman Ashraf 
//!@date   2018

#ifndef __DIGITERATOR__H
#define __DIGITERATOR__H

//! custom include
#include "node.h"

namespace VVector{

template<class T>
class DigIterator{

public:
	DigIterator(Node<T>** pNode):mpNode(pNode){}

	DigIterator& operator = (const DigIterator& digIter){
		this->mpNode = digIter.mpNode;
		this->mpNode = digIter.mpNode;
		return *this;
	}

	bool operator == (const DigIterator& iterator){
		if ((*mpNode)==nullptr){
			PUT_LOG("End of iterator.\n");
			return true;
		} else {
			return false;
		}
	}

	bool operator != (const DigIterator& iterator){
		return !(*this==iterator);
	}

	DigIterator& operator++(){
		if((*mpNode)!=nullptr){
			mpNode++;
		}
		return *this;
	}

	DigIterator& operator++(int){
		DigIterator* pTempItr = this;
		++*this;
		return *pTempItr;
	}

	T operator*(){
		if((*mpNode)!=nullptr){
			return (*mpNode)->mData;
		} else {
			return NULL;
		}
	}


private:
	Node<T>** mpNode;
};
}

#endif
