//! @file client_side_socket.c
//!
//!
//! @brief It is client end process of socket IPC.
//! 
//!
//! @ Author: Usman Ashraf 
//! @ email:  u.ashraf1988@gmail.com
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
#define PUT_LOG(msg) if(PROTOTYPE)std::cout<<msg;
#else
#define PUT_LOG(msg)
#endif

#define SOCKET_PATH     "/tmp/mysocket"
#define BUFFER_CAPACITY 256


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


static void error(const char * msg){
	perror(msg);
	unlink(SOCKET_PATH);
	exit(1);
}



int main(int argc, char * argv [] )
{
    // socket client side.
    int msg_counter=0;
    int sock_fd;
    struct sockaddr_un sockAddr_unCli;
    char buff[BUFFER_CAPACITY] = {0}; 

    // create socket.
    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        error("client side socket creation problem....\n");
    }

    memset( &sockAddr_unCli, 0, sizeof(sockAddr_unCli));
    sockAddr_unCli.sun_family = AF_UNIX;
    strcpy(sockAddr_unCli.sun_path, SOCKET_PATH);
    sockAddr_unCli.sun_path[strlen(SOCKET_PATH)]='\0';

    // connect socket.
    if (connect(sock_fd, (struct sockaddr*)&sockAddr_unCli, sizeof(struct sockaddr_un)) < 0) {
	close(sock_fd);
        perror("client side connect problem....");
    }


    // Read write.
    int str_len = 0;
    char fix_part_msg [] = "*******Msg from Client to server......"; 
    char clintMsg [BUFFER_CAPACITY] = {0};
    int clintMsg_comp_len = 0;
    while(1){
      sleep(1);
      memset( buff, '\0',  BUFFER_CAPACITY);
      memset( clintMsg, '\0',  BUFFER_CAPACITY);

      sprintf(clintMsg, "id: %d ==> payload: %s",msg_counter++, fix_part_msg );
      clintMsg_comp_len = strlen(clintMsg)+1;
      if ( (str_len = write(sock_fd, clintMsg, clintMsg_comp_len)) <= 1 ){
	      break;
      }

      printf("I am client\n");

      if ( (str_len = read(sock_fd, buff, BUFFER_CAPACITY)) <= 1 ){
	      break;
      }
      printf("Received %d characters, and Msg is: \"%s.\"\n", str_len, buff);

    }

    unlink(SOCKET_PATH);
    close(sock_fd);
    exit(0);
}
