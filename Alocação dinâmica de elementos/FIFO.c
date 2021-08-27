// Inclusões
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Definições
#define MAX_STATIONS 10



// Structs
typedef struct 
{
  char  *data;
  int   *last;	
  int  *first;
  int last_removed;
  int amount;
}elements;


// Protótipo de funções
void init_elements(elements *r);
elements *alloc_data(elements *r, char* data);
elements *remove_station(elements *addr, elements *main_element);
elements **alloc_array(int num0);
void print_array(elements *view);


// Variáveis globais
elements teste;
elements **alloc_addr = NULL;
elements **ptrStation = NULL;
elements **anchor     = NULL;

char string[] = "";


// Desenvolvimento de funções
int main(int argc, char *argv[])
{
  register int i, j;
  int remove = 0;
  
  init_elements(&teste);
  alloc_addr = alloc_array(MAX_STATIONS);
  
  ptrStation = alloc_addr;
  anchor     = alloc_addr;
  
  while(1)
  {
  	printf("1 - Adicionar estacao\n");
  	printf("2 - Remover estacao\n");
  	printf("3 - Visualizar estacoes\n");
  	printf("9 - Visualizar ponteiros\n");
  	printf("ESC - Sair\n");
  	
  	char cmd = getch();
  	
  	switch (cmd)
  	{
  	  case '1':
  	  	alloc_addr[teste.amount] = alloc_data(&teste, "TESTANDO _ + de_10");
		system("PAUSE");
  	  break;
  	  
  	  
  	  case '2':
  	  	printf("Estacao que deseja remover:\n");
  	  	remove = (getch()-48);
  	  	
		remove_station(alloc_addr[remove], &teste);
  	  	system("PAUSE");
  	  break;
  	  
  	  
  	  case '3':
  	  	for (i = 0; i < teste.amount; i++)print_array(alloc_addr[i]);
  	  	system("PAUSE");
  	  break;
  	  
  	  
  	  case '9':
  	  	putchar('\n');
  	  	
		printf("ptrStation: 0x%x\n", ptrStation);
  	  	printf("Value ptrStation: 0x%x\n", *ptrStation);
		putchar('\n');
		
		printf("anchor: 0x%x\n"      , anchor);
  	  	printf("Value anchor: 0x%x\n", *anchor);
		putchar('\n');
		
		printf("alloc_addr: 0x%x\n", alloc_addr);
		putchar('\n');	
		
  	  	
		printf("sizeof elements: %d byte\n", sizeof(elements));
		putchar('\n');
  	  	
		for (i = 0; i < MAX_STATIONS; i++)printf("Value alloc_addr[%d]: 0x%x\n", i, alloc_addr[i]);
		putchar('\n');
		
		for (i = 0; i < MAX_STATIONS; i++)printf("ADDR alloc_addr[%d]: 0x%x\n", i, &alloc_addr[i]);
  	  	putchar('\n');
		system("PAUSE");
  	  break;
  	  
  	  
  	  case 0x1B: 
  	    free(alloc_addr);
  	    exit(0);
  	  break;
	}
    fflush(stdin);
    system("CLS");
  }
  system("PAUSE");
  return 0;
}



//============================================================================================================//
void init_elements(elements *r)
{
  r->first   =   NULL;
  r->amount  =      0;
  r->last_removed = 0;
}



//============================================================================================================//
elements *alloc_data(elements *r, char *data)
{
  elements *new_element = NULL;
  int dataLen = strlen(data);
  dataLen += 1;
  if((new_element = (elements*)calloc(1, sizeof(elements))) == NULL) return NULL;
  if((new_element->data = (char*)calloc(dataLen, sizeof(char))) == NULL) return NULL;
  strcpy(new_element->data, data);
  r->amount += 1;
  
  
  printf("ADDR new_element: 0x%x\n", new_element);
  printf("ADDR new_element->data: 0x%x\n", &new_element->data);
  return new_element;
}



//============================================================================================================//
void print_array(elements *view)
{
  register int i = 0;	
  printf ("Dados escritos em ADDR -> 0x%x:  ", view);
  while (view->data[i] != 0)
  {
  	printf ("%c", view->data[i]);
    i++;
  }
  putchar('\n');
  putchar('\n');
}



//============================================================================================================//
elements **alloc_array(int num0)
{
  elements **ptr = NULL;
  if (num0 < 1) return NULL;
    if ((ptr = (elements**)calloc(num0, sizeof(elements*))) == NULL)return NULL;
  return ptr;
}



//============================================================================================================//
elements *remove_station(elements *addr, elements *main_element)
{
  //*ptrStation = addr;
  register int i = 0;
  if (addr == NULL) return NULL;
  free(addr);
  //main_element->amount -= 1;
  putchar('\n');
  printf("Memory successfully released!\n");
  return NULL;
}

