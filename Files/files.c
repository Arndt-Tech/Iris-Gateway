// Inclusões
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Definições
#define arquivo "data.txt"


// Protótipo de funções
FILE *init_arq(const char *arqv_nm);
int write_file(FILE *file_ptr, char *str);

// Enums
typedef enum ERRORS
{
  FILE_NOT_FOUND   = 0,
  FILE_NOT_CREATED = 1,
  FILE_SUCCESSFULLY_CREATED = 2,
  FILE_FOUND       = 3,   
}ERRORS;


// Variáveis globais
int c;
char string[] = "";


// Ponteiros



// Objetos
FILE *arq = NULL;


// Desenvolvimento de funções
int main()
{
  if ((arq = init_arq(arquivo)) == NULL) printf("Impossivel criar arquivo!\n");     
  printf("Arquivo aberto com sucesso!\n");   
  putchar('\n');
  
  
  while (1)
  {
  	printf("1- Escrever no arquivo\n");
  	printf("2- Ler arquivo\n");
  	printf("3- Limpa arquivo\n");
  	printf("0- Sair e salvar\n");
  	putchar ('\n');
  	
  	int cmd = getch();
  	
  	switch (cmd)
  	{
  	  case '1':
  	  	printf("Escreva algo no arquivo\n");
        gets(string);
  	  break;
  	  
  	  
  	  case '2':
  	  	rewind(arq);
  	  	while ((c = fgetc(arq)) != EOF)printf("%c", c);
  	  	putchar('\n');
  	  	putchar('\n');
  	  	
  	  	fseek(arq, 0, SEEK_END);
  	  	printf("O arquivo tem %ld bytes.\n", ftell(arq));
  	  	
  	  	putchar('\n');
		system ("PAUSE");
  	  break;
  	  
  	  
  	  case '3':
  	  	fclose (arq);
  	  	if ((arq = fopen(arquivo, "w+")) == NULL)printf("Nao foi possivel limpar o arquivo\n");
  	  	printf("Arquivo limpo!\n");
  	  	system("PAUSE");
  	  break;
  	  
  	  
  	  case '0':
  	  	fclose (arq);
  	  	exit(1);
  	  break;
  	  
  	  
  	  default:
  	  	system ("CLS");
  	  	printf("Opcao inexistente\n");
  	  	system("PAUSE");
  	  break;
	}
    system ("CLS");
    fflush(stdin);
  }
  system("PAUSE");
  exit(1);
}



FILE *init_arq(const char *arqv_nm)
{
  FILE *file;
  if ((file = fopen(arqv_nm, "r+")) == NULL)
  {
  	if ((file = fopen(arqv_nm, "w+")) == NULL)return NULL;
	return file;
  }
  else return file;
}



int write_file(FILE *file_ptr, char *str)
{
  int error;
  if ((error = fputs(str, file_ptr)) == EOF)return 0;
  else return 1;
  system ("PAUSE");
}



void *close_file(FILE *arqv)
{
  fclose (arqv);
  return NULL;
}
