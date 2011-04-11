/* vul.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  存在堆溢出漏洞的演示程序
*/

#include <stdio.h>

int main(int argc, char *argv[]) {
    char *p0 = (char *) malloc(16);
    char *p1 = (char *) malloc(16);

    if (argc > 1)
        strcpy(p0, argv[1]);

    printf("Before free p0.\n");
    free(p0);
    printf("Before free p1.\n");
    free(p1);
}
