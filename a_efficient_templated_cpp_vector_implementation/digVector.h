//!@file digVector.h
//!It is the declaration of DigVector class which
//!would be used for custom STL.
//!
//!
//!@author Usman Ashraf
//!@date 2018 
//!
#ifndef __DIGVECTOR__H
#define __DIGVECTOR__H

//! system includes
#include <iostream>

//! custom include
#include "node.h"
#include "digIterator.h"

namespace VVector{

#define DEFAULTCAPACITY 2

template<class T>
class DigVector{

public:

	DigVector(std::size_t capacity=DEFAULTCAPACITY){
		mSize=0;
		mCapacityFactor=1;
		mCapacity=capacity;
		mppNodes= new Node<T>*[mCapacity];
		initializeStoragePointer();
		digIterStrt = nullptr;
		digIterEnd = nullptr;
	}

	void push_back(T nodeData){
		if (mSize==mCapacity){
			mCapacityFactor++;
			mCapacity = mCapacityFactor * DEFAULTCAPACITY;
			enlargeVector();
		}
		mppNodes[mSize] = new Node<T>();
		mppNodes[mSize]->mData = nodeData;
		mSize++;
	}

	T at_index(std::size_t index) {
		if (index <= mCapacity && index<=mSize){
			return mppNodes[index]->mData;
		}
	}

	void enlargeVector(){

		Node<T>** tempPPNodes=mppNodes;
		mppNodes = new Node<T>*[mCapacity];
		initializeStoragePointer();
		for(std::size_t i=0; i<(mSize); i++){
			mppNodes[i] = new Node<T>();
			mppNodes[i]->mData=tempPPNodes[i]->mData;
			delete tempPPNodes[i];
		}
		delete [] tempPPNodes;
	}


	std::size_t getSize() const{
		return mSize;
	}

	std::size_t getCapacity() const{
		return mCapacity;
	}

	void initializeStoragePointer(){
		for(std::size_t i=0; i<(mCapacity); i++){
			mppNodes[i] = nullptr;
		}
	}	

	virtual ~DigVector(){

		for(std::size_t i=0; i<(mCapacity); i++){
			delete mppNodes[i];
		}

		delete [] mppNodes;

		if(digIterStrt){
			delete digIterStrt;
		}
		if(digIterEnd){
			delete digIterEnd;
		}
		
	}


	friend std::ostream& operator << (std::ostream& out, DigVector& vec){

		out<<"( ";
		for(std::size_t i=0; i<vec.getSize(); i++){
			if (i==( vec.getSize()-1 ) ) {
				out << vec.at_index(i) <<" )\n";
			}else {
				out << vec.at_index(i) << ", ";
			}
		}

		return out;
	}

	DigIterator<T>& begin(){
		digIterStrt = new DigIterator<T>(mppNodes);
		return *digIterStrt;
	}

	DigIterator<T>& end(){
		digIterEnd = new DigIterator<T>(nullptr);
		return *digIterEnd;
	}

private:
	Node<T>** mppNodes;
	std::size_t mSize;
	std::size_t mCapacity;
	std::size_t mCapacityFactor;
        DigIterator<T>* digIterStrt;
	DigIterator<T>* digIterEnd;
};
}
#endif
