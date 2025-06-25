
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
int main()
{
	int status;
	if (fork() == 0) {
	//execl() is one of the exec family of functions:
	//Defined in <unistd.h> | runs a new program
	/*
	
	int execl(const char *path, const char *arg0, ..., (char *)0);

	| Parameter         | Meaning                                                           |
| ----------------- | ----------------------------------------------------------------- |
| `path`            | Full path to the executable (e.g., `/bin/date`)   Load /bin/date (the actual program file).                |
| `arg0`            | First argument: conventionally the name of the program (`"date"`)|Pass it a single argument "date" (as argv[0]). |
| `arg1, arg2, ...` | Optional arguments to the new program                             |
| `NULL`            | Marks the **end** of the argument list (required!)  terminated by (char *)0              |

	it's equivalent to running this shell command:
	date
       /bin/date. (The new process) gets these:
	
	argv[0] = "date"
        argv[1] = NULL
        
        main of /bin/date
        
        int main(int argc, char *argv[]) {
    // argc == 1
    // argv[0] == "date"
    // argv[1] == NULL
}
🧪 If You Use Extra Arguments

execl("/bin/ls", "ls", "-l", "-a", (char *)0);
displays
ls -l -a
argv becomes:
argv[0] = "ls"
argv[1] = "-l"
argv[2] = "-a"
argv[3] = NULL


	*/
		execl("/bin/date", "date", (char*)0);
		perror("execl failed");
		return 1;
	}
	wait(&status);
	if(WIFEXITED(status)){
		printf("child exited with status :%d\n",WEXITSTATUS(status));
	}
	return 0;
}
/*
| Line                                     | Explanation                                                    |
| ---------------------------------------- | -------------------------------------------------------------- |
| `#include ...`                           | Includes system call and process control headers.              |
| `if (fork() == 0)`                       | Creates a child process. In the child (`== 0`), run `execl()`. |
| `execl("/bin/date", "date", (char *)0);` | Replace child with `/bin/date` (i.e., run the `date` command). |
| `perror("execl failed");`                | Prints error if `execl()` fails.                               |
| `wait(&status);`                         | Parent waits for child to finish.                              |
| `WIFEXITED/WEXITSTATUS`                  | Checks child terminated normally and prints its exit code.     |

o/p
~$ ./ex14
Wednesday 25 June 2025 02:29:50 PM IST
child exited with status :0


*/
