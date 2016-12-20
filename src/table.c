/**
 ** edwin.lzh@gmail.com
 **/

#include "wordcheck/platform.h"
#include "wordcheck/table.h"

void wordcheck_create_block(wcBlock **block)
{
	wcBlock* instance;
	instance = (wcBlock *)malloc(sizeof(wcBlock));
	memset(instance->word, '\0', 2);
	instance->aNum = 0;
	instance->isDeny = 0;
	wcTable* table;
	wordcheck_create_table(&table);
	instance->cTbl = table;
	*block = instance;
}

void wordcheck_free_block(wcBlock *block)
{
	free(block->word);
	wordcheck_free_table(block->cTbl);
	free(block);
}

void wordcheck_create_table(wcTable **table)
{
	wcTable* instance;
	instance = (wcTable *)malloc(sizeof(wcTable));
	instance->num = 0;
	instance->tbl = g_hash_table_new(g_str_hash, g_str_equal);
	*table = instance;
}

void wordcheck_free_table(wcTable *table)
{
	g_hash_table_destroy(table->tbl);
	free(table);
}

void wordcheck_handle_table(char *words, wcTable *table)
{
	long pos = strlen(words) - 1;
	unsigned char *buffer = (unsigned char*)malloc(2);
	memset(buffer, '\0', 2);
	memset(buffer, words[pos], 1);
	//printf("%d\n", pos);
	//printf("%d\n", table->num);return;
	table->num++;
	wcBlock *block;
	if(g_hash_table_lookup_extended(table->tbl, buffer, NULL, (void **)&block) == TRUE){
		//printf("exists\n");
		//printf("%c   exists\n", buffer);
		if(pos > 0){
			words[pos] = '\0';
			block->aNum++;
			wordcheck_handle_table(words, block->cTbl);
		}else{
			block->isDeny = 1;
		}
	}else{
		//printf("fail\n");
		//printf("%c   fail\n", buffer);
		wordcheck_create_block(&block);
		memcpy(block->word, buffer, 1);
		if(pos > 0){
			words[pos] = '\0';
			block->aNum++;
			wordcheck_handle_table(words, block->cTbl);
		}else{
			block->isDeny = 1;
		}
		g_hash_table_insert(table->tbl, buffer, block);
	}
}

int wordcheck_insert_table(const char *words,  wcTable *table)
{
	char *toWords = malloc(strlen(words)+1);
	memset(toWords, '\0', strlen(words)+1);
	memcpy(toWords, words, strlen(words));
    wordcheck_utils_strtoupper(toWords);   //不区分大小写
	wordcheck_handle_table(toWords, table);
	//printf("%d\n", t->num);
	return WORDCHECK_SUCCESS;
}
