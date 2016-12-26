/**
 ** edwin.lzh@gmail.com
 **/

#include <stdint.h>

typedef struct wcTable
{
	int num;
	GHashTable *tbl;
} wcTable;

typedef struct wcBlock 
{
	char word[2];		/*字符*/
	uint8_t isDeny;		/*此节为是否已经为一个deny字*/
	uint aNum;			/*所有节点个数*/
	wcTable *cTbl;		/*子节点*/
} wcBlock;

/*新建词典*/
void wordcheck_create_table(wcTable **table);

/*释放词典*/
void wordcheck_free_table(wcTable *table);

/*处理词库*/
void wordcheck_handle_table(char* words, wcTable *table);					

/*添加词*/
int wordcheck_insert_table(const char *words,  wcTable *table);

/*新建block*/
void wordcheck_create_block(wcBlock **block);

/*释放block*/
void wordcheck_free_block(wcBlock *block);
