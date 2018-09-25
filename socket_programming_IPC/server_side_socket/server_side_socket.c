//! @file server_side_socket.c
//!
//!
//! @brief It is server end process of socket IPC.
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
    // socket server side.
    static int msg_counter=0;
    int sock_fd, newConn_sock_fd;
    int len;
    struct sockaddr_un sockAddr_unSer, sockAddr_unCli;
    char buff[BUFFER_CAPACITY] = {0}; 


    // create socket.
    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        error("server side socket creation problem....\n");
    }

    memset( &sockAddr_unSer, 0, sizeof(struct sockaddr_un));
    sockAddr_unSer.sun_family = AF_UNIX;
    strcpy(sockAddr_unSer.sun_path, SOCKET_PATH);
    sockAddr_unSer.sun_path[strlen(SOCKET_PATH)]='\0';
    
    unlink(SOCKET_PATH);

    // bind socket to address filesystem path.
    if (bind(sock_fd, (struct sockaddr*)&sockAddr_unSer, sizeof(struct sockaddr_un)) < 0) {
        error("server side socket bind problem....\n");
    }


    // listen with queue connection limit of 5.
    if (listen(sock_fd, 1) < 0) {
        error("server side socket listen problem....\n");
    }

    // accept
    len = sizeof(sockAddr_unCli);
    if ((newConn_sock_fd = accept(sock_fd, (struct sockaddr*)&sockAddr_unCli, &len) ) < 0) {
        error("server side socket accept problem....\n");
    }

    // Read write
    int str_len = 0;
    char fix_part_msg [] = "*******Msg from server to client......";
    char servMsg [BUFFER_CAPACITY] = {0};
    int servMsg_comp_len = 0;
    while(1){
      
      printf("I am server\n");
      memset( buff, '\0',  BUFFER_CAPACITY);
      memset( servMsg, '\0',  BUFFER_CAPACITY);

      if ( (str_len = read(newConn_sock_fd, buff, BUFFER_CAPACITY)) <= 1 ){
	      break;
      }
      printf("Received %d characters, and Msg is: \"%s.\"\n", str_len, buff);

      sprintf(servMsg, "id: %d ==> payload: %s",msg_counter++, fix_part_msg );
      servMsg_comp_len = strlen(servMsg)+1;
      if ( (str_len = write(newConn_sock_fd, servMsg, servMsg_comp_len)) <= 1 ){
	      break;
      }
    }
    close(newConn_sock_fd);
    close(sock_fd);

    exit(0);
}
