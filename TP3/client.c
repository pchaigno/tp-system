#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	int fserver, fclient;
	char buf[80];
	int nb;
	
	if(mkfifo("tmp/fifoClient", 0666) == -1) {
		perror("creation fifo client");
		exit(0);
	}
	
	if(!(fserver = open("tmp/fifoServer", O_RDONLY))) {
		perror("open fifo server");
		exit(1);
	}
	write(fserver, "1:tmp/fifoClient", 17);
	close(fserver);
	
	if(!(fclient = open("tmp/fifoClient", O_RDONLY))) {
		perror("open fifo client");
		exit(1);
	}
	nb = read(fclient, buf, 80);
	if(nb > 0) {
		printf("%s\n", buf);
	}
	close(fclient);
	
	return 0;
}
