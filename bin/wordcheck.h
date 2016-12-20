/**
 ** edwin.lzh@gmail.com
 **/

#include "wordcheck/platform.h"
#include "wordcheck/table.h"
#include "wordcheck/mm.h"
#include "wordcheck/hash.h"
#include "wordcheck/mmtable.h"
#include "wordcheck/docs.h"
#include "wordcheck/check.h"

#define WORDCHECK_BIN_SUCCESS 0
#define WORDCHECK_BIN_FAILURE 1

static void usage(void);
static int open_docs(const char* docs, char** filterTxt);
static int create(unsigned char* ident, unsigned char* docs, uint mm_size);
static void info(unsigned char* ident);
