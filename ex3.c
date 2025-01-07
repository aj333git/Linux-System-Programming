#include<stdio.h>
#include<stdlib.h>
int main()
{
char environment_name[50];
printf("Enter the Environment name:");
scanf("%s", environment_name);
//The getenv() function returns a pointer to the c-string containing the value of the environment variable 
// The getenv()  environment variables search at runtime, and returns a pointer to the variable's value
//if no environment variable is found, it returns a null pointer.
printf("Environment value: %s\n", getenv(environment_name));
return 0;

}

/*
Enter the Environment name:HOME
Environment value: /home/oem
Enter the Environment name:PATH
*/
/*
Environment variables are system-wide variables and  available to all processes .May contain information about the paths of certain executable files, the home directory, or the TEMP directory
*/


