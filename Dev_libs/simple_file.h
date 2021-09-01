#ifndef SIMPLE_FILE_H
#define SIMPLE_FILE_H


// Inclusões
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//===================================================//
// Enums
typedef enum  // Criar/Abrir/Renomear
{
  ERROR_TO_OPEN_FILE = 0,
  FILE_OPENED_SUCCESSFULLY = 1,
  ERROR_TO_REOPEN_FILE = 2,
  SUCCESSFULLY_CLEANED_FILE = 4,
  WRITING_ERROR_IN_THE_FILE  = 5,
  WRITING_UNFINISHED = 6,
  SUCCESSFUL_WRITTEN_FILE  = 7,
  ERROR_READING_FILE_SIZE = 8,
  READING_PERFORMED_SUCCESSFULLY = 9,
  SUCCESSFUL_REMOVAL = 10,
}cfg;



//===================================================//
// Estruturas
typedef struct arqv_idnt
{
  FILE *file;
  char *data;
  const char *file_name;
  long amount_data;
  long file_len;
}arqv;


//===================================================//
// Protótipo de funções
cfg start_arqv(arqv *fl, const char *name);
cfg open_file(arqv *fl, const char *name);
char *read_data_file(arqv *fl, int cur_set);
cfg write_string(arqv *fl, char *str);
cfg clear_file(arqv *fl);

int length_file(arqv *fl);

cfg len_fl(arqv *fl);


//===================================================//
// Desenvolvimento das funções
cfg start_arqv(arqv *fl, const char *name)
{
  fl->amount_data  = 0;
  fl->file_len  = 0;
  fl->file_name = name;
  if (open_file(fl, name) != FILE_OPENED_SUCCESSFULLY) return ERROR_TO_OPEN_FILE;
  fclose(fl->file);
}


//===================================================//
cfg open_file(arqv *fl, const char *name)
{ 
  if ((fl->file = fopen(fl->file_name, "a+b")) == NULL)
  {
  	fclose(fl->file);
  	return ERROR_TO_OPEN_FILE;
  }
  if (fseek(fl->file, 0, SEEK_END) != 0)return ERROR_READING_FILE_SIZE;
  if (ftell(fl->file) == 0)fwrite("", sizeof(char), 1, fl->file);
  len_fl(fl);
  return FILE_OPENED_SUCCESSFULLY;
}


//===================================================//
cfg write_string(arqv *fl, char *str)
{
  if (strcmp (str, "") == 0)return WRITING_UNFINISHED;
  if (open_file(fl, fl->file_name) == ERROR_TO_OPEN_FILE)return ERROR_TO_OPEN_FILE;
  int er;
  int tam = strlen(str) + 1;
  if ((er = fwrite(str, sizeof(char), tam, fl->file)) != tam)
  {
  	clearerr (fl->file);
  	fclose(fl->file);
  	return WRITING_ERROR_IN_THE_FILE;
  }
  len_fl(fl);
  fclose (fl->file);
  return SUCCESSFUL_WRITTEN_FILE;
}


//===================================================//
cfg clear_file(arqv *fl)
{
  fclose (fl->file);
  if ((fl->file = fopen (fl->file_name, "w+b")) == NULL)return ERROR_TO_REOPEN_FILE;
  if (fl->data != NULL)
  {
  	free(fl->data);
  	fl->data = NULL;
  }
  fwrite("", sizeof(char), 1, fl->file);
  len_fl(fl);
  fclose (fl->file);
  return SUCCESSFULLY_CLEANED_FILE ;
}


//===================================================//
char *read_data_file(arqv *fl, int cur_set)
{
  if (fl->data != NULL)
  {
  	free(fl->data);
  	fl->data = NULL;
  }
  if (cur_set < 1 || cur_set > fl->amount_data)return NULL;
  if (open_file(fl, fl->file_name) == ERROR_TO_OPEN_FILE)return NULL;
  int cnt_wrds = 0, c = 0, tam = 0, er = 0;
  fpos_t p = 0;
  fseek(fl->file, 0, SEEK_END);
  fl->file_len = ftell(fl->file);
  if (fl->file_len == 0)
  {
  	fclose(fl->file);
  	return NULL;
  }
  rewind(fl->file);
  while (!feof(fl->file))
  {
  	c = fgetc(fl->file);
	if (ferror(fl->file)) return NULL;
	if (c == 0x00)cnt_wrds += 1;
	if (cnt_wrds == cur_set && !tam)fgetpos(fl->file, &p);
  	if (cnt_wrds == cur_set)tam += 1;
  	if (cnt_wrds == cur_set + 1)break;
  }
  char str[tam];
  fsetpos(fl->file, &p);
  if ((er = fread (str, sizeof(char), tam, fl->file)) != tam)return NULL;
  fl->data = (char*)calloc(tam, sizeof(char));
  strcpy(fl->data, str);
  fclose (fl->file);
  return fl->data;
}

//===================================================//
cfg len_fl(arqv *fl)
{
  int c, amount_file = 0;
  if (fseek(fl->file, 0, SEEK_END) != 0)return ERROR_READING_FILE_SIZE;
  fl->file_len = ftell(fl->file);
  if (fl->file_len == -1)return ERROR_READING_FILE_SIZE;
  
  fl->file_len -= 1;
  rewind(fl->file);
  while (!feof(fl->file))
  {
  	c = fgetc(fl->file);
  	if (c == 0x00)amount_file += 1;
  }
  amount_file -= 1;
  fl->amount_data = amount_file;
  return READING_PERFORMED_SUCCESSFULLY;
}


//===================================================//
cfg remove_data_file(arqv *fl, int cursor);

#endif
