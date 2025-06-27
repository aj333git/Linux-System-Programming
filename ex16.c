#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

void handle_sigint(int sig){
	char choice;
	printf("\ncaught signal%d(ctrl+c)\n",sig);
	printf("do you want to exit(y/n)");
	scanf("%c", &choice);

	if(choice == 'y' || choice == 'Y'){
		printf("exiting...\n");
			exit(0);
		}
	else {
		printf("continuing ...\n");
	     }
    }
int main() {
//signal()	Set a custom handler for Ctrl+C (SIGINT)
	signal(SIGINT,handle_sigint);
	while(1);
	printf("running...press ctrl+c to interrupt\n");
	sleep(2);
}

