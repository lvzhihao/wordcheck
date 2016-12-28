/**
 ** edwin.lzh@gmail.com
 **/

#include <stdint.h>

typedef struct wcmmTable {
    int num;
    uint tbl_offset;
} wcmmTable;

typedef struct wcmmBlock {
    char word[1];     /*字符*/
    uint16_t info;    /*信息*/
    uint8_t aNum;     /*所有节点个数*/
    uint cTbl_offset; /*子节点*/
} wcmmBlock;

/*创建wcmmtable*/
int wordcheck_mmtable_create(wcMM* MM, wcTable* table);

/*获取wcmmtable*/
int wordcheck_mmtable_fetch(wcMM* MM, wcmmTable** table);

/*转换wctable至wcmmtable*/
wcmmTable* wordcheck_mmtable_create_from_table(wcMM* MM, wcTable* table);

/*新建mmtable*/
void wordcheck_create_mmtable(wcMM* MM);

/*新建block*/
void wordcheck_create_mmblock(wcMM* MM);

/*foreach*/
void wordcheck_mmtable_convert(gpointer key, gpointer val, gpointer data);
