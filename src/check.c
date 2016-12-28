/**
 ** edwin.lzh@gmail.com
 **/

#include "wordcheck/platform.h"
#include "wordcheck/table.h"
#include "wordcheck/docs.h"
#include "wordcheck/mm.h"
#include "wordcheck/hash.h"
#include "wordcheck/mmtable.h"
#include "wordcheck/check.h"

static unsigned char *wordcheck_replace_op = NULL;

static uint wordcheck_replace_len = WORDCHECK_DEFAULT_REP_LEN;

wcResult *wordcheck_check_result_create(void)
{
    wcResult *wc = (wcResult *)malloc(sizeof(wcResult));
    wc->string = NULL;
    wc->info = wordcheck_word_info_create();
    wc->start = 0;
    wc->len = 0;
    return wc;
}

void wordcheck_check_result_free(wcResult *result)
{
    wordcheck_word_info_free(result->info);
    free(result);
}

int wordcheck_check(wcTable *wt, const char *filter, int filter_len, char **out, int *out_len, wcList **outList)
{
    int num = 0;
    long ret;
    int pos = filter_len - 1;
    char *buf = malloc(filter_len + 1);
    char *lastMatch = malloc(filter_len + 1);
    char *rettxt = malloc(filter_len + 1);
    wcList *lt = wordcheck_list_create();
    memset(rettxt, '\0', filter_len + 1);
    strcpy(rettxt, filter);
    while (pos >= 0) {
        memset(buf, '\0', filter_len + 1);
        memset(lastMatch, '\0', filter_len + 1);
        wcResult *ltVal = wordcheck_check_result_create();
        ret = wordcheck_handle_check((const char *)rettxt, wt, pos, &(ltVal->info), &buf, &lastMatch);
        if (ret == WORDCHECK_SUCCESS) {
            wordcheck_utils_strreverse(lastMatch);
            num++;
            //printf("%s\n", lastMatch);
            //break;
            pos = pos - wordcheck_handle_replace(rettxt, pos, strlen(lastMatch));
            if (outList != NULL) {
                char *ltKey = (char *)malloc(strlen(lastMatch) + 1);
                memset(ltKey, '\0', strlen(lastMatch) + 1);
                strncpy(ltKey, lastMatch, strlen(lastMatch));
                ltVal->len = strlen(lastMatch);
                ltVal->string = (char *)malloc(ltVal->len + 1);
                memset(ltVal->string, '\0', ltVal->len + 1);
                strncpy(ltVal->string, lastMatch, ltVal->len);
                ltVal->start = pos + 1;
                lt = wordcheck_list_insert(lt, ltKey, ltVal);
            }
        } else {
            pos = pos - 1;
            free(ltVal);
        }
    }
    if (outList != NULL) {
        *outList = wordcheck_list_reverse(lt);
    } else {
        free(lt);
    }
    *out = rettxt;
    *out_len = strlen(rettxt);
    return num;
}

int wordcheck_mm_check(wcMM *MM, wcmmTable *wt, const char *filter, int filter_len, char **out, int *out_len, wcList **outList)
{
    int num = 0;
    long ret;
    int pos = filter_len - 1;
    char *buf = malloc(filter_len + 1);
    char *lastMatch = malloc(filter_len + 1);
    char *rettxt = malloc(filter_len + 1);
    wcList *lt = wordcheck_list_create();
    memset(rettxt, '\0', filter_len + 1);
    strncpy(rettxt, filter, filter_len);
    while (pos >= 0) {
        memset(buf, '\0', filter_len + 1);
        memset(lastMatch, '\0', filter_len + 1);
        wcResult *ltVal = wordcheck_check_result_create();
        ret = wordcheck_mm_handle_check(MM, (const char *)rettxt, wt, pos, &(ltVal->info), &buf, &lastMatch);
        if (ret == 0) {
            wordcheck_utils_strreverse(lastMatch);
            num++;
            //printf("%s\n", lastMatch);
            //break;
            pos = pos - wordcheck_handle_replace(rettxt, pos, strlen(lastMatch));
            if (outList != NULL) {
                char *ltKey = (char *)malloc(strlen(lastMatch) + 1);
                memset(ltKey, '\0', strlen(lastMatch) + 1);
                strncpy(ltKey, lastMatch, strlen(lastMatch));
                ltVal->len = strlen(lastMatch);
                ltVal->string = (char *)malloc(ltVal->len + 1);
                memset(ltVal->string, '\0', ltVal->len + 1);
                strncpy(ltVal->string, lastMatch, ltVal->len);
                ltVal->start = pos + 1;
                lt = wordcheck_list_insert(lt, ltKey, ltVal);
            }
        } else {
            pos = pos - 1;
            free(ltVal);
        }
    }
    if (outList != NULL) {
        *outList = wordcheck_list_reverse(lt);
    } else {
        free(lt);
    }
    *out = rettxt;
    *out_len = strlen(rettxt);
    return num;
}

int wordcheck_handle_replace(char *txt, long start, long size)
{
    int i, len = strlen(txt);
    for (i = 0; i < size; i++) {
        txt[start - i] = *wordcheck_get_replace_op();
        //替换符定长判断
        if (WORDCHECK_DEFAULT_REP_LEN > 0 && i >= wordcheck_replace_len - 1) {
            //todo pre
            char *pre = malloc(len + 1);
            memset(pre, '\0', len + 1);
            strncpy(pre, txt, start - size + 1);
            //printf("%s\n===\n", pre);
            //todo post
            char *post = malloc(len + 1);
            memset(post, '\0', len + 1);
            strncpy(post, txt + start - i, len - start + i);
            //printf("%s\n==\n", post);
            //todo txt
            memset(txt, '\0', len + 1);
            strcat(txt, pre);
            strcat(txt, post);
            free(pre);
            free(post);
            //printf("%s\n", txt);
            break;
        }
    }
    return size;
}

int wordcheck_handle_check(const char *txt, wcTable *wt, int pos, wcWordInfo **resultInfo, char **buf, char **lastMatch)
{
    wcBlock *block = (wcBlock *)malloc(sizeof(wcBlock));
    char *buffer = malloc(1);
    memset(buffer, '\0', 1);
    memset(buffer, txt[pos], 1);
    strcat(*buf, buffer);
    wordcheck_utils_strtoupper(buffer);  //不区分大小写
    if (g_hash_table_lookup_extended(wt->tbl, buffer, NULL, (void **)&block) == TRUE) {
        if (block->aNum > 0) {
            if (block->info > 0) {
                memset(*lastMatch, '\0', strlen(*lastMatch) + 1);
                strcpy(*lastMatch, *buf);
                *resultInfo = wordcheck_word_info_decode(block->info);
                if (pos == 0) {
                    return WORDCHECK_SUCCESS;
                }
            }  //保留最后一个匹配结果
            if (pos > 0) {
                return wordcheck_handle_check(txt, block->cTbl, pos - 1, resultInfo, buf, lastMatch);
            } else {
                return WORDCHECK_FAILURE;
            }
        } else {
            memset(*lastMatch, '\0', strlen(*lastMatch) + 1);
            strcpy(*lastMatch, *buf);
            *resultInfo = wordcheck_word_info_decode(block->info);
            return WORDCHECK_SUCCESS;
        }
    } else {
        if (strlen(*lastMatch) > 0) {
            //如果有匹配
            return WORDCHECK_SUCCESS;
        } else {
            return WORDCHECK_FAILURE;
        }
    }
}

int wordcheck_mm_handle_check(wcMM *MM, const char *txt, wcmmTable *wt, int pos, wcWordInfo **resultInfo, char **buf, char **lastMatch)
{
    char *buffer = malloc(1);
    memset(buffer, '\0', 1);
    memset(buffer, txt[pos], 1);
    strcat(*buf, buffer);
    uint offset;
    wcmmHash *ht = (wcmmHash *)(MM + wt->tbl_offset);
    wordcheck_utils_strtoupper(buffer);  //不区分大小写
    if (wordcheck_mmhash_table_lookup(MM, ht, buffer, &offset) == WORDCHECK_SUCCESS) {
        wcmmBlock *block = (wcmmBlock *)(MM + offset);
        if (block->aNum > 0) {
            if (block->info > 0) {
                memset(*lastMatch, '\0', strlen(*lastMatch) + 1);
                strcpy(*lastMatch, *buf);
                *resultInfo = wordcheck_word_info_decode(block->info);
                if (pos == 0) {
                    return WORDCHECK_SUCCESS;
                }
            }  //保留最后一个匹配结果
            wcmmTable *table = (wcmmTable *)(MM + block->cTbl_offset);
            if (pos > 0) {
                return wordcheck_mm_handle_check(MM, txt, table, pos - 1, resultInfo, buf, lastMatch);
            } else {
                return WORDCHECK_FAILURE;
            }
        } else {
            memset(*lastMatch, '\0', strlen(*lastMatch) + 1);
            strcpy(*lastMatch, *buf);
            *resultInfo = wordcheck_word_info_decode(block->info);
            return WORDCHECK_SUCCESS;
        }
    } else {
        if (strlen(*lastMatch) > 0) {
            //如果有匹配
            return WORDCHECK_SUCCESS;
        } else {
            return WORDCHECK_FAILURE;
        }
    }
}

int wordcheck_set_replace_op(const char *op)
{
    char *buffer = malloc(4);
    snprintf(buffer, 4, "%d", op[0]);
    int ascii = atoi(buffer);
    free(buffer);
    if (ascii >= 33) {  //可见
        wordcheck_replace_op = (unsigned char *)malloc(2);
        memset(wordcheck_replace_op, '\0', 2);
        memset(wordcheck_replace_op, op[0], 1);
        return WORDCHECK_SUCCESS;
    } else {
        return WORDCHECK_FAILURE;
    }
}

const char *wordcheck_get_replace_op(void)
{
    if (wordcheck_replace_op != NULL) {
        return (const char *)wordcheck_replace_op;
    } else {
        unsigned char *op = (unsigned char *)malloc(2);
        memset(op, '\0', 2);
        memset(op, WORDCHECK_DEFAULT_REP_OP, 1);
        return (const char *)op;
    }
}

int wordcheck_set_replace_len(int len)
{
    if (len > 0) {
        wordcheck_replace_len = len;
        return WORDCHECK_SUCCESS;
    } else {
        return WORDCHECK_FAILURE;
    }
}

int wordcheck_get_replace_len(void)
{
    return wordcheck_replace_len;
}

void wordcheck_reset_replace_op(void)
{
    wordcheck_replace_op = NULL;
}

void wordcheck_reset_replace_len(void)
{
    wordcheck_replace_len = WORDCHECK_DEFAULT_REP_LEN;
}
