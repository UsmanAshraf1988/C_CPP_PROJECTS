//! @file shm_header.h
//!
//!
//! @brief It is memory struct layout for shared memory IPC.
//! 
//!
//! @author Usman Ashraf 
//! @email  u.ashraf1988@gmail.com/usman.ashrafpak@gmail.com
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
//! Compiled with GCC Version > 4.5


//! system includes
#include <semaphore.h>

#define BUF_SIZE 1024

const char shm_name[] = "/shm_mem";

struct Shm_Mem {
  sem_t wrSem;
  sem_t rdSem;
  int cnt;
  char buf[BUF_SIZE];
};
