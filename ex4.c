#include<stdio.h>
int main(){
//Environment variables are in the global variable environ
//This points to an array of pointers to strings.
//To access the variable, we declare it with extern. We can then iterate over it like a normal array.
extern char **environ;
char **environment_list = environ;
while(*environment_list != NULL){
	printf("%s\n", *environment_list);
	environment_list++;
	}
	printf("\n");
	printf("fun again");
	for(int i=0;i<=9;i++)
	printf("\n");
	//size_t is an unsigned integer data type
	//it is used to represent the size of objects in bytes 
	//The size_t data type is never negative.
	//size_t can be used as loop variable,size of arrays, memory blocks, and strings
	for (size_t i = 0; environ[i] != NULL; i++) {
    printf("%s\n", environ[i]);
  }
	return 0;
}



