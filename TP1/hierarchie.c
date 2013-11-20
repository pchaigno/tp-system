#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>


pid_t pidFils;
int anc[4];

main(){
  
  int level = 0;
  int pere;
  
  while(level < 4) {
  	pere = getpid();
  	anc[level] = pere;
  	pidFils = fork();
 		if (pidFils!=0){
   		pidFils = fork();
	 		if (pidFils!=0){
		  	break;
		  } 
  	}
		level++;
  }

		printf("je suis le fils du level %d no %d / parents : ", level, getpid());
		
		if (level == 4) anc[3] = getpid();
		
    int j;
    for (j = 0; j < level; j++) {
    	printf("%d ", anc[j]);
    }
    printf("\n");
    
		sleep(1);
  	
}
