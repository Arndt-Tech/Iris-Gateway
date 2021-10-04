#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_alloc.h"

// Variáveis globais
element teste;
char string[] = "";

int main()
{
  register int i;
  int cmd;	
  
  start_element(&teste);
  
  while(1)
  {
  	system("CLS");
    
    printf("amount: %d\n", teste.amount);
    printf("first_addr: 0x%x\n", teste.first_addr);
    printf("last_addr: 0x%x\n" , teste.last_addr);
    printf("Sizeof(element): %d\n" , sizeof(element));
    
    putchar('\n');
    for (i = 0; i < teste.amount; i++)print_addr_data(teste.addr_bank[i]);
    
    putchar('\n');
	for (i = 0; i < teste.amount; i++)printf("teste.addr_bank[%d]: 0x%x  -->  0x%x\n", i, &teste.addr_bank[i], teste.addr_bank[i]);
    
    
    cmd = getch();
	if (cmd == 0x1B)break;
	else if(cmd == '1')
	{
	  printf("Digite algo para alocar: ");
	  gets(string);
	  putchar('\n');
	  alloc_element(&teste, string);
	}
    else if(cmd == '2')remove_element(&teste);
    else if(cmd == '3')
	{
	  printf("Selecione qual elemento deseja remover\n");
	  spec_removal(&teste, (getch()-48));
	}
	else if(cmd == '0')
	{
	  free_all(&teste);
	  system("PAUSE");
	}
  }
  free_all(&teste);
  exit(1);
}
