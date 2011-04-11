/* double-free.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  存在两次释放漏洞的演示程序
*/

#include <stdio.h>

int main(int argc, char *argv[])
{
    char *p0 = (char *) malloc(8);
    char *p1 = (char *) malloc(8);
    char *p2, *p3;

    free(p0);
    free(p0);

    p2 = (char *) malloc(8);
    if (argc > 1)
        strcpy(p2, argv[1]);
    p3 = (char *) malloc(8);
}
