// future example
#include <iostream>       // std::cout
#include <future>         // std::async, std::future
#include <chrono>         // std::chrono::milliseconds
#include <thread>

// a non-optimized way of checking for prime numbers:
bool is_prime (long x) {
	
  std::this_thread::sleep_for( std::chrono::milliseconds(100) );
  for (long i=2; i<x; ++i) {
	  if (x%i==0) 
		  return false;
  }
  return true;
}

int main ()
{
  // call function asynchronously:
  std::shared_future<bool> fut = std::async (is_prime,4444444444444444443); 

  // do something while waiting for function to set future:
  std::cout << "checking, please wait";
  std::chrono::milliseconds span (10);
  while (fut.wait_for(span)!=std::future_status::ready)
    std::cout << '.';

  bool x = fut.get();     // retrieve return value

  std::cout << "\n4444444444444444443 " << (x?"is":"is not") << " prime.\n";

  std::shared_future<bool> fut1 = fut;
  bool x1 = fut1.get();     // retrieve return value

  std::cout << "\n4444444444444444443 " << (x1?"is":"is not") << " prime.\n";


  return 0;
}
