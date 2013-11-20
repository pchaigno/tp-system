#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <setjmp.h>


int i = 0;
int j = 0;
jmp_buf env;
struct sigaction action;

void handler(int sig){

  printf("div 0\n");
  i=1;
  j=1;
  //  longjmp(env,0);  // A decommenter
}

int main (int argc, char * argv[])
{
  
  

  //Définition du handler
  action.sa_handler=handler;
  signal(SIGFPE,(*handler));
  //  setjmp(env);    // A decommenter
  printf("deb i=%d j=%d\n", i ,j);
  j = 12/i;
  printf("fin i=%d j=%d\n", i, j);
  exit(0);
}
