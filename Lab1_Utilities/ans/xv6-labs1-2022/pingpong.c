#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int P2C[2];               //the pipe that parent send byte to child
  int C2P[2];               //the pipe that child send byte to parent
  char buffer[1]={'1'};     		  //one byte
  
  pipe(P2C);
  pipe(C2P);
    
  if(fork()==0)             //child process
  {
    close(P2C[1]);
    close(C2P[0]);
    read(P2C[0],buffer,1);
    close(P2C[0]);
	printf(1,"%d:received ping\n",getpid());
    write(C2P[1],buffer,1);
    close(C2P[1]);
    exit();
  }
  else                      //parent process
  { 
    close(C2P[1]);
    close(P2C[0]);
	write(P2C[1],buffer,1);
	close(P2C[1]);
    read(C2P[0],buffer,1);
    close(C2P[0]);
    printf(1,"%d:received pong\n",getpid()); 
    wait(); 
    exit();
  }
}

