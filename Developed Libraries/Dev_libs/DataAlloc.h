#ifndef DATA_ALLOC_H
#define DATA_ALLOC_H

//==================================================================================//
// Enum's
typedef enum 
{
  // ALLOCATION
  ELEMENT_WITHOUT_ALLOCATION = 0,
  NO_DATA_TO_ALLOCATE = 1,
  ERROR_DURING_ALLOCATION = 2,
  ERROR_DURING_RELOCATION = 3,
  ELEMENT_SUCCESSFULLY_RELOCATED = 4,
  ELEMENT_SUCCESSFULLY_ALLOCATED = 5,
  ERROR_DURING_BANK_ALLOCATION = 6,
  BANK_SUCCESSFULLY_ALLOCATED = 7,
  // REMOVE
  ELEMENT_NOT_FOUND  = 20,
  ERROR_DURING_REMOVAL = 21,
  ELEMENT_SUCCESSFULLY_REMOVED = 22,
  // RELEASE 
  ELEMENT_IS_ALREADY_RELEASED = 40,
  ELEMENT_SUCCESSFULLY_RELEASED = 41,
}malc;

//==================================================================================//
// Struct's
typedef struct elem_idnt
{
  char     *data;
  struct elem_idnt  *first_addr;
  struct elem_idnt   *last_addr;
  struct elem_idnt  **addr_bank;
  int   bank_spc;
  int     amount;
}element;

//==================================================================================//
// Prot�tipo de fun��es
malc StartElement(element *elm);
malc AllocElement(element *elm, char *data);
malc RemoveElement(element *addr, unsigned int num);
char *ReadElement(element *view);
malc FreeElement(element *free_element);
element **alloc_addr_array(int num0);
malc realloc_addr_array(element *addr);
void shift_array(element *addr, int cursor);

//==================================================================================//
// Desenvolvimento de fun��es
//====================================//
// StartElement()
malc StartElement(element *elm)
{
  elm->data       = NULL;
  elm->first_addr = NULL;
  elm->last_addr  = NULL;
  elm->bank_spc  =  1;
  elm->amount    =  0;
  if ((elm->addr_bank = alloc_addr_array(elm->bank_spc)) == NULL)return ERROR_DURING_BANK_ALLOCATION;
  return BANK_SUCCESSFULLY_ALLOCATED;
}

//====================================//
// AllocElement()
malc AllocElement(element *elm, char *data)
{
  if (elm->addr_bank == NULL)return ELEMENT_WITHOUT_ALLOCATION;
  if (data[0] == 0x00)return NO_DATA_TO_ALLOCATE;
  element *new_element = NULL; 
  int dataLen = strlen(data);
  dataLen += 1;
  if((new_element = (element*)calloc(1, sizeof(element))) == NULL) return ERROR_DURING_ALLOCATION;
  if((new_element->data = (char*)calloc(dataLen, sizeof(char))) == NULL) return ERROR_DURING_ALLOCATION;
  strcpy(new_element->data, data);
  if(!elm->amount)elm->addr_bank[elm->amount] = new_element;
  elm->data = new_element->data;
  elm->first_addr = elm->addr_bank[0];
  elm->last_addr = new_element;
  elm->addr_bank[elm->amount] = elm->last_addr;
  elm->amount += 1;
  if (elm->amount >= elm->bank_spc)
  {
	if (realloc_addr_array(elm) == ERROR_DURING_RELOCATION)return ERROR_DURING_RELOCATION;
	else return ELEMENT_SUCCESSFULLY_ALLOCATED;
  }
  return ELEMENT_SUCCESSFULLY_ALLOCATED;
}

//====================================//
// RemoveElement()
malc RemoveElement(element *addr, unsigned int num)
{
  register int i = 0;
  if (addr->addr_bank[num] == NULL)return ELEMENT_NOT_FOUND;
  if (num > addr->bank_spc)return ELEMENT_NOT_FOUND;
  free(addr->addr_bank[num]);
  shift_array(addr, num);
  addr->amount -= 1;
  if (!(addr->addr_bank[addr->amount] == NULL))free (addr->addr_bank[addr->amount]);
  if (!num)addr->last_addr = addr->addr_bank[addr->amount];
  else addr->last_addr = addr->addr_bank[addr->amount-1];
  if (addr->amount <= addr->bank_spc-2)
  {
  	if (realloc_addr_array(addr) == ERROR_DURING_RELOCATION)return ERROR_DURING_REMOVAL;
  	return ELEMENT_SUCCESSFULLY_REMOVED;
  }
  return ELEMENT_SUCCESSFULLY_REMOVED;
}

//====================================//
// ReadElement()
char *ReadElement(element *view)
{
  register int i = 0;
  if (view == NULL)return NULL;
  while (view->data[i] != 0)i++;
  char str[i];
  i = 0;
  while (view->data[i] != 0)
  {
  	str[i] = view->data[i];
  	i++;
  }
  str[i] = 0x00;
  strcpy(view->data, str);
  return view->data;
}

//====================================//
// FreeElement()
malc FreeElement(element *free_element)
{
  register int i;
  if (free_element->addr_bank == NULL)return ELEMENT_IS_ALREADY_RELEASED;
  for(i = 0; i < free_element->amount; i++)free(free_element->addr_bank[i]);
  free_element->first_addr = NULL;
  free_element->last_addr  = NULL;
  free_element->bank_spc  = 0;
  free_element->amount    = 0;
  free(free_element->addr_bank);
  free_element->addr_bank = NULL;
  return ELEMENT_SUCCESSFULLY_RELEASED;
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
// realloc_addr_array()
malc realloc_addr_array(element *addr)
{ 
  register int i;
  if (addr->bank_spc < 1)return ERROR_DURING_RELOCATION;
  element new_bank;
  addr->bank_spc = addr->amount + 1;
  if((new_bank.addr_bank = (element**)calloc(addr->bank_spc, sizeof(element*))) == NULL)return ERROR_DURING_RELOCATION;
  for (i = 0; i < addr->amount; i++) new_bank.addr_bank[i] = addr->addr_bank[i];
  free(addr->addr_bank);
  addr->addr_bank = new_bank.addr_bank;
  return ELEMENT_SUCCESSFULLY_RELOCATED;
}

//====================================//
// shift_array()
void shift_array(element *addr, int cursor)
{
  register int i;
  for (i = cursor; i < addr->amount; i++)addr->addr_bank[i] = addr->addr_bank[i+1];
}

#endif
