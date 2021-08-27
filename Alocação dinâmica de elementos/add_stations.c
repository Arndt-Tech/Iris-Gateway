/*
   Programa de teste para adicionar estações
   ENUMS:
  --------------------------------------------------------------
   add_station():
   * '0' --> Número máximo de estações atingido;
   * '1' --> Estação adicionada;
  --------------------------------------------------------------
  remove_station():
  * '0' --> Nenhuma estação para remover;
  * '1' --> Estação removida;
  --------------------------------------------------------------
  custom_remove_station():
  * '0' --> Nenhuma estação para remover;
  * '1' --> Estação removida;
  * '2' --> Nenhuma estação no endereço apontado;
  --------------------------------------------------------------
*/

// Inclusões
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definições
#define MAX_STATIONS 10
#define data 1

// Protótipo de funções
int add_station(int **ptr_mem, int *anch, int max_anch, int *addr, int *cnt, int dat);  // Adiciona estação
int remove_station(int **ptr_mem, int *anch, int *addr, int *cnt);                      // Remove última estação adicionada
int custom_remove_station(int **ptr_mem, int *anch, int num, int *cnt);                 // Remove uma estação específica
void *vrf_av_maddr(int *vec, int rep);                                                  // Verifica espaço de memória disponível mais próximo da âncora
void *vrf_unav_maddr(int *vec, int rep);                                                // Verifica espaço de memória indisponível mais afastado da âncora
void mem_rst(int **ptr_mem, int *anch, int max_anch , int *cnt, int *vec);              // Reseta contadores e volta para endereços iniciais
void view_station(int *vec, int rep, int *cnt);                                         // Visualiza todas estações
void mem_debug();                                                                       // Mostra dados técnicos da memória

// Variáveis Globais
int station [MAX_STATIONS] = {0};
int counter = 0;




char string[] = "";

// Ponteiros
int  *ptrStation = NULL,
     *anchor     = NULL;

//==================================================================================//
int main(int argc, char *argv[])
{ 
  ptrStation = station;
  anchor     = station;
   
  while (1)
  {
    printf ("1 - Adicionar nova estacao\n");
    printf ("2 - Remover estacao\n");
    printf ("3 - Verificar estacoes\n");
    printf ("4 - Remover estacao especifica\n");
    printf ("8 - Visualizar enderecos\n");
    printf ("9 - Reset\n");
	printf ("ESC - Quit\n");
    putchar('\n');
	
    int cmd = getch ();
	  
    switch (cmd)
    {
      case '1':
        add_station(&ptrStation, anchor, MAX_STATIONS, (int*)vrf_av_maddr(station, MAX_STATIONS) ,&counter, data);
      break;
		
		
      case '2':
        remove_station(&ptrStation, anchor, (int*)vrf_unav_maddr(station, MAX_STATIONS), &counter);
      break;
		
		
      case '3':
        view_station(station, MAX_STATIONS, &counter);
      break;
		
		
      case '4':
        printf("Digite a estacao que deseja remover\n");
        custom_remove_station(&ptrStation, anchor, getchar(), &counter);
      break;
		
		
      case '8':
        mem_debug();
      break;
		
		
      case '9':
        mem_rst(&ptrStation, anchor, MAX_STATIONS, &counter, station);
      break;
		
		
      case 0x1B:
        exit (0);
      break;
    }	
  system ("CLS");
  }
  return 0;
}



//==================================================================================//
int add_station(int **ptr_mem, int *anch, int max_anch, int *addr, int *cnt, int dat)
{ 
  register int *ptr_aux = *ptr_mem; 
  if (*ptr_mem == (anch + (max_anch-1)) && addr == NULL || addr == NULL) // Caso endereço estiver ocupado
  {
    printf ("MEMORY OVERFLOW!\n");
    system("PAUSE");
    return 0;
  }
  else if (addr != NULL)                                                 // Caso o endereço estiver livre
  {
    *ptr_mem = addr;
    **ptr_mem = dat;
    
    ++*cnt;
    printf ("Estacao adicionada!\n");
    return 1;
  } 
}



//==================================================================================//
int remove_station(int **ptr_mem, int *anch, int *addr, int *cnt)
{
  if (*ptr_mem <= anch && addr == NULL || addr == NULL) // Caso o endereço estiver livre
  {
    printf ("Nenhuma estacao para remover\n");
    system ("PAUSE");
    return 0;
  }
  else if (addr != NULL)                                // Caso algum endereço estiver ocupado
  {
    *ptr_mem = addr; 
    **ptr_mem = 0;
    --*cnt;
    printf ("Estacao removida!\n");
    return 1;
  }
}



//==================================================================================//
int custom_remove_station(int **ptr_mem, int *anch, int num, int *cnt)
{
  int *ptr_aux = *ptr_mem;
  if (*ptr_mem <= anch)
  {
    printf ("Nenhuma estacao para remover\n");
    system ("PAUSE");
    return 0;
  }
  else 
  {
    *ptr_mem  = (anch + (num - 48));       // OBS: (num-48) Operação utilizada para compatibilizar com a função getchar que retorna char
    if (**ptr_mem != 0)                    // Caso o endereço estiver ocupado
    {
      **ptr_mem = 0;	
      --*cnt;
      printf ("Estacao removida\n");
      *ptr_mem = ptr_aux;
      return 1;
	}
    else                                   // Caso o endereço estiver livre
    {
      ptrStation = ptr_aux;
      ptr_aux = NULL;
      printf ("Nenhuma estacao neste endereco\n");
      system("PAUSE");
      return 2;
    } 
  }
}



//==================================================================================//
void view_station(int *vec, int rep, int *cnt)
{
  register int i;
  if (*cnt == 0) printf("Nenhuma estacao adicionada\n");
  for (i = 0; i < rep; i++) printf ("Estacao %d: %d\n", i, vec[i]);
  putchar ('\n');
  if (*cnt > 0)printf ("Total de %d estacoes adicionadas\n", *cnt);
  putchar ('\n');
  system ("PAUSE");
}



//==================================================================================//
void mem_debug () 
{
  printf("Tamanho maximo de memoria: 0x%x\n", (anchor+(MAX_STATIONS-1)));
  printf("Anchor     ADDR: 0x%x\n",     anchor);
  printf("ptrStation ADDR: 0x%x\n", ptrStation);
  putchar ('\n');
  printf("Counter    ADDR: 0x%x\n",   &counter);
  printf("Counter   VALUE: %d\n"  ,    counter);
  putchar ('\n');
  printf("Espaco mais proximo da ancora vazio: 0x%x\n", vrf_av_maddr(station, MAX_STATIONS));
  printf("Espaco mais afastado da ancora cheio: 0x%x\n", vrf_unav_maddr(station, MAX_STATIONS));
  putchar ('\n');
  if (counter > 0)printf ("Total de %d estacoes adicionadas\n", counter);
  putchar ('\n');
  
  system ("PAUSE");
}



//==================================================================================//
void mem_rst(int **ptr_mem, int *anch, int max_anch , int *cnt, int *vec)
{
  register int i;
  *ptr_mem = anch;
  *cnt = 0;
  for (i = 0; i <= max_anch; i++) vec[i] = 0;
  printf("Programa resetado!\n");
  system("PAUSE");
}



//==================================================================================//
void *vrf_av_maddr(int *vec, int rep)
{
  register int i;
  int *vmem = NULL;
  
  for (i = 0; i < rep; i++)
  {
    vmem = &vec[i];
    if (*vmem == 0) return &vec[i];
  }
  return NULL;
}



//==================================================================================//
void *vrf_unav_maddr(int *vec, int rep)
{
  register int i;
  int *vmem = NULL;
  
  for (i = (2*(rep)); i > rep; i--)
  {
    vmem = &vec[i-(rep+1)];
    if (*vmem != 0) return &vec[i-(rep+1)];
  }
  return NULL;
}
