/* t.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  初始化数据区溢出演示程序
*/

#include<stdio.h>
int x = 5;
void main(void)
{
        char *p="Hello World";  /* p指向在已初始化数据区 */
        int i=0;
        char buff[1024];
        for(i=0;i<1024;buff[i++]='A');
        buff[1023]=0;
        strcpy(p,buff); /*这个拷贝将覆盖PLT中存放strcpy函数地址的内存单元 */
        strcpy(p,buff); /* 这次调用将会转到0x41414140处 (指令地址需4字节对齐，*/
                        /* 如果没有对齐会自动对齐而不会出错：) */
}
