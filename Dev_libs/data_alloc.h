#ifndef DATA_ALLOC_H
#define DATA_ALLOC_H
/* Como usar:
   
  |-------------| 
  | Inicializar |
  |-------------|
   * Declarar    -> element elemento;
   * Inicializar -> start_element(&elemento);
  
  |---------------------------| 
  | Criar ou remover elemento |
  |---------------------------|
   * Criar/Escrever     -> alloc_element (&elemento, string);    // Cria um novo elemento;
   * Remover            -> remove_element(&elemento);            // Remove sempre �ltimo elemento do banco;
                        -> spec_removal(&elemento, num);         // Remove um elemento especifico;
                        -> free_all(&elemento);                  // Remove todos endere�os alocados pelo elemento;
                        
  |----------------------------| 
  | Ler ou vizualizar elemento |
  |----------------------------| 
   * Ler -> print_addr_data(elemento::element);      // L� dados alocados em endere�os de tipo element e printa no console;
         -> 
          
  |------------------|  
  | Objetos leg�veis |
  |------------------| 
   * element::addr_bank             // Banco onde guarda endere�os dos elementos;
              first_addr            // Primeiro elemento do banco de endere�os;
              last_addr             // �ltimo elemento do banco de endere�os;
			  amount                // Quantidade de elementos alocados;
*/
//==================================================================================//
// Inclus�es 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//==================================================================================//
// Struct's
typedef struct elem_idnt
{
  char   *data;
  struct elem_idnt  *first_addr;
  struct elem_idnt  *last_addr;
  struct elem_idnt  **addr_bank;
  int    amount;
}element;


//==================================================================================//
// Prot�tipo de fun��es 
void start_element(element *r);

element *alloc_element(element *r, char *data);

element **alloc_addr_array(int num0);

element *remove_element(element *addr);

element *spec_removal(element *addr, unsigned int num);

element *free_all(element *free_element);

element *print_addr_data(element *view);

void shift_array(element *addr, int cursor);

element **realloc_char_array(element *addr);



//==================================================================================//
// Desenvolvimento de fun��es
//====================================//
// start_element()
void start_element(element *r)
{
  r->data       = NULL;
  r->first_addr = NULL;
  r->last_addr  = NULL;
  r->amount    = 0;
  r->addr_bank = alloc_addr_array(20);
}

//====================================//
// alloc_element()
element *alloc_element(element *r, char *data)
{
  if (r->addr_bank == NULL)
  {
  	printf("ERROR -> Element without allocation\n");
  	system("PAUSE");
  	return NULL;
  }
  element *new_element = NULL;
  int dataLen = strlen(data);
  dataLen += 1;
  if((new_element = (element*)calloc(1, sizeof(element))) == NULL) return NULL;
  if((new_element->data = (char*)calloc(dataLen, sizeof(char))) == NULL) return NULL;
  strcpy(new_element->data, data);
  if(!r->amount)r->addr_bank[r->amount] = new_element;
  r->first_addr = r->addr_bank[0];
  r->last_addr = new_element;
  r->addr_bank[r->amount] = r->last_addr;
  r->amount += 1;
  printf("Data allocated in ADDR: 0x%x\n", &new_element->data);
  return new_element;
}

//====================================//
// alloc_addr_array()
element **alloc_addr_array(int num0)
{
  element **ptr = NULL;
  if (num0 < 1) return NULL;
  if ((ptr = (element**)calloc(num0, sizeof(element*))) == NULL)return NULL;
  return ptr;
}

//====================================//
// remove_element()
element *remove_element(element *addr)
{
  register int i = 0;
  if (addr->last_addr == NULL || addr->amount < 1) 
  {
  	addr->first_addr = NULL;
	addr->last_addr = NULL;
  	return NULL;
  }
  free(addr->last_addr);
  addr->addr_bank[addr->amount-1] = NULL;
  if (addr->amount == 1)addr->last_addr = addr->addr_bank[addr->amount-1];
  else addr->last_addr = addr->addr_bank[addr->amount-2];
  addr->amount -= 1;
  putchar('\n');
  printf("Memory successfully released!\n");
  return NULL;
}

//====================================//
// spec_removal()
element *spec_removal(element *addr, unsigned int num)
{
  register int i = 0;
  if (addr->addr_bank[num] == NULL)return NULL;
  free(addr->addr_bank[num]);
  shift_array(addr, num);
  addr->amount -= 1;
  if (!(addr->addr_bank[addr->amount] == NULL))free (addr->addr_bank[addr->amount]);
  if (!num)addr->last_addr = addr->addr_bank[addr->amount];
  else addr->last_addr = addr->addr_bank[addr->amount-1];
  putchar('\n');
  printf("Memory successfully released!\n");
  return addr->addr_bank[num];
}

//====================================//
// print_array()
element *print_addr_data(element *view)
{
  register int i = 0;	
  printf ("Data written in ADDR -> 0x%x:  ", view);
  while (view->data[i] != 0)
  {
  	printf ("%c", view->data[i]);
    i++;
  }
  putchar('\n');
  return view;
}

//====================================//
// realloc_addr_array()
element **realloc_char_array(element *addr)
{ 
  if (addr->amount < 1)return NULL;
  if ((addr->addr_bank = (element**)realloc(addr->addr_bank, (addr->amount*2) * sizeof(element*))) == NULL)return NULL;
  return addr->addr_bank;
}

//====================================//
// free_all()
element *free_all(element *free_element)
{
  register int i;
  if (free_element->addr_bank == NULL)
  {
  	printf("\nElement is already clean!\n");
  	return NULL;
  }
  for(i = 0; i < free_element->amount; i++)free(free_element->addr_bank[i]);
  free_element->first_addr = NULL;
  free_element->last_addr  = NULL;
  free_element->amount = 0;
  free(free_element->addr_bank);
  free_element->addr_bank = NULL;
  printf("\nSuccessfully cleaned element!\n");
  return NULL;
}

//====================================//
// shift_array()
void shift_array(element *addr, int cursor)
{
  register int i;
  for (i = addr->amount*2; i > addr->amount; i--)
    addr->addr_bank[((2*addr->amount)-i) + cursor] = addr->addr_bank[(((2*addr->amount)+1)-i) + cursor];
}

#endif
