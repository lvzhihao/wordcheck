/**
 ** edwin.lzh@gmail.com
 **/

#include "wordcheck.h"

static void usage(void){
	printf("wordcheck tools: Version %0.2f\n", 0.40);
	printf("usage: wordcheck [help|info|create]\n");
	printf("options:\n");
	printf("\t -c \t\t docs file\n");
	printf("\t -n \t\t name\n");
	printf("\t -s \t\t mm size\n");
	printf("example: wordcheck info -n demo \n");
	printf("example: wordcheck create -c demo.txt -n demo -s 67108864\n");
}

static int create(unsigned char* ident, unsigned char* docs, uint mm_size)
{
	wcTable *table;
	wordcheck_create_table(&table);
	if(wordcheck_open_deny_file((const char*)docs, table) != WORDCHECK_SUCCESS){
        printf("%s don't open\n", docs);
        return WORDCHECK_BIN_FAILURE;
    }
    wcMMInfo *wcMMInfo;
    if(wordcheck_mminfo_create(&wcMMInfo, "test", mm_size) != WORDCHECK_SUCCESS){
        printf("%s don't create mminfo\n", ident);
        return WORDCHECK_BIN_FAILURE;
    } 
	wcMM *MM;
    if(wordcheck_mm_create(&MM, wcMMInfo, (const char*)ident) != WORDCHECK_SUCCESS){
        printf("%s don't create\n", ident);
        return WORDCHECK_BIN_FAILURE;
    }
	if(wordcheck_mmtable_create(MM, table) != WORDCHECK_SUCCESS){
        printf("%s failure\n", ident);
        return WORDCHECK_BIN_FAILURE;
    }
    return WORDCHECK_BIN_SUCCESS;
}

static void info(unsigned char* ident)
{
	wcMM *MM;
	if(wordcheck_mm_fetch(&MM, (const char*)ident) == WORDCHECK_SUCCESS){
        wcmmTable *mmtable;
        wordcheck_mmtable_fetch(MM, &mmtable);
        printf("Memory: %d (%0.2fM)\n", wordcheck_mm_size(MM), (double)wordcheck_mm_size(MM)/1024/1024);
        printf("Nums: %d\n", mmtable->num);
    }else{
        printf("%s don't created\n", ident);
    }
}

int main(int argc, char* argv[])
{  
	int count;
	extern char *optarg;
	extern int optind;
    unsigned char *ident = NULL;
    unsigned char *docs = NULL;
    uint mm_size;
    uint hash_sec;

    while((count = getopt(argc, argv, "c:n:s:")) != -1){
		switch (count)
		{
			case 'c':
                if(optarg != NULL){
                    docs = (unsigned char *)strdup(optarg);
                }
			break;
			case 'n':
                if(optarg != NULL){
                    ident = (unsigned char *)strdup(optarg);
                }
            break;
            case 's':
                if(optarg != NULL){
                    mm_size = (uint)atoi(strdup(optarg));
                }
			break;
			default:
				usage();
			break;
		}
	}

	if(argc - optind >= 1){
		if(strcmp(argv[optind], "help")==0){
			usage();
        }else if(strcmp(argv[optind], "create")==0){
            if(docs != NULL && ident != NULL){
                if(create(ident, docs, mm_size) == WORDCHECK_BIN_SUCCESS){
                    printf("%s Created!\n", ident);
                }
            }
        }else if(strcmp(argv[optind], "info")==0){
            info(ident);
        }
    }else{
        usage();
    }
    return 0;
}
