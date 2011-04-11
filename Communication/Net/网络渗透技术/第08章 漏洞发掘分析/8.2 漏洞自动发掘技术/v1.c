#include<stdio.h>
int main(int argc,char  * argv[])
{
        char buff[32];
        if(argc > 1)
        {
                strcpy(buff,argv[1]);
        }
        printf("buff : %s\n",buff);
        return 0;
}
