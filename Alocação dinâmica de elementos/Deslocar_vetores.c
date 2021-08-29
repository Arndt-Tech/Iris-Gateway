#include <stdio.h>
#include <stdlib.h>

#define max 10
#define cursor 7

void printVetor();
void delay(void);


int vector[max] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int cnt = 0;

int main()
{
  register int i;
  printVetor();
  
  for (i = cursor; i < max; i++)vector[i] = vector[i+1];
   
  printVetor();
  
  
  system("PAUSE");
  exit(1);
}



void printVetor()
{
  register int i;
  for (i = 0; i < max; i++)printf("[%d] -> %d\n", i, vector[i]);     // Printa vetor
  putchar('\n');
}


void delay(void)
{
  register int j;
  for (j = 0; j < 1E9; j++);
}
