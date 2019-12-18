//! @file shm_writer_process.c
//!
//!
//! @brief It is one end process of posix shared memory for IPC.
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

//! For debug messages, Please define this macro here.
//! Or, at compile time with macro flags
//! #define PROTOTYPE 1 or -DPROTOTYPE=1 compilaiton flag.

#ifdef PROTOTYPE
#define PUT_LOG(msg) if(PROTOTYPE)fprintf(stdout,"%s",msg);
#else
#define PUT_LOG(msg)
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "shm_header.h"


int main(int argc, char * argv []){

  int fd_shm = shm_open(shm_name, O_CREAT|O_EXCL|O_RDWR, 0666);
  if (fd_shm<0){
    shm_unlink(shm_name);
    fprintf(stderr, "shm_open:unsuccessful: \'%s\' ...\n", shm_name);
    exit(1);
  }

  int ret_set_size = ftruncate(fd_shm, sizeof(struct Shm_Mem));
  if (ret_set_size<0){
    shm_unlink(shm_name);
    close(fd_shm);
    fprintf(stderr, "ftruncate: Resetting size of shm_mem discriptor: %d is unsuccessful...\n", fd_shm);
    exit(1);
  }

  
  struct  Shm_Mem * shm_mem = (struct Shm_Mem *) mmap ( NULL, sizeof(struct Shm_Mem), PROT_READ|PROT_WRITE,
                                MAP_SHARED, fd_shm, 0 );
  if( shm_mem == ((void*)-1) ){
    munmap(shm_mem, sizeof(struct Shm_Mem)); 
    shm_unlink(shm_name);
    close(fd_shm);
    fprintf(stderr,"mmap: for posix shared memory unsuccessful...\n");
    exit(1);
  }

  memset( (void *)shm_mem, 0,  sizeof(struct Shm_Mem));
  sem_init( &(shm_mem->wrSem), 1, 1);
  sem_init( &(shm_mem->rdSem), 1, 0);

  while(1){

    sem_wait( &(shm_mem->wrSem) );

    shm_mem->cnt = 0;
    memset( (void *)( &(shm_mem->buf)), '\0',  BUF_SIZE);

    printf("Writer process, please write something or press enter to finish.\n");
    shm_mem->cnt = read(STDIN_FILENO, shm_mem->buf, BUF_SIZE);

    printf("From Writer: %d characters are written, and string is: \'%s\'\n", shm_mem->cnt, shm_mem->buf);

    sem_post( &(shm_mem->rdSem) );

    if( shm_mem->cnt <= 1 ){
      break;
    }
  }

  //Do cleanup
  munmap( shm_mem, sizeof(struct Shm_Mem) );
  shm_unlink( shm_name );
  close( fd_shm ); 



  return 0;
}
