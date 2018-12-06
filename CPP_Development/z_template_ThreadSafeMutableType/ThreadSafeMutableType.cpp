//! @file ThreadSafeMutableType.cpp
//!
//!
//! @brief It is demo to c++ based thread safe mutable type.
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
//! flags: -std=c++11 -lpthread
//! Shell Command: g++ -std=c++11 -lpthread ThreadSafeMutableType.cpp

#include <iostream> 
#include <thread> 
#include <mutex> 

#define CHECKER_VAL 999

#ifndef NO_DEBUG_MSG
	#define LOG(msg) std::cout<<"msg: Thread id:"<<std::this_thread::get_id()<<": " <<msg<<std::endl;
#else
	#define LOG(msg) if(msg==msg) printf("\r");
#endif
 


template <typename T, typename U>
class AreSameType {

public:
  static const bool value=false;

};


template <typename T>
class AreSameType<T, T> {

public:
  static const bool value = true;

};



template <typename T>
class Mentor {

public: 
  static const int TTT1;

  void display1() const {
    LOG( "Mentor display1 function ('~')" );
  }

  virtual void display2() const {
    LOG( "Mentor display2 function ('~')" );
  }

};


template <>
class Mentor <double> {

public: 
  static const int TTT1;

  void display1() const {
    LOG( "Mentor specialized display1 function (*~*)" );
  }

  virtual void display2() const {
    LOG( "Mentor specialized display2 function (*~*)" );
  }

};
template <typename T>
const int Mentor<T>::TTT1=CHECKER_VAL;
const int Mentor<double>::TTT1=CHECKER_VAL;



template <typename T>
class Checker : private Mentor<T> {

public: 
  static const int TTT;

  void display_resolver() const {
    Mentor<T>::display1();
  }

  virtual void display2() const {
    LOG("(%~%)Checker(%~%) ");
    Mentor<T>::display2();
  }

};
template <typename T>
const int Checker<T>::TTT=CHECKER_VAL;



template <typename T>
class ThreadSafeMutableType : public Checker<T> {

protected:

  mutable T val;
  mutable std::mutex mmtx;

public:
 
  ThreadSafeMutableType<T> () {
    LOG( "constructor" );
  }

  ThreadSafeMutableType<T> (const ThreadSafeMutableType<T> & foo ) {
    LOG( "copy constructor" );
  }


  ThreadSafeMutableType<T>& operator = (const ThreadSafeMutableType<T> & foo) {
    LOG( "Equal operator" );
    return *this;
  }

  void operator ( ) ( )  const {
    LOG( "functor operator" );
  }

  void operator * ( )  const {
    LOG( "Dereferenced operator" );
  }

  void operator [ ] (const unsigned int & i ) const {
    LOG( "Index operator" );
  }

  operator bool ( ) const {
    LOG( "bool operator" );
    return !AreSameType< Checker<T> , Mentor<T> >::value;
  }

  const ThreadSafeMutableType< T > * const & operator -> ( ) const {
    LOG( "Arrow operator" );
    return this;
  }


  void display() const {
    LOG( "Display function (#~#)");
  }

  void setValue(const T & i) const {
    LOG( "Setter" );
    std::lock_guard<std::mutex> lck(mmtx);
    val=i;
  }

  T getValue() const {
    LOG( "Getter" );
    return val;
  }

  virtual ~ThreadSafeMutableType<T>() {
    LOG( "Destructor" );
  }

};




int main(int argc, char * argv []) {
 
    const ThreadSafeMutableType<double> foo1; // constructor
    ThreadSafeMutableType<double> foo2=foo1;// copy constructor
    ThreadSafeMutableType<double> foo3(foo2);// copy constructor


    foo3 = foo1;// = operator
    foo1();// () functor
    *foo1;// * operator
    foo1[999]; // [] operator

    // bool operator
    if ( foo1 ) { 
      foo1.display_resolver();
      foo1.display2();
    }

    foo1.setValue(Mentor<long>::TTT1); // Mutable storage access specifiar member of const object is changed (*~*).
    LOG((foo1->getValue())); //  -> operator;

    return 0; 
}

