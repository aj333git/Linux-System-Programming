#include<stdio.h>

#include<signal.h>
#include<unistd.h>

void handle_alarm(int sig){
	printf("clock interrupt (SIGALRM recived\n)");
}
          int main() {
	signal(SIGALRM, handle_alarm);
	alarm(1);
	while(1){
		pause();
		alarm(1);
	}
	return 0;
	  }
	  /*
	  
	  
	  | Line                            | Meaning                                    |
| ------------------------------- | ------------------------------------------ |
| `signal(SIGALRM, handle_alarm)` | Sets up `handle_alarm()` as signal handler |
| `alarm(1)`                      | Triggers SIGALRM after 1 second            |
| `pause()`                       | Halts until a signal is received           |
| `alarm(1)` in loop              | Reschedules the next “tick”                |


command->gcc ex26.c -o ex26
./ex26.c

o/p
./ex26
clock interrupt (SIGALRM recived
)clock interrupt (SIGALRM recived
)clock interrupt (SIGALRM recived
)clock interrupt (SIGALRM recived
)clock interrupt (SIGALRM recived
)clock interrupt (SIGALRM recived
)clock interrupt (SIGALRM recived
)clock interrupt (SIGALRM recived
................................
ctrl+c

*/
