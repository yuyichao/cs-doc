#include <stdlib.h>
#include <string.h>
int main(int argc, char ** argv)
{
        char buf[16];
	char buf1[32];//需要加上这个，不然指令add    xxx,%esp将跳不过mmap函数的参数总共大小。
		      //或者说定义下面的 char pad3[8 + POPNUM - sizeof(struct mmap_args)];
		      //就会出错，也就是会出现8 + POPNUM - sizeof(struct mmap_args) <0的情况
		
        if (argc==2)
                strcpy(buf,argv[1]);
}
