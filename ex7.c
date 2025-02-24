#include<stdio.h>
#include<stdlib.h>
void printthis(char argname[]);
int main(int argc, char *argv[])
{
int i;
int add = 0;
if(argc == 1)
{
 printthis(argv[0]);
 return 1;
}
for(i=1;i<argc;i++)
{
 add=add+atoi(argv[i]);
}

printf("Total add : %i\n",add);
return 0;

}
void printthis(char argname[])
{
 printf("%s integer...\n", argname);
 printf("this is add program\n");

}


//Credit:-Jack-Banny Persson for book->Linux System Programming Techniques


