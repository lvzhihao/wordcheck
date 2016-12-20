/**
 ** edwin.lzh@gmail.com
 **/

#include "wordcheck/platform.h"
#include "wordcheck/table.h"
#include "wordcheck/mm.h"
#include "wordcheck/hash.h"
#include "wordcheck/mmtable.h"
#include "wordcheck/docs.h"
#include "wordcheck/check.h"

int open_filter_file(const char* filename, char** filterTxt)
{
	FILE *fp = fopen(filename, "r");
	if(fp != NULL){
		long int save_pos;
		long int filesize;
		save_pos = ftell(fp);
		fseek(fp, 0L, SEEK_END);
		filesize = ftell(fp);
		fseek(fp, save_pos, SEEK_SET);

		char *buffer = malloc(1024);
		char *txt = malloc(filesize+1);
		memset(buffer, '\0', 1024);
		memset(txt, '\0', filesize+1);
		while(fgets(buffer, 1024, fp) != NULL){
			strcat(txt, buffer);
		}
		fclose(fp);
		free(buffer);
		*filterTxt = txt;
	}
}

int main()
{
	/*
	char deny_file[100];
	strcpy(deny_file, WORDCHECK_DIR);
	strcat(deny_file, "/deny");
	unlink(deny_file);
	*/
	
	/*
	wcMM *MM;
	wordcheck_mm_create("deny", &MM);

	assert(sem_init(&MM->mutex, 1, 1) == 0); 
	setbuf(stdout, NULL);
	assert(sem_wait(&MM->mutex) >=0 );
	
	//printf("%d\n", MM->offset);
	//wcHash* ht = wordcheck_hash_table_create(MM);
	//wordcheck_hash_table_insert(ht, MM, "A", 1212);
	
	wcHash* ht = (wcHash *)(MM + 20);

	int offset;
	if(wordcheck_hash_table_lookup(ht, MM, "A", &offset) == 0){
		printf("%d\n", offset);
	}else{
		printf("%s\n", "Not Found");
	}

	assert(sem_post(&MM->mutex) >=0 );

	return;
	*/
	//wordcheck_set_replace_op("#");
	//wordcheck_set_replace_len(8);
	//printf("%s\n", wordcheck_get_replace_op());
    
	wcTable *table;
	wordcheck_create_table(&table);
	const char file[30] = "demo.txt";
	wordcheck_open_deny_file(file, table);
	
    wcMMInfo *mmInfo;
	wcMM *MM;
    wordcheck_mminfo_create(&mmInfo, "demo test", 1073741824);
	wordcheck_mm_create(&MM, mmInfo, "demo");
    //printf("create MM success\n");

	wordcheck_mmtable_create(MM, table);
	//printf("%d\n", table->num);
	
	//printf("%d\n", MM->offset);
	//return;

    wcMMInfo *mmInfo1;
	wcMM *MM1;
	wordcheck_mm_fetch(&MM1, "demo");
    wordcheck_mminfo_fetch(&mmInfo1, MM1);
    //printf("fetch MMInfo\n");
    //printf("version: %s\n", mmInfo1->version);
    //printf("memo: %s\n", mmInfo1->memo);
    //printf("mm_size: %d\n", mmInfo1->mm_size);

	wcmmTable *mmtable;
	wordcheck_mmtable_fetch(MM1, &mmtable);

	//printf("%d\n", mmtable->num);
	//return;

	char *txt;
	open_filter_file("filter.txt", &txt);
	//printf("%s\n", txt);return;

	wcList *list;
	
	char *out;
    int out_len;
    int num = 0;
	//num = wordcheck_check(table, txt, strlen(txt), &out, &out_len, &list);
    num = wordcheck_mm_check(MM1, mmtable, txt, strlen(txt), &out, &out_len, &list);
	printf("\n");
	printf("%s\n",     "============info============");
	printf("Memory: %d (%0.2fM)\n", wordcheck_mm_size(MM1), (double)wordcheck_mm_size(MM1)/1024/1024);
	printf("Nums: %d\n", mmtable->num);
	printf("\n\n%s\n", "============txt=============");
	printf("%s\n", txt);
	printf("\n\n%s\n", "===========output===========");
	printf("%s\n", out);
	printf("\n\n%s\n", "============num=============");
	printf("%d\n", num);
	printf("\n\n%s\n", "===========result===========");
    wcList *lt;
    while(wordcheck_list_get_current(list, &lt) == WORDCHECK_SUCCESS)
    {
        //printf("%s\n", list->key);
        wcResult* res = (wcResult *)lt->val;
        printf("-->禁词: %s\n", res->string);
        printf("-->起始: %d\n", res->start);
        printf("-->长度: %d\n\n", res->len);
        wordcheck_list_next_item(&list);
	}
	printf("\n");
    
    //sleep(3000000);
}

