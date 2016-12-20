/**
 ** edwin.lzh@gmail.com
 **/

#include "wordcheck/platform.h"
#include "wordcheck/table.h"
#include "wordcheck/docs.h"

int wordcheck_open_deny_file(const char *file, wcTable *table)
{
	FILE *fp;
	char *buffer = malloc(1024);
	char *words = malloc(1024);
		
	fp = fopen(file, "r");
	if(fp == NULL)	return WORDCHECK_FAILURE;
	while(fgets(buffer, 1024, fp) != NULL){
		memset(words, 0, 1024);
		strcpy(words, wordcheck_utils_trim(buffer));
		if(strcmp(words, "") != 0){
			//printf("%s\n", words);
			wordcheck_insert_deny(words, table);
		}
	}
	free(buffer);
	free(words);
	fclose(fp);
	return WORDCHECK_SUCCESS;	
}

int wordcheck_insert_deny(const char *words, wcTable *table)
{
	return wordcheck_insert_table(words, table);
}

