//! @file vectorCompact.cpp
//!
//!
//! @brief dynamically changing capacity vector implementation.
//! 
//!
//! @ Author: Usman Ashraf 
//! @ email:  u.ashraf1988@gmail.com
//!
//! MIT-License, free to use, re-use, distriution and modification provided this above
//! header information. Author take no responsibility for any outcome of this code. This
//! is self practcing code and not well tested.
//! 
//!
//! If you need more information, please, contact me at my email address.

#ifndef __NODE__H
#define __NODE__H

//! system includes
#include<iostream>
#include<string>

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




#ifndef __DIGVECTOR__H
#define __DIGVECTOR__H


namespace VVector{


//Forward declaration of vector iterator.
template<class T>
class DigIterator;


#define DEFAULTCAPACITY 3

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

	DigVector(const DigVector & dV) = delete;

	DigVector(DigVector && dV){
		this->mppNodes = dV.mppNodes;
		this->mSize = dV.mSize;
		this->mCapacity = dV.mCapacity;
		this->mCapacityFactor = dV.mCapacityFactor;

		dV.mppNodes = nullptr;
		dV.mSize = 0;
		dV.mCapacity = 0;
		dV.mCapacityFactor = 0;
		dV.digIterStrt = nullptr;
		dV.digIterEnd = nullptr;
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


#ifndef __DIGITERATOR__H
#define __DIGITERATOR__H


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
