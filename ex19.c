#include<stdio.h>
//Provides exit() and perror() functions.
#include<stdlib.h>
//Provides sbrk() function for manual heap control.
#include<unistd.h>
//Provides signal types and signal() function.
#include<signal.h>
//Pointer to current memory address
char *cp;

//Counter to track how many times the signal handler (catcher) was called.
int callno = 0;
// Stop after 10 expansions
//Limit how many times we expand memory after a SIGSEGV.
//Prevents infinite memory growth and crashes.
int max_calls = 10; 
//Function called automatically when a SIGSEGV is caught. 
void catcher(int signo){
	callno++;
	printf("caught signal %d,call #%d, at address %p\n", signo, callno,(void *)cp);
	if (callno >= max_calls) {
        printf("Max calls reached. Exiting program.\n");
        exit(0);
    }
    // Extend the heap by 256 bytes
    //Try to expand the heap by 256 bytes.
    //If it fails (returns -1), something went wrong.
	if (sbrk(256) == (void *)-1){
		perror("sbrk failed");
		//Print system error and exit with failure.
		exit(1);}
		   // Reinstall signal handler
	signal(SIGSEGV,catcher);
}
// No command-line arguments.
int main(void){
 // Register SIGSEGV handler
 //Re-register the signal handler.
	signal(SIGSEGV,catcher);
	 // Get current heap break address
	 //Get the current program break (end of heap).

        //Store it in cp to begin writing from there.
	cp = (char *)sbrk(0);
	printf("original break value:%p\n", (void *)cp);
	    // Keep writing beyond heap to trigger SIGSEGV
		for(;;)
			*cp++ = 1;
	return 0;
	
	
}
/*

output:-

original break value:0x55e4eff7d000
caught signal 11,call #1, at address 0x55e4eff9e001
caught signal 11,call #2, at address 0x55e4eff9f001
caught signal 11,call #3, at address 0x55e4eff9f001
caught signal 11,call #4, at address 0x55e4eff9f001
caught signal 11,call #5, at address 0x55e4eff9f001
caught signal 11,call #6, at address 0x55e4eff9f001
caught signal 11,call #7, at address 0x55e4eff9f001
caught signal 11,call #8, at address 0x55e4eff9f001
caught signal 11,call #9, at address 0x55e4eff9f001
caught signal 11,call #10, at address 0x55e4eff9f001
Max calls reached. Exiting program.



| Feature                    | Benefit                                |
| -------------------------- | -------------------------------------- |
| `max_calls`                | Prevents infinite loop                 |
| Graceful `exit(0)`         | Clean termination                      |
| `perror("sbrk failed")`    | Shows system error if allocation fails |
| `signal(SIGSEGV, catcher)` | Reinstalls handler (POSIX-safe)        |



| Component   | Role                                            |
| ----------- | ----------------------------------------------- |
| `cp`        | Points to the edge of heap, gets written to     |
| `SIGSEGV`   | Triggered when writing beyond allocated memory  |
| `catcher()` | Catches signal, expands heap, resumes execution |
| `max_calls` | Safety limit to prevent infinite allocation     |
| `sbrk(256)` | Adds 256 bytes to heap on each signal           |
| `exit()`    | Gracefully ends the program                     |
*/



