/**
 ** edwin.lzh@gmail.com
 **/

#include <stdint.h>

typedef struct wcWordInfo {
    uint8_t weight; /* 权重，默认为1，范围1~255*/
} wcWordInfo;

wcWordInfo* wordcheck_word_info_create(void);

void wordcheck_word_info_free(wcWordInfo* info);

uint16_t wordcheck_word_info_encode(wcWordInfo* info);

wcWordInfo* wordcheck_word_info_decode(uint16_t info);

/*打开deny file*/
int wordcheck_open_deny_file(const char* file, wcTable* table);

/*添加deny words*/
int wordcheck_insert_deny(const char* words, wcTable* table);
