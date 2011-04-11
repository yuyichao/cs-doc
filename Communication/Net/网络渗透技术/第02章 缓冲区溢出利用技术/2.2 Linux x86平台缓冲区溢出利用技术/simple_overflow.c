/* simple_overflow.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  Simple program to demonstrate buffer overflows
*  on the IA32 architecture.
*/

#include <stdio.h>
#include <string.h>
char largebuff[] =
"1234512345123451234512345===ABCD";
int main (void)
{
    char smallbuff[16];
    strcpy (smallbuff, largebuff);
}
