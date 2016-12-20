/**
 ** edwin.lzh@gmail.com
 **/

#include "wordcheck/platform.h"
#include "wordcheck/utils.h"

char *wordcheck_utils_ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *wordcheck_utils_rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *wordcheck_utils_trim(char *s)
{
    return wordcheck_utils_rtrim(wordcheck_utils_ltrim(s)); 
}

void wordcheck_utils_strtoupper(char *str){ 
	int len = strlen(str), i;
	for(i=0;i<len;i++){
		if(str[i]>='a' && str[i]<='z'){
			str[i] = str[i]-'a'+'A'; 
		}
	}
}

void wordcheck_utils_strtolower(char *str){
	int len = strlen(str), i;
	for(i=0;i<len;i++){
		if(str[i]>='A' && str[i]<='Z'){
			str[i] = str[i]-'A'+'a';
		}
	}
}

void wordcheck_utils_strreverse(char *str){
	int len, i;
	char *buf;
	len = strlen(str);
	buf = malloc(len+1);
	memset(buf, 0, len+1);
	for(i=len-1; i>=0; i--){
		buf[len-1-i] = str[i];
	}
	memset(str, 0, len+1);
	memcpy(str, buf, len);
}

int wordcheck_utils_strpos(char *haystack, char *needle)
{
   char *p = strstr(haystack, needle);
   if (p)
      return p - haystack;
   return -1;
}

