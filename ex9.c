#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{
//system has a unique process ID number, known as the pid
   pid_t pid;
   int status;

   /* Get and print my own pid, then fork
      and check for errors */
      
      //getpid()  returns the process ID of the calling process
   printf("My PID is %d\n", getpid());
   
   // fork() is used to create new  processes,which becomes the child process.  takes no arguments and returns a process ID. 
   if ( (pid = fork()) == -1 )
   {
       // perror() print a descriptive error message to  stderr
      perror("Can't fork");
      return 1;
   }
   if (pid == 0)
   {
      /* If pid is 0 we are in the child process,
         from here we execute 'man ls' */
         
         // execl()  replaces the current process image with a new process image specified by path.
      if ( execl("/usr/bin/man", "man", "ls", 
         (char*)NULL) == -1 )
      {
         perror("Can't exec");
         return 1;
      }
   }
   else if(pid > 0)
   {
      /* In the parent we must wait for the child
         to exit with waitpid(). Afterward, the
         child exit status is written to 'status' */
         
         //waitpid(() Suspends the calling process until a child process ends or is stopped.
      waitpid(pid, &status, 0);
      printf("Child executed with PID %d\n", pid);
      printf("Its return status was %d\n", status);
   }
   else
   {
      fprintf(stderr, "Something went wrong "
         "forking\n");
      return 1;
   }
   return 0;
}


/*
    execl():
        execl() is a system call in Unix-like operating systems that loads and executes a program. It replaces the current process image with a new process image.
        The arguments to execl() are:
            The path to the executable file you want to run (in this case, "/usr/bin/man").
            A list of arguments passed to the new program. The first argument is traditionally the name of the program, followed by any arguments you want to pass to the program.
            The list of arguments must end with (char*)NULL to signal the end of the arguments.

    "/usr/bin/man":
        This is the full path to the man program, which is used to display manual pages for various commands in Unix-like systems. It’s located in the /usr/bin/ directory.

    "man":
        This is the first argument passed to the man program. It tells the man program that it should display a manual page.

    "ls":
        This is the second argument to the man program. It specifies that the man program should display the manual page for the ls command (which lists directory contents).

    (char*)NULL:
        This marks the end of the argument list for the execl() call. execl() requires a NULL pointer to signal the end of the arguments.

    == -1:
        After calling execl(), it will not return unless there's an error, because it replaces the current process image with the new one.
        If execl() fails (e.g., if the specified executable cannot be found or if there’s some issue with the execution), it will return -1.
        The if condition checks whether execl() returned -1, which indicates an error occurred while trying to execute the man command.

What Happens:

    The code attempts to run the man command to display the manual page for the ls command.
    If execl() fails (if it returns -1), the program will enter the if block, indicating that an error occurred while trying to execute the man command.

Example Error Handling:

You might add error handling after the if condition, like this:

if (execl("/usr/bin/man", "man", "ls", (char*)NULL) == -1) {
    perror("execl failed");
    exit(1);
}

This would print an error message if execl() fails.
*/
