/* vulnerable.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  Vulnerable program on the Solaris SPARC architecture.
*/

#include <stdio.h>
#include <string.h>

int overflow(char * str) {
    char smallbuff[16];
    strcpy (smallbuff, str);
    printf("%s\n", smallbuff);
    getchar(); // for debug
}

int main (int argc, char *argv[])
{
    if ( argc > 1 )
    {
        overflow(argv[1]);
    }
}
