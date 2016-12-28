/**
 ** edwin.lzh@gmail.com
 **/

#ifdef MACOS
#define DEFAULT_WORDCHECK_DIR "/tmp/wordcheck"
#else
#define DEFAULT_WORDCHECK_DIR "/dev/shm/wordcheck"
#endif
#define WORDCHECK_DIR_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)  /* DIR MODE */
#define WORDCHECK_FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) /* FILE MODE */

typedef struct wordcheckMMInfo {
    char version[21];  //版本
    char memo[51];     //介绍
    uint mm_size;      //大小
} wcMMInfo;

// 共享内存
typedef struct wordcheckMM {
    //sem_t mutex;              /* the mutex: a Posix memory-based semaphore */
    uint offset;        /* MM size offset */
    uint mmtable_start; /* mmtable start */
} wcMM;

/*创建共享信息*/
int wordcheck_mminfo_create(wcMMInfo** mmInfo, const char* memo, uint mm_size);

/*获取共享信息*/
int wordcheck_mminfo_fetch(wcMMInfo** mmInfo, wcMM* MM);

/*创造共享内存*/
int wordcheck_mm_create(wcMM** MM, wcMMInfo* mmInfo, const char* mm);

/*获取共享内存*/
int wordcheck_mm_fetch(wcMM** MM, const char* mm);

/*释放共享内存*/
int wordcheck_mm_munmap(wcMM* MM);

/*分配共享内存空间*/
void* wordcheck_mm_malloc(wcMM* MM, int len);

/*获取共享内存大小*/
int wordcheck_mm_size(wcMM* MM);
