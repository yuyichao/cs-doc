/* frees.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  Solaris释放堆利用演示
*/

void
vulfunc(char *str)
{
        char    *ff[32];
        int     i;
        char    *m1, *m2;

        m1 = (void *) malloc(1024);
        m2 = (void *) malloc(2048);

        for (i=0; i<32; i++) {
            ff[i] = (void *)malloc(64+i*4);
        }

        strcpy(ff[0], str); /* overflow */

        for (i=0; i<32; i++) {
            free(ff[i]);
        }

        free(m1);
        free(m2);        /* boom! */

        printf ("Free all blocks\n");
}

int
main(int argc, char **argv)
{
        if (argc > 1)
                vulfunc(argv[1]);
        else {
                printf("No enough aruments\n");
                exit(0);
        }
}
