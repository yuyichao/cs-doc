/* kformat.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  内核格式化串漏洞演示程序
*  gcc -O3 -c -I/usr/src/linux/include kformat.c
*/

#define MODULE
#define __KERNEL__
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>
#include <sys/syscall.h>
#include <linux/slab.h>

#define __NR_function   240     //linux not use

extern void* sys_call_table[];

int (*old_function) (void );

static char buffer[256];

asmlinkage int new_function(unsigned int len, char * buf) {
        char   code[256] ;

        if (len > 256) len = 256;

        if (copy_from_user(code, buf, len))
                goto out;

        snprintf(buffer,len,code);
        printk("%s",buffer);

out:
        return 0;
}

int init_module(void) {
  old_function = sys_call_table[__NR_function];
  sys_call_table[__NR_function] = new_function;
  printk("<1>kformat test loaded...\n");
  return 0;
}

void cleanup_module(void) {
  sys_call_table[__NR_function] = old_function;
  printk("<1>kformat test unloaded...\n");
}
