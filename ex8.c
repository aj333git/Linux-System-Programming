
//#define_XOPEN_SOURCE500

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
void printthis(char argname[]);
int main(int argc, char *argv[])
{
int k;
int add,option;
if(argc == 1)
{
 printthis(argv[0]);
 return 1;
}


while((option = getopt(argc,argv, "amh")) != -1)
{
  switch(option)
  {
   case 'a':
       add = 0;
       for(k=2; k<argc;k++)
           add = add+atoi(argv[k]);
           
           break;
    case 'm':
        add = 1;
        for(k=2;k<argc;k++)
            add = add*atoi(argv[k]);
           
            break;
     case 'h':
          printthis(argv[0]);
          return 0;
     default:
           printthis(argv[0]);
           return 1;
       }
  }
  
    printf("total: %i\n", add);
    return 0;

}
void printthis(char argname[])
{
 printf("%s [-s] [-m] integer...\n", argname);
// printf("this is add program\n", argname);

}



//Credit:-Jack-Banny Persson for book->Linux System Programming Techniques
