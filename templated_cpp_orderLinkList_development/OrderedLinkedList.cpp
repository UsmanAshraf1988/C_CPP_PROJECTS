//! @file OrderedLinkedList.cpp
//!
//!
//! @brief templated cpp based order link list development.
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
//! Flags: -std=c++11
//!


#include<iostream>
#include<memory>


namespace TASK{

template <class T>
bool lessThan(T a, T b){
	return a<b;
}



template <class T>
class Node{

public:
	template<class U>
	friend class Iterator;
	
	template <class U>
	friend class OrderedLinkedList;

private:
	T mData;
	Node* mNext;
};






//forward declaration 
template<class U>
class Iterator;








template <class T>
class OrderedLinkedList{

public:
	
	explicit OrderedLinkedList( bool (*orderFn)(T,T) = lessThan  ): mOrderFn(orderFn) {

		mNodeStart = nullptr;
		mSize=0;
	}

	OrderedLinkedList(const OrderedLinkedList&)=delete;


	void insert(T data) {

		if (mSize==0) {
			mNodeStart = new Node<T>();
			mNodeStart->mData = data;
			mNodeStart->mNext = nullptr;
			mSize++;
			return;

		} else {

			Node<T>* tmpNodePtr = mNodeStart;
			Node<T>* tmpNodePtrValid = mNodeStart;
			bool tmpFlagCheck=false;

			while( tmpNodePtr!=nullptr && mOrderFn(data, tmpNodePtr->mData) ){
				tmpNodePtrValid = tmpNodePtr ;
				tmpNodePtr = tmpNodePtr->mNext;
				tmpFlagCheck = true;
			}

			if(tmpFlagCheck && tmpNodePtrValid!=nullptr){
				Node<T>* tmpPtr = tmpNodePtrValid->mNext;
				tmpNodePtrValid->mNext = new Node<T>();
				tmpNodePtrValid->mNext->mData = data;
				tmpNodePtrValid->mNext->mNext = tmpPtr;
				mSize++;
			} else {
				mNodeStart = new Node<T>();
				mNodeStart->mData = data;
				mNodeStart->mNext =  tmpNodePtr;
				mSize++;
			}
		}
	}



	void print(){
		int i=0;

		std::cout << ">> ";
		Node<T>* tmpNodePtr = mNodeStart;
		while( i<mSize && tmpNodePtr!=nullptr){
			if(i!=mSize-1){
				std::cout << tmpNodePtr->mData << ", ";
			} else {
				std::cout << tmpNodePtr->mData << "\n";
			}
			i++;
			tmpNodePtr = tmpNodePtr->mNext;
		}
	}


	std::size_t getSize() const{
		return mSize;
	}

	T front() const {
		return mNodeStart->mData;
	}

	void pop_front(){
		Node<T>* tmpNodePtr = mNodeStart->mNext;
		delete mNodeStart;
		mNodeStart = tmpNodePtr;
		mSize--;
	}

	Iterator<T>& begin(){
		mStrtItr = new Iterator<T>(mNodeStart);
		return *mStrtItr;
	}

	Iterator<T>& end(){
		mEndItr = new Iterator<T>(nullptr);
		return *mEndItr;
	}

	void erase(Iterator<T>& itr){
		Node<T>* tmpNodePtr = mNodeStart;
		Node<T>* tmpNodePtrValid = mNodeStart;
		bool flag = false;

		while(tmpNodePtr!=nullptr && !( flag=(*itr==tmpNodePtr->mData) )  ){
				tmpNodePtrValid = tmpNodePtr ;
				tmpNodePtr = tmpNodePtr->mNext;
		}
		if( tmpNodePtr!=tmpNodePtrValid && tmpNodePtr!=nullptr && flag ){ // if not first node check
			// if not last node check
			if(tmpNodePtr->mNext){
				tmpNodePtrValid->mNext = tmpNodePtr->mNext;
			} else { //if last node check
				tmpNodePtrValid->mNext = nullptr;
			}
			delete tmpNodePtr;
			mSize--;
		} else if (  tmpNodePtr==tmpNodePtrValid && flag ) { //first node check
			mNodeStart = mNodeStart->mNext;
			delete tmpNodePtr;
			mSize--;
		} else {
			std::cerr<< *itr << "not exist in OrderedLinkList.\n";
		}

	}

	virtual ~OrderedLinkedList(){
		Node<T>* tmpNodePtr;

		while(mNodeStart!=nullptr){
			tmpNodePtr= mNodeStart;
			mNodeStart = mNodeStart->mNext;
			delete tmpNodePtr;
		}

		delete mStrtItr;
		delete mEndItr;
	}


private:
	std::size_t mSize;
	Node<T>* mNodeStart;
	bool (*mOrderFn)(T,T);
	Iterator<T>* mStrtItr;
	Iterator<T>* mEndItr;


};









template<class T>
class Iterator{

public:
	Iterator(Node<T>* pNode):mpNode(pNode){ }

	Iterator& operator = (const Iterator& digIter){
		this->mpNode = digIter.mpNode;
		return *this;
	}

	bool operator == (const Iterator& iterator){
		if (mpNode==iterator.mpNode){
			return true;
		} else {
			return false;
		}
	}

	bool operator != (const Iterator& iterator){
		return !(*this==iterator);
	}

	Iterator& operator++(){
		if(mpNode!=nullptr){
			mpNode = mpNode->mNext;
		}
		return *this;
	}

	Iterator& operator++(int){
		Iterator* pTempItr = this;
		++*this;
		return *pTempItr;
	}

	T operator*() const {
		if(mpNode!=nullptr){
			return mpNode->mData;
		} else {
			return (T)NULL;
		}
	}


private:
	Node<T>* mpNode;
};

} //namespae TASK




using namespace TASK;

int main(int argc, char* argv[]){


	OrderedLinkedList<int> myList(lessThan);

	myList.insert(5); //5
	myList.insert(6); //6,5
	myList.insert(7); //7,6,5
	myList.insert(4); //7,6,5,4
	myList.insert(3); //7,6,5,4,3
	myList.insert(42);//42,7,6,5,4,3
	myList.insert(1); //42,7,6,5,4,3,1
	myList.insert(-5);//42,7,6,5,4,3,1,-5
	//tmpNodePtrValid=3; tmpNodePtr=1; ==> creat mNext for 3 and to its mNext assign 1;
	myList.insert(2); //42,7,6,5,4,3,2,1,-5


	myList.print();

	std::cout<<"myList size: "<<myList.getSize()<<"\n";


	myList.pop_front();
	std::cout<<"After myList.pop_front()\n";
	myList.print();
	std::cout<<"myList size: "<<myList.getSize()<<"\n";
	std::cout<<"Just front without poping it: "<<myList.front()<<std::endl;

	Iterator<int> iterBegin = myList.begin();
	Iterator<int> iterEnd = myList.end();

	std::cout<<"Printing with iterator and for loop\n";
	for(iterBegin; iterBegin!=iterEnd; iterBegin++){
		std::cout<<*iterBegin<<"\n";
	}


	iterBegin = myList.begin();
	myList.erase(iterBegin);
	std::cout<<"Printing after erase() start element\n";
	myList.print();


	iterBegin = myList.begin();
	iterBegin++; 	
	myList.erase(iterBegin);
	std::cout<<"Printing after erase() 2nd element or say middle element, depending on iterator position.\n";
	myList.print();	

	iterBegin = myList.begin();
	iterEnd = myList.end();
	Iterator<int> lastValidNodeItr = iterBegin;
	while(iterBegin!=iterEnd){
		lastValidNodeItr = iterBegin;
		iterBegin++;
	}	
	myList.erase(lastValidNodeItr);
	std::cout<<"Printing after erase() last element.\n";
	myList.print();



	myList.insert(42);
	myList.insert(1);
	myList.insert(-5);

	std::cout<<"After adding some elements into myList.\n";
	myList.print();


	std::cout<<"Iter with while check\n";
	iterBegin = myList.begin();
	iterEnd = myList.end();
	while(iterBegin!=iterEnd){
		std::cout<<"*iterBegin: "<< *iterBegin << " From Main\n";
		++iterBegin;
	}


	return 0;
}//main

