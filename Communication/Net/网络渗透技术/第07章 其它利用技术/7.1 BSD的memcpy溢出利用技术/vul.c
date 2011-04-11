/* vul.c
* 
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  *bsd memcpy bug demo
*/

#include <fcntl.h>
#define BUFSIZE 1024

int main(int argc,char ** argv)
{
    char buf[BUFSIZE];
    char buf1[BUFSIZE];
    char buf2[BUFSIZE];
    int i, fp;

    fp = open(argv[1], O_RDONLY);
    read(fp,buf1,BUFSIZE);
    close(fp);
    
    i = atoi(argv[2]);

    memcpy(buf2-1,buf,i);
    exit(0);
}
