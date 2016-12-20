/**
 ** edwin.lzh@gmail.com
 **/

#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <assert.h>
#include <dirent.h>
#include <stdarg.h>
#include <ctype.h>
#include <unistd.h>

#include "utils.h"

#ifndef WORDCHECK_VERSION
#define WORDCHECK_VERSION "1.0.0"	/*version*/
#endif

#ifndef WORDCHECK_MM_HASH_SEC
#define WORDCHECK_MM_HASH_SEC   16 
#endif

#define WORDCHECK_SUCCESS 0		/*成功*/
#define WORDCHECK_FAILURE 1		/*失败*/

#define WORDCHECK_MMAP_PREFIX    20
#define WORDCHECK_DEFAULT_MM_SIZE      67108864    //64M

#define WORDCHECK_DEFAULT_REP_OP '*'
#define WORDCHECK_DEFAULT_REP_LEN 6

