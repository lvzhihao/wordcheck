/**
 ** edwin.lzh@gmail.com
 **/

typedef struct wcmmNode {
    char word[2];
    uint block_offset;
    uint next_offset;
} wcmmNode;

typedef struct wcmmHash {
    uint offset[WORDCHECK_MM_HASH_SEC];
} wcmmHash;

typedef struct wcList {
    void* key;
    void* val;
    void* pre;
    void* next;
} wcList;

uint wordcheck_mmhash_get_str(const char* str);

wcmmHash* wordcheck_mmhash_table_create(wcMM* MM);

//void wordcheck_mmhash_table_destroy(wcmmHash* ht);	//TODO

int wordcheck_mmhash_table_insert(wcMM* MM, wcmmHash* ht, const char* key, uint val);

//int wordcheck_mmhash_table_update(wcMM* MM, wcmmHash* ht, const char* key, void* val);	//TODO

//int wordcheck_mmhash_table_remove(wcMM* MM, wcmmHash* ht, const char* key);		//TODO

int wordcheck_mmhash_table_lookup(wcMM* MM, wcmmHash* ht, const char* key, uint* val);

wcmmNode* wordcheck_mmnode_list_create(wcMM* MM);

//void wordcheck_mmnode_list_destroy(wcMM* MM, wcmmNode* list);	//TODO

int wordcheck_mmnode_list_insert(wcMM* MM, wcmmNode* list, const char* key, uint val);

//int wordcheck_mmnode_list_update(wcMM* MM, wcmmNode* list, const char* key, void* val);	//TODO

//int wordcheck_mmnode_list_remove(wcMM* MM, wcmmNode* list, const char* key);		//TODO

int wordcheck_mmnode_list_lookup(wcMM* MM, wcmmNode* list, const char* key, uint* val);

wcList* wordcheck_list_create(void);

//TODO: 暂时程序就认为key是字符串，没有判断类型
wcList* wordcheck_list_insert(wcList* list, void* key, void* val);

wcList* wordcheck_list_lookup(wcList* list, void* key, void** val);

wcList* wordcheck_list_rewind(wcList* list);

wcList* wordcheck_list_last(wcList* list);

wcList* wordcheck_list_reverse(wcList* list);

int wordcheck_list_get_current(wcList* list, wcList** lt);

int wordcheck_list_next_item(wcList** list);
