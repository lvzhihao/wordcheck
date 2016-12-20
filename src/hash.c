/**
 ** edwin.lzh@gmail.com
 **/

#include "wordcheck/platform.h"
#include "wordcheck/table.h"
#include "wordcheck/mm.h"
#include "wordcheck/hash.h"

uint wordcheck_mmhash_get_str(const char* str)
{
	char *buffer = malloc(4);
	snprintf(buffer, 4, "%d", str[0]);
	int ascii = atoi(buffer);
	free(buffer);
	//printf("%d\n", ascii);
	return (uint)ascii % WORDCHECK_MM_HASH_SEC;
}

wcmmHash* wordcheck_mmhash_table_create(wcMM* MM)
{
	wcmmHash* ht = (wcmmHash *)wordcheck_mm_malloc(MM, sizeof(wcmmHash));
	int i = 0;
	for(i=0; i<WORDCHECK_MM_HASH_SEC; i++){
		ht->offset[i] = 0;
	}
	return ht;
}

int wordcheck_mmhash_table_insert(wcMM* MM, wcmmHash* ht, const char* key, uint val)
{
	wcmmNode* lt;
	int sec = wordcheck_mmhash_get_str(key);
	if(ht->offset[sec] == 0){
		ht->offset[sec] = MM->offset;
		lt = wordcheck_mmnode_list_create(MM);
		lt->block_offset = val;
		memcpy(lt->word, key, 2);
		return WORDCHECK_SUCCESS;
	}else{
		lt = (wcmmNode *)(MM + ht->offset[sec]);
		return wordcheck_mmnode_list_insert(MM, lt, key, val);
	}
}

int wordcheck_mmhash_table_lookup(wcMM* MM, wcmmHash* ht, const char* key, uint* val){
	wcmmNode* lt = NULL;
	int sec = wordcheck_mmhash_get_str(key);
	if(ht->offset[sec] == 0){
		return WORDCHECK_FAILURE;
	}else{
		lt = (wcmmNode *)(MM + ht->offset[sec]);
        return wordcheck_mmnode_list_lookup(MM, lt, key, val);
	}
}

wcmmNode* wordcheck_mmnode_list_create(wcMM* MM)
{
	wcmmNode* lt = (wcmmNode *)wordcheck_mm_malloc(MM, sizeof(wcmmNode));
	memset(lt->word, '\0', 2);
	lt->next_offset = 0;
	lt->block_offset = 0;
	return lt;
}

int wordcheck_mmnode_list_insert(wcMM* MM, wcmmNode* list, const char* key, uint val)
{
	wcmmNode *lt = list;
	while(lt->next_offset != 0){
		if(strcmp(lt->word, key) == 0){
			return WORDCHECK_FAILURE;
		}
		lt = (wcmmNode *)(MM + lt->next_offset);
	}
	lt->next_offset = MM->offset;
	wcmmNode* new_lt = wordcheck_mmnode_list_create(MM);
	new_lt->block_offset = val;
	memcpy(new_lt->word, key, 2);
	return WORDCHECK_SUCCESS;
}

int wordcheck_mmnode_list_lookup(wcMM* MM, wcmmNode* list, const char* key, uint* val)
{
	wcmmNode* lt = list;
	while(lt != NULL){
		if(strcmp(lt->word, key) == 0){
			*val = lt->block_offset;
			return WORDCHECK_SUCCESS;
		}
		if(lt->next_offset > 0){
			lt = (wcmmNode*)(MM + lt->next_offset);
		}else{
			lt = NULL;
		}
	}
	return WORDCHECK_FAILURE;
}

wcList* wordcheck_list_create(void)
{
	wcList* lt = (wcList *)malloc(sizeof(wcList));
	lt->pre = NULL;
	lt->next = NULL;
	lt->key = NULL;
	lt->val = NULL;
	return lt;
}

wcList* wordcheck_list_insert(wcList* list, void* key, void* val)
{
	if(list->key == NULL){
		list->key = key;
		list->val = val;
		return list;
	}else{
		wcList *lt = list;
		while(lt->next != NULL){
			lt = (wcList *)lt->next;
		}
		wcList* new_lt = wordcheck_list_create();
		lt->next = new_lt;
		new_lt->key = key;
		new_lt->val = val;
		new_lt->pre = lt;
		return new_lt;
	}
}

wcList* wordcheck_list_lookup(wcList* list, void* key, void** val)
{
    wcList *lt;
    while(wordcheck_list_get_current(list, &lt) == WORDCHECK_SUCCESS)
    {
		if(strcmp(lt->key, key) == 0){
			*val = lt->val;
			return lt;
		}
        wordcheck_list_next_item(&list);
	}
	return NULL;
}

wcList* wordcheck_list_rewind(wcList* list)
{
	wcList *lt = list;
	while(lt->pre != NULL){
		lt = (wcList *)lt->pre;
	}
	return lt;
}

wcList* wordcheck_list_last(wcList* list)
{
	wcList *lt = list;
	while(lt->next != NULL){
		lt = (wcList *)lt->next;
	}
	return lt;
}

wcList* wordcheck_list_reverse(wcList* list)
{
	wcList *lt = wordcheck_list_last(list);
	wcList *olt = wordcheck_list_create();
	olt->key = lt->key;
	olt->val = lt->val;
	while(lt->pre != NULL){
		lt = lt->pre;
		wcList *tlt = wordcheck_list_create();
		tlt->key = lt->key;
		tlt->val = lt->val;
		tlt->pre = olt;
		olt->next = tlt;
		olt = tlt;
	}
	return wordcheck_list_rewind(olt);
}

int wordcheck_list_get_current(wcList* list, wcList **lt)
{   
    if(list != NULL && list->key != NULL){
		*lt = list;
		return WORDCHECK_SUCCESS;
    }else{
        *lt = NULL;
		return WORDCHECK_FAILURE;
    }
}

int wordcheck_list_next_item(wcList** list)
{
	wcList *lt = *list;
	if(lt->next == NULL){
        *list = NULL;
		return WORDCHECK_FAILURE;
	}else{
		*list = lt->next;
		return WORDCHECK_SUCCESS;
	}	
}
