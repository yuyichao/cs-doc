/* fmt.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  存在格式化串漏洞的演示程序
*  演示平台：Solaris SPARC 7
*/

int main (int argc, char **argv)
{
    char buf[512];

    strncpy(buf, argv[1], sizeof(buf)-1);
    printf(buf);
    return 0;
}
