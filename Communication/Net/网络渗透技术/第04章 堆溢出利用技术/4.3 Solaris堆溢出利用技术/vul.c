/* - vul.c -
* Simple vulnerable program to demostrate free/malloc heap overflow in
* Solaris SPARC.
*
* by warning3 <warning3@nsfocus.com> 2001.6.9
*/

void
vulfunc(char *str)
{
        char           *m1, *m2, *m3;

        m1 = (void *) malloc(1024);
        m2 = (void *) malloc(2048);
        strcpy(m1, str); /* overflow */
        free(m2);        /* free m2 */         
        m3 = (void *) malloc(512); /* boom! */
        printf ("Try to free m1\n");
        free(m1);
        free(m3);

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
