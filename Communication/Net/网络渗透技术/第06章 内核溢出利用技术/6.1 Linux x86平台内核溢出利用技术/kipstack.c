/* kipstack.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  存在TCP/IP协议栈漏洞的演示程序
*  gcc -O3 -c -I/usr/src/linux/include kipstack.c
*/

#define MODULE
#define __KERNEL__

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/string.h>
#include <linux/inet.h>
#include <linux/sockios.h>
#include <linux/net.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/in.h>

struct packet_type  proto;

int do_with(unsigned char *data,unsigned int len)
{
        char buf[256];
#ifdef DEBUG
        unsigned int mylen;
        mylen = len >256?256:len;
        if (mylen ==0) return 0;
        memcpy(buf,data,mylen);
        buf[mylen-1]=0;
        printk("data len %d\n---------\n%s\n",len,buf);
#else
        memcpy(buf,data,len);
#endif
        /*do something else*/
}
int func(struct sk_buff *skb, struct net_device *dv, struct packet_type *pt)
{
char buffer[512];//only for Exploit easy

        /* fix some pointers */
        skb->h.raw = skb->nh.raw + skb->nh.iph->ihl*4;
        skb->data = (unsigned char *)skb->h.raw + (skb->h.th->doff << 2);
        skb->len -= skb->nh.iph->ihl*4 + (skb->h.th->doff << 2) ;

       if ((skb->nh.iph->protocol != IPPROTO_TCP)&&(skb->nh.iph->protocol != IPPROTO_UDP))
                goto pkt_out;

        if( 65500 != ntohs(skb->h.th->dest) )
                goto pkt_out;

        do_with(skb->data,skb->len);

pkt_out:
        kfree_skb(skb);
        return 0;

}
int init_module(void)
{

  proto.type=htons(ETH_P_ALL);
  proto.func=func;
  dev_add_pack(&proto);
  printk("my network proto loaded\n");
  return 0;
}
void cleanup_module(void)
{
  dev_remove_pack(&proto);
  printk("Unload network proto.\n");
  return;
}
