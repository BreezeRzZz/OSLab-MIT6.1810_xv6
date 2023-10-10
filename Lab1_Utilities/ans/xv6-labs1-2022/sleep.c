#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc<=1)
  {
  	printf(2,"error:argument numbers not match\n");
  	exit();
  }
  
  int time=atoi(argv[1]);
  sleep(time);
  printf(1,"(nothing happens for a little while)\n");
  
  exit();
}
