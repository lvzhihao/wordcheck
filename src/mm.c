/**
 ** edwin.lzh@gmail.com
 **/

#include "wordcheck/platform.h"
#include "wordcheck/table.h"
#include "wordcheck/docs.h"
#include "wordcheck/mm.h"

int wordcheck_mminfo_create(wcMMInfo** mmInfo, const char* memo, uint mm_size)
{
    wcMMInfo* instance;
    instance = (wcMMInfo*)malloc(sizeof(wcMMInfo));
    memset(instance->version, 0, 21);
    if (strlen(WORDCHECK_VERSION) > 20) {
        memcpy(instance->version, WORDCHECK_VERSION, 20);
    } else {
        memcpy(instance->version, WORDCHECK_VERSION, strlen(WORDCHECK_VERSION));
    }
    memset(instance->memo, 0, 51);
    if (strlen(memo) > 50) {
        memcpy(instance->memo, memo, 50);
    } else {
        memcpy(instance->memo, memo, strlen(memo));
    }
    if (mm_size > 0) {
        instance->mm_size = mm_size;
    } else {
        instance->mm_size = WORDCHECK_DEFAULT_MM_SIZE;
    }
    *mmInfo = instance;
    return WORDCHECK_SUCCESS;
}

int wordcheck_mminfo_fetch(wcMMInfo** mmInfo, wcMM* MM)
{
    *mmInfo = (wcMMInfo*)(MM + WORDCHECK_MMAP_PREFIX);
    return WORDCHECK_SUCCESS;
}

int wordcheck_mm_create(wcMM** MM, wcMMInfo* mmInfo, const char* mm)
{
    int fd;
    int fc;
    char shared_mm_file[255];
    strcpy(shared_mm_file, DEFAULT_WORDCHECK_DIR);
    DIR* dirp = opendir(shared_mm_file);
    if (dirp == NULL) {
        mkdir(shared_mm_file, WORDCHECK_DIR_MODE);
        chmod(shared_mm_file, 00777); /* todo: set dir umask, It's Can be change */
    }
    strcat(strcat(shared_mm_file, "/"), mm);
    fd = open(shared_mm_file, O_RDWR | O_TRUNC, WORDCHECK_FILE_MODE);
    if (fd < 0) {
        fd = open(shared_mm_file, O_RDWR | O_CREAT, WORDCHECK_FILE_MODE);
        if (fd < 0) {
            return WORDCHECK_FAILURE;
        }
    }
    //printf("mm create\n");
    //printf("version: %s\n", mmInfo->version);
    //printf("memo: %s\n", mmInfo->memo);
    //printf("size: %d\n", mmInfo->mm_size);

    fc = ftruncate(fd, mmInfo->mm_size);
    if (fc == 0) {
        chmod(shared_mm_file, 00777); /* todo: set file umask, It's Can be change */
        struct wordcheckMM* ptr = (struct wordcheckMM*)mmap(NULL, mmInfo->mm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
        memcpy(ptr + WORDCHECK_MMAP_PREFIX, mmInfo, sizeof(struct wordcheckMMInfo));
        ptr->offset = WORDCHECK_MMAP_PREFIX + sizeof(struct wordcheckMMInfo);  //Set MM Info
        *MM = ptr;
        return WORDCHECK_SUCCESS;
    } else {
        return WORDCHECK_FAILURE;
    }
    //todo: fix failure
}

int wordcheck_mm_fetch(wcMM** MM, const char* mm)
{
    /*
	va_list arg_ptr;
	void* mmap_start;
	va_start(arg_ptr, argnum);
    if(argnum > 0){
        mmap_start = va_arg(arg_ptr, void*);
    }else{
        mmap_start = NULL;
    }
	va_end(arg_ptr);
    */

    int fd;
    char shared_mm_file[255];
    strcpy(shared_mm_file, DEFAULT_WORDCHECK_DIR);
    DIR* dirp = opendir(shared_mm_file);
    if (dirp == NULL) {
        return WORDCHECK_FAILURE;
    }
    strcat(strcat(shared_mm_file, "/"), mm);
    fd = open(shared_mm_file, O_RDWR, WORDCHECK_FILE_MODE);
    if (fd < 0) {
        return WORDCHECK_FAILURE;
    }
    FILE* fp = fopen(shared_mm_file, "r");
    if (fp == NULL) {
        return WORDCHECK_FAILURE;
    }
    fseek(fp, 0L, SEEK_END);
    long mm_size = ftell(fp);
    //printf("File Size:  %ld", mm_size);
    fclose(fp);

    struct wordcheckMM* ptr = (struct wordcheckMM*)mmap(NULL, mm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    *MM = ptr;
    return WORDCHECK_SUCCESS;
}

int wordcheck_mm_munmap(wcMM* MM)
{
    wcMMInfo* mmInfo;
    wordcheck_mminfo_fetch(&mmInfo, MM);
    if (munmap(MM, mmInfo->mm_size) == 0) {
        return WORDCHECK_SUCCESS;
    } else {
        return WORDCHECK_FAILURE;
    }
}

void* wordcheck_mm_malloc(wcMM* MM, int len)
{
    uint old = MM->offset;
    MM->offset += len;
    return (void*)(MM + old);
}

int wordcheck_mm_size(wcMM* MM)
{
    return (int)MM->offset * 8;
}
