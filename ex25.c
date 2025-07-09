//Using Raw syscall() for time()

#include<stdio.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<time.h>
int main(void){
	time_t t;
	t = syscall(SYS_time, NULL);
	if(t == -1){
		perror("syscall time");
		return 1;
	}
	printf("current UNIX time(epoch):%ld\n", t);
	return 0;
}

/*
| Line                       | Description                                       |
| -------------------------- | ------------------------------------------------- |
| `#include <sys/syscall.h>` | Defines system call numbers like `SYS_time`       |
| `syscall(SYS_time, NULL)`  | Directly invokes the `time()` system call         |
| `time_t`                   | Stores seconds since the UNIX epoch (Jan 1, 1970) |
| `printf(...)`              | Prints the raw epoch time                         |


*/
