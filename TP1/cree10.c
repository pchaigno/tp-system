#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>


pid_t pidFils;  

main(){
  int i;
  for(i=0; i<10; i++) {
  	pidFils = fork();
  	if(pidFils==0) {
  		break;
  	}
  }
  if(pidFils!=0) {
		exit(1);
	}
  for(i=0; i<5; i++) {
  	printf("je suis le fils %d\n", getpid());
  	sleep(1);
  }

}
