/**
 ** edwin.lzh@gmail.com
 **/

typedef struct wcResult {
    char* string;
    wcWordInfo* info;
    uint start;
    uint len;
} wcResult;

wcResult* wordcheck_check_result_create(void);

void wordcheck_check_result_free(wcResult*);

int wordcheck_check(wcTable* wt, const char* filter, int filter_len, char** out, int* out_len, wcList** outList);

int wordcheck_handle_replace(char* txt, long start, long size);

int wordcheck_handle_check(const char* txt, wcTable* wt, int pos, wcWordInfo** resultInfo, char** buf, char** lastMatch);

int wordcheck_mm_check(wcMM* MM, wcmmTable* wt, const char* filter, int filter_len, char** out, int* out_len, wcList** outList);

int wordcheck_mm_handle_check(wcMM* MM, const char* txt, wcmmTable* wt, int pos, wcWordInfo** resultInfo, char** buf, char** lastMatch);

int wordcheck_set_replace_op(const char* op);

const char* wordcheck_get_replace_op(void);

int wordcheck_set_replace_len(int len);

int wordcheck_get_replace_len(void);

void wordcheck_reset_replace_op(void);

void wordcheck_reset_replace_len(void);
