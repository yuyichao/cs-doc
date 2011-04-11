#include <stdlib.h>
#include <string.h>
char spare[1024+200];//加了200个bytes,不然在我实验的环境下FRAMESINDATA为0x8049a00(包含\0)
char bigbuf[1024];

int
main(int argc, char ** argv)
{
	char buf[16];
	char * ptr=getenv("STR");
	if (ptr) {
		bigbuf[0]=0;
		strncat(bigbuf, ptr, sizeof(bigbuf)-1);
	}
	ptr=getenv("LNG");
	if (ptr)
		 strcpy(buf, ptr);	
}
