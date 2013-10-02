#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

char* service1() {
	return "Service 1";
}

char* service2() {
	return "Service 2";
}

int main() {
	int nb;
	int fserver, fclient;
	int serviceId;
	char buf[80];
	char* reponse;
	char* filepath;

	if(mkfifo("tmp/fifoServer", 0666) == -1) {
		perror("creation fifo server");
		exit(0);
	}

	while(1) {
		if(!(fserver = open("tmp/fifoServer", O_RDONLY))) {
			perror("open fifo server");
			exit(1);
		}
		
		nb = read(fserver, buf, 80);
		if(nb > 0) {
			serviceId = buf[0];
			//filepath = getFilePath(buf);
			filepath = "fifoClient";
			switch(serviceId) {
				case 1:
					reponse = service1();
					break;
				case 2:
					reponse = service2();
					break;
				default:
					perror("service id incorrect");
					exit(1);
			}
			
			if(!(fclient = open(filepath, O_RDONLY))) {
				perror("open fifo client");
				exit(1);
			}
			write(fclient, reponse, 80);
			close(fclient);
		}
		close(fserver);
	}
}
/*
char* getFilePath(char line[]) {
	int length = strlen(line)-2;
	char reponse[length];
	int i;
	for(i=0; i<length; i++) {
		reponse[i] = line[i+2];
	}
	return reponse;
}*/
