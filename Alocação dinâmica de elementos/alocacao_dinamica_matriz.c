/*
   // Limpar lixo de memoria quando uma nova realocação é feita;
   // Contar e printar as matrizes de forma inteligente;
*/

// Inclusões
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Definições
#define five 5
#define ten 10
#define DATA_LEN 20


// Protótipo de funções

// Memory Alloc - Matrix
char **alloc_char_matrix(int num0, int num1);                                   // Aloca uma matriz de unsigned char
char **realloc_char_matrix(char **new_ptr, int num0, int num1);                 // Realoca uma matriz de unsigned char
char **free_char_matrix(char **free_ptr, int num0, int num1);                   // Libera alocação de memória

// Memory Alloc - Array
char **alloc_char_array(int num0);
char **realloc_char_array(char **new_ptr, int num0);
char **free_char_array(char **free_ptr);

void write_str_matrix(char** mtx, const char* str, int init_vec);               // Escreve uma string na matriz
void print_char_matrix(char **free_ptr, int max_len0, int max_len1);            // Printa a matriz


// Ponteiros
char **station = NULL;


// Desenvolvimento de funções
//==================================================================================//
int main(int argc, char *argv[])
{ 
  station = alloc_char_matrix(five, five);
  
  print_char_matrix(station, five, five);
  putchar('\n');
  putchar('\n');
  
  
  
  station = realloc_char_matrix(station, ten, ten);
  putchar('\n');
  putchar('\n');
  print_char_matrix(station, ten, ten);
  
  
  
  station = realloc_char_matrix(station, five, five);
  putchar('\n');
  putchar('\n');
  print_char_matrix(station, five, five);
  
  
  /*
  station = realloc_char_matrix(station, ten, ten);
  putchar('\n');
  putchar('\n');
  print_char_matrix(station, ten, ten);
  */
  
  
  station = free_char_matrix(station, ten, ten);
  system("PAUSE");
  return 0;
}



//==================================================================================//
char **alloc_char_matrix(int num0, int num1)
{
  char **ptr = NULL;
  register int i, j;
  if (num0 < 1 || num1 < 1) return NULL;
  ptr = (char**)calloc(num0, sizeof(char*));
  if (ptr == NULL)return NULL;
  for (i = 0; i < num0; i++) 
  {
  	ptr[i] = (char*)calloc(num1, sizeof(char));
	if (ptr[i] == NULL)return NULL;
  } 
  return ptr;
}



//==================================================================================//
char **realloc_char_matrix(char **new_ptr, int num0, int num1)
{
  register int i, j;
  if (num0 < 1 || num1 < 1)return NULL;
  new_ptr = (char**)realloc(new_ptr, num0 * sizeof(char*));
  if (new_ptr == NULL)return NULL;
  for (i = 0; i < num0; i++) 
  {
  	new_ptr[i] = (char*)realloc(new_ptr[i], num1 * sizeof(char));
  	if (new_ptr[i] == NULL)return NULL;
  }
  return new_ptr;
}



//==================================================================================//
char **free_char_matrix(char **free_ptr, int num0, int num1)
{
  register int i;
  putchar('\n');
  if (free_ptr == NULL) return NULL;
  if (num0 < 1 || num1 < 1) return free_ptr;
  for (i = 0; i < num0; i++) free(free_ptr[i]);
  free(free_ptr);
  printf("Memory successfully released!\n");
  return NULL;
}



//==================================================================================//
void print_char_matrix(char **free_ptr, int max_len0, int max_len1)
{
  register int i, j;	
  for (i = 0; i < max_len0; i++)
  {
  	printf ("Estacao %d: ", i);
  	for ( j = 0; j < max_len1; j++) printf (" %d",free_ptr[i][j]);
  	putchar('\n');
  }   
}



//==================================================================================//
void write_str_matrix(char** mtx, const char* str, int init_vec)
{
  register int i;
  for (i = 0; i < strlen(str)+1; i++)mtx[init_vec][i] = str[i];
  mtx[init_vec][strlen(str)] = 0x00;
}



//==================================================================================//
char **alloc_char_array(int num0)
{
  char **ptr = NULL;
  if (num0 < 1) return NULL;
  ptr = (char**)calloc(num0, sizeof(char*));
  if (ptr == NULL)return NULL;
  return ptr;
}



//==================================================================================//
char **realloc_char_array(char **new_ptr, int num0)
{
  if (num0 < 1)return NULL;
  new_ptr = (char**)realloc(new_ptr, num0 * sizeof(char*));
  if (new_ptr == NULL)return NULL;
  return new_ptr;
}



//==================================================================================//
char **free_char_array(char **free_ptr)
{
  putchar('\n');
  if (free_ptr == NULL) return NULL;
  free(free_ptr);
  printf("Memory successfully released!\n");
  return NULL;
}



