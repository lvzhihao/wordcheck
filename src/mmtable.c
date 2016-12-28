/**
 ** edwin.lzh@gmail.com
 **/

#include "wordcheck/platform.h"
#include "wordcheck/table.h"
#include "wordcheck/mm.h"
#include "wordcheck/hash.h"
#include "wordcheck/mmtable.h"

typedef struct wcmmConvert {
    wcMM *MM;
    uint tbl_offset;
} wcmmConvert;

/*保存MM*/
int wordcheck_mmtable_create(wcMM *MM, wcTable *table)
{
    //assert(sem_init(&MM->mutex, 1, 1) == 0);
    //setbuf(stdout, NULL);
    //assert(sem_wait(&MM->mutex) >=0 );

    MM->mmtable_start = MM->offset;
    //printf("MM Offset: %d\n", MM->offset);
    //wcmmTable *mmtable = wordcheck_mmtable_create_from_table(MM, table);
    wordcheck_mmtable_create_from_table(MM, table);
    wcMMInfo *mmInfo;
    wordcheck_mminfo_fetch(&mmInfo, MM);
    //printf("version: %s\n", mmInfo->version);
    //printf("memo: %s\n", mmInfo->memo);
    //printf("size: %d\n", mmInfo->mm_size);
    msync(MM, mmInfo->mm_size, 0);

    //assert(sem_post(&MM->mutex) >=0 );
    return WORDCHECK_SUCCESS;
}

int wordcheck_mmtable_fetch(wcMM *MM, wcmmTable **table)
{
    //TODO: nginx module下会出问题，暂时注释
    //assert(sem_init(&MM->mutex, 1, 1) == 0);
    //setbuf(stdout, NULL);
    //assert(sem_wait(&MM->mutex) >=0 );

    wcmmTable *mmtable = (wcmmTable *)(MM + MM->mmtable_start);
    *table = mmtable;

    //assert(sem_post(&MM->mutex) >=0 );
    return WORDCHECK_SUCCESS;
}

wcmmTable *wordcheck_mmtable_create_from_table(wcMM *MM, wcTable *table)
{
    uint offset = MM->offset;
    wordcheck_create_mmtable(MM);
    wcmmTable *mmtable = (wcmmTable *)(MM + offset);

    mmtable->num = table->num;

    wcmmConvert *convert = (wcmmConvert *)malloc(sizeof(wcmmConvert));
    convert->MM = MM;
    convert->tbl_offset = mmtable->tbl_offset;

    g_hash_table_foreach(table->tbl, wordcheck_mmtable_convert, convert);

    free(convert);

    return mmtable;
}

void wordcheck_mmtable_convert(gpointer key, gpointer val, gpointer data)
{
    char *orgkey = key;  //暂时只支持gHash的key为字符串形式
    wcBlock *orgval = val;
    wcmmConvert *convert = data;

    char *str = (char *)wordcheck_mm_malloc(convert->MM, 1);
    memset(str, '\0', 1);
    memset(str, orgkey[0], 1);

    uint offset = convert->MM->offset;
    wordcheck_create_mmblock(convert->MM);
    wcmmBlock *block = (wcmmBlock *)(convert->MM + offset);

    strncpy(block->word, orgval->word, 1);
    block->info = orgval->info;
    block->aNum = orgval->aNum;
    //printf("%d\n", block->aNum);
    //printf("%d\n", offset);
    if (block->aNum > 0) {
        block->cTbl_offset = convert->MM->offset;
        wordcheck_mmtable_create_from_table(convert->MM, orgval->cTbl);
    }
    wcmmHash *ht = (wcmmHash *)(convert->MM + convert->tbl_offset);
    wordcheck_mmhash_table_insert(convert->MM, ht, str, offset);
}

void wordcheck_create_mmtable(wcMM *MM)
{
    wcmmTable *ht = (wcmmTable *)wordcheck_mm_malloc(MM, sizeof(wcmmTable));
    ht->num = 0;
    ht->tbl_offset = MM->offset;
    wordcheck_mmhash_table_create(MM);
}

void wordcheck_create_mmblock(wcMM *MM)
{
    wcmmBlock *instance;
    instance = (wcmmBlock *)wordcheck_mm_malloc(MM, sizeof(wcBlock));
    memset(instance->word, '\0', 1);
    instance->aNum = 0;
    instance->info = 0;
    instance->cTbl_offset = MM->offset;
    wordcheck_create_mmtable(MM);
}
