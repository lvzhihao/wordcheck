/**
 ** edwin.lzh@gmail.com
 **/


/*打开deny file*/
int wordcheck_open_deny_file(const char *file, wcTable *table);

/*添加deny words*/
int wordcheck_insert_deny(const char *words, wcTable *table);

