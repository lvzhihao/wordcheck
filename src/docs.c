/**
 ** edwin.lzh@gmail.com
 **/

#include "wordcheck/platform.h"
#include "wordcheck/table.h"
#include "wordcheck/docs.h"

int wordcheck_open_deny_file(const char *file, wcTable *table)
{
    FILE *fp;
    char *buffer = malloc(1024);
    char *line = malloc(1024);

    fp = fopen(file, "r");
    if (fp == NULL) return WORDCHECK_FAILURE;
    while (fgets(buffer, 1024, fp) != NULL) {
        memset(line, '\0', 1024);
        strcpy(line, wordcheck_utils_trim(buffer));
        if (strcmp(line, "") != 0) {
            //printf("%s\n", words);
            wordcheck_insert_deny(line, table);
        }
    }
    free(buffer);
    free(line);
    fclose(fp);
    return WORDCHECK_SUCCESS;
}

int wordcheck_insert_deny(const char *line, wcTable *table)
{
    wcWordInfo *info = wordcheck_word_info_create();
    return wordcheck_insert_table(line, wordcheck_word_info_encode(info), table);
}

wcWordInfo *wordcheck_word_info_create(void)
{
    wcWordInfo *info = (wcWordInfo *)malloc(sizeof(wcWordInfo));
    info->weight = 1;
    return info;
}

void wordcheck_word_info_free(wcWordInfo *info)
{
    free(info);
}

uint16_t wordcheck_word_info_encode(wcWordInfo *info)
{
    //todo
    return (uint16_t)info->weight;
}

wcWordInfo *wordcheck_word_info_decode(uint16_t info)
{
    wcWordInfo *wcInfo = wordcheck_word_info_create();
    wcInfo->weight = (uint8_t)info;
    return wcInfo;
}
