/* format.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  存在格式化串漏洞的演示程序
*/

#include <stdio.h>
#define IOSIZE 1024

int main(int argc, char **argv )
{
    FILE * binFileH;
    char binFile[] = "binfile";
    char buf[IOSIZE];

    if ( (binFileH = fopen(binFile, "rb")) == NULL )
    {
        printf("can't open file %s!\n", binFile);
        exit();
    }

    memset(buf, 0, sizeof(buf));
    fread(buf, sizeof(char), IOSIZE, binFileH);

    printf("%d\n", strlen(buf));
    printf(buf);

    fclose(binFileH);
}
