#!/bin/sh
#File:x_bellmail.sh
#The assistant of x_aix5_bellmail.pl
#Author : watercloud@xfocus.org
#Date   :2004-6-6
#

X_BELL_PL="./x_aix5_bellmail.pl"
AIM=$1

if [ $# ne 1 ] ;then
        echo "Need a aim file name as argv."
        exit 1;
fi

if [ ! -e "$1" ];then
        echo "$1 not exist!"
        exit 1
fi
if [ ! -x "$X_BELL_PL" ];then
        echo "can not exec $X_BELL_PL"
        exit 1
fi

ret=`ls -l $AIM`
echo $ret; echo
fuser=`echo $ret |awk '{print $3}'`
while [ "$fuser" != "$LOGIN" ]
do
        $X_BELL_PL $AIM
        ret=`ls -l $AIM`
        echo $ret;echo
        fuser=`echo $ret |awk '{print $3}'`
done
echo $ret; echo
#EOF
