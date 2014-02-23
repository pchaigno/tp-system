#include <unistd.h>
#include <stdio.h>

int main() {
	int fp[2];
	pipe(fp);
	int test = fork();
	if(test != 0) {
		close(fp[0]);
		execlp("who", "who", NULL);
		dup2(fp[1], 1);
		close(fp[1]);
	} else {
		close(fp[1]);
		char *user;
		dup2(fp[0], 0);
		scanf("%s", &user);
		execlp("grep", "grep", user, NULL);
		close(fp[0]);
	}
}
