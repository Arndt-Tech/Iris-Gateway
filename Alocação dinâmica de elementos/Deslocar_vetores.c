#include <stdio.h>
#include <stdlib.h>

#define max 10
#define cursor 7

int vector[max] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

int main()
{
  register int i, j;
  
  for (i = 0; i < max; i++)printf("[%d] -> %d\n", i, vector[i]);             // Printa vetor
  putchar('\n');
  
  for (i = max*2; i > max; i--)vector[((2*max)-i)+cursor] = vector[(((2*max)+1)-i)+cursor];  // Desloca vetor
  putchar('\n');
  
  for (i = 0; i < max; i++)printf("[%d] -> %d\n", i, vector[i]);             // Printa vetor
  putchar('\n');
  
  system("PAUSE");
  exit(1);
}

