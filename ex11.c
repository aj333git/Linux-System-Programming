/*#include <signal.h>
//chapter signal
//Maurice J.Bach
main()
{
 register int i;
 setpgrp();
 for(i=0;i<10;i++)
 {
  if(fork()==0)
  {
    if(i&1)
	    setpgrp();
    printf("pid = %d pgrp = %d\n",getpid(), getpgrp());
    pause();
  }
 }
 kill(0,SIGINT);
}

*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main() {
    int i;
    setpgrp(); // Set current process group leader

    for (i = 0; i < 10; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0) { // Child process
            if (i & 1) {
                setpgrp(); // Put in new process group
            }

            printf("pid = %d pgrp = %d\n", getpid(), getpgrp());
            pause(); // Wait for signal
            exit(0); // Child exits after signal
        }
    }

    sleep(2); // Give time for all children to pause
    kill(0, SIGINT); // Send SIGINT to all processes in current process group
    return 0;
}

/*

| Problem                          | Fix                                                         |
| -------------------------------- | ----------------------------------------------------------- |
| `main()` lacks return type       | Changed to `int main()`                                     |
| `register int i;` is outdated    | Changed to `int i;`                                         |
| Missing headers                  | Added `<stdio.h>`, `<unistd.h>`, `<stdlib.h>`, `<signal.h>` |
| No `exit()` in child             | Ensured child exits after `pause()`                         |
| `pause()` may hang indefinitely  | Parent sends signal after `sleep()`                         |
| `kill(0, SIGINT);` without setup | Ensured it's sent to proper group with `setpgrp()`          |


setpgrp() sets the calling process's group ID to its PID.

Children either inherit the parent group or create a new one using setpgrp() if i is odd.

pause() causes each child to wait for a signal.

kill(0, SIGINT) sends SIGINT to all processes in the same process group as the caller.
*/

