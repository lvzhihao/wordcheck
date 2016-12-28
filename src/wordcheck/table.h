/**
 ** edwin.lzh@gmail.com
 **/

#include <stdint.h>

typedef struct wcTable {
    uint num;
    GHashTable *tbl;
} wcTable;

typedef struct wcBlock {
    char word[1];  /*字符*/
    uint16_t info; /*信息*/
    uint8_t aNum;  /*所有节点个数*/
    wcTable *cTbl; /*子节点*/
} wcBlock;

/*新建词典*/
void wordcheck_create_table(wcTable **table);

/*释放词典*/
void wordcheck_free_table(wcTable *table);

/*处理词库*/
void wordcheck_handle_table(char *words, uint16_t info, wcTable *table);

/*添加词*/
int wordcheck_insert_table(const char *words, uint16_t info, wcTable *table);

/*新建block*/
void wordcheck_create_block(wcBlock **block);

/*释放block*/
void wordcheck_free_block(wcBlock *block);
