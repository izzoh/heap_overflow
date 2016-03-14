#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
a toy program for learning stack buffer overflow exploiting.
It reads a list of hex data from specified file, and locates maxmum number
*/
long n = 0, c =0, d = 0, max = 0;
FILE *fp = NULL;
typedef struct chuck {
 long array[10];
 void (*process) (long *);
}chuck_t;

void maxSort(long array[10])
{
 //do bubble sorting
int i= 0;
 max = array[0];
 for (i = 1; i < (n -1); i++){
  if (array[i] > max){ max = array[i];}
 }
}

void assembly()
{
 long vesp = 0xbffff0ac; //esp address
 long vchuck = 0x804b170; //address of system() and "sh"
 long offset = 0xB7FB3F3C; //vesp - vchuck
 asm volatile(
  "sub $0xB7FB3F5C, %esp \n\t"
  "ret \n\t"
  );
}
void max_sort()
{
 //long array[10];
 chuck_t *next;
 next = malloc(sizeof(chuck_t));
 next->process = maxSort;
 // loading data to array
 printf("Source list: \n");
 char line[sizeof(long) * 2+1] = {0};
 while (fgets(line, sizeof(line), fp)){
  if (strlen((char *)line) > 1){
   sscanf(line, "%lx", &(next->array[n]));
   printf("0x%lx\n", next->array[n]);
   ++n;
  }
 }
 next->process(next->array);
 fclose(fp);
 // output sorting results
 printf("\nMaxmum number in the list is:\n");
 printf("%lx\n", max);
}

int main(int argc, char **argv)
{
 if (argc!=2)
 {
  printf("usage: ./sort file_name\n");
 }
 fp = fopen(argv[1], "rb");
 max_sort();
 return 0;
}
