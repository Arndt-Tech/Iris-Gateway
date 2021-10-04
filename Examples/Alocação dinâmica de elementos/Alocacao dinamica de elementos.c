//==================================================================================//
// Inclusões 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//==================================================================================//
// Definições
#define MAX_STATION 10


//==================================================================================//
// Struct's
typedef struct 
{
  char   *data;
  int   amount;	
  int last_removed;
}element;


//==================================================================================//
// Protótipo de funções 
void start_element(element *r);
element *alloc_element(char *data);
element **alloc_addr_array(int num0);

int new_station(element *r, element **allocator, char *data);
element *remove_station(element *addr);

void print_array(element *view);
void debug();

void menu();


//==================================================================================//
// Variáveis globais
element station;
char string[20];


//==================================================================================//
// Ponteiros
element **alloc_addr     = NULL;
element **ptr_element    = NULL;
element **anchor_element = NULL;


//==================================================================================//
// Desenvolvimento de funções

// main
int main(int agrc, char *argv[])
{
  start_element(&station);                          // Inicializo elemento
  alloc_addr = alloc_addr_array(MAX_STATION);       // Cria array para alocar endereços de elementos
  
  anchor_element = alloc_addr;
  ptr_element    = alloc_addr;
  
  while(1)
  {
    register int i;
    printf ("1 - Adicionar nova estacao\n");
    printf ("2 - Remover estacao\n");
    printf ("3 - Verificar enderecos\n");
    printf ("4 - Debug\n");
    printf ("8 - Visualizar enderecos\n");
    printf ("9 - Reset\n");
    printf ("ESC - Quit\n");
    putchar('\n');

    int cmd = getch ();
	  
    switch (cmd)
    {
      case '1':
		printf("Digite alguma coisa para alocar: ");
  	    gets(string);
  	    putchar('\n'); 
		
		//print_array(alloc_element(string));
		
  	    alloc_addr[station.amount] = alloc_element(string);
  	    station.amount += 1;
		system("PAUSE");
      break;
    
    
      case '2':
        system("PAUSE");
      break;
    
    
      case '3':
        for (i = 0; i < station.amount; i++)print_array(alloc_addr[i]);
        system("PAUSE");
      break;
    
    
      case '4':
        debug();
        system("PAUSE");
      break;
    
    
	  case 0x1B:
	  	free(alloc_addr);
        exit (0);
      break;
    }
    fflush(stdin);
    system("CLS");
  }
  return 0;
}



//==================================================================================//
// start_element()
void start_element(element *r)
{
  r->last_removed = 0;
  r->amount = 0;
  r->data   = NULL;
}



//==================================================================================//
// alloc_element()
element *alloc_element(char *data)
{
  element *new_element = NULL;
  int dataLen = strlen(data);
  dataLen += 1;
  if((new_element = (element*)calloc(1, sizeof(element))) == NULL) return NULL;
  if((new_element->data = (char*)calloc(dataLen, sizeof(char))) == NULL) return NULL;
  strcpy(new_element->data, data);

  printf("Data allocated in ADDR: 0x%x\n", &new_element->data);
  return new_element;
}



//============================================================================================================//
// alloc_addr_array
element **alloc_addr_array(int num0)
{
  element **ptr = NULL;
  if (num0 < 1) return NULL;
  if ((ptr = (element**)calloc(num0, sizeof(element*))) == NULL)return NULL;
  return ptr;
}



//============================================================================================================//
// new_station()
int new_station(element *r, element **allocator, char *data)
{
  if(r->amount >= MAX_STATION) return 0;
  else if (r->amount < MAX_STATION)
  {
  	allocator[r->amount] = alloc_element(data);
  	r->amount += 1;
    printf("Station added!\n");
    printf("Total stations: %d\n", r->amount);
    return 1;
  }
}



//============================================================================================================//
// remove_station()
element *remove_station(element *addr)
{
  register int i = 0;
  if (addr == NULL) return NULL;
  free(addr);
  putchar('\n');
  printf("Memory successfully released!\n");
  return NULL;
}



//============================================================================================================//
// print_array()
void print_array(element *view)
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
// debug()
void debug()
{
  register int i;
	
  printf("station->amount: %d", station.amount);
  putchar('\n');
  	  	
  putchar('\n');
  printf("ptrStation: 0x%x\n", ptr_element);
  printf("Value ptrStation: 0x%x\n", *ptr_element);
  
  putchar('\n');		
  printf("anchor: 0x%x\n"      , anchor_element);
  printf("Value anchor: 0x%x\n", *anchor_element);
  
  putchar('\n');		
  printf("alloc_addr: 0x%x\n", alloc_addr);
  	
  putchar('\n');			  	
  printf("sizeof elements: %d byte\n", sizeof(element));

  putchar('\n');  	  	
  for (i = 0; i < MAX_STATION; i++)printf("Value alloc_addr[%d]: 0x%x\n", i, alloc_addr[i]);
  
  putchar('\n');
  for (i = 0; i < MAX_STATION; i++)printf("ADDR alloc_addr[%d]: 0x%x\n", i, &alloc_addr[i]);
  
  putchar('\n');
}
