beginfig(2);
u:=.7cm;
drawoptions(withcolor black);
pickup pencircle scaled 1pt;
for i=0 upto 11: draw (i*u,0)--(i*u,7*u) dashed withdots; endfor;
for i=0 upto 7: draw (0,i*u)--(11u,i*u)  dashed withdots; endfor;
pair a,b;
a=(u,4u); b=(u,2u);
pickup pencircle scaled 2pt;
draw (u,0)--b--(8u,2u)--(8u,7u);
draw (3u,0)--(3u,7u);
draw (u,7u)--a--(9u,4u)--(9u,7u);
draw (5u,0)--(5u,6u)--(7u,6u)--(7u,7u);
draw (8u,0)--(8u,u)--(10u,u)--(10u,0);

pickup pencircle scaled 3.5pt;
draw (4u,0)--(4u,3u)--(10u,3u)--(10u,7u) withcolor blue;
draw (0,7u)--(11u,7u);
draw (0,0)--(11u,0);

label.llft("A", a);
label.lft("B", b);

t:= 8u;

%Column and track labels
for i=1 upto 11: z[i]=(i*u,t); endfor;
labels(1,2,3,4,5,6,7,8,9,10,11);
label(btex $t_1$ etex, (12u,6u));
label(btex $t_2$ etex, (12u,5u));
label(btex $t_3$ etex, (12u,4u));
label(btex $t_4$ etex, (12u,3u));
label(btex $t_5$ etex, (12u,2u));
label(btex $t_6$ etex, (12u,1u));

%exchange arrow for t2 and t3

pickup pencircle scaled .5pt;
drawdblarrow (12u,5u)+(.3u,0){dir -30}..{dir 210}(12u,4u)+(.3u,0);

%text labels
label.lft("Column", (-1u,t));
label.lft("Net", (-1u,t)-(0,u));
label.rt("Track", (11u,7u));

%labels for crosstalk value
drawoptions(withcolor blue);
defaultscale:=1.3;
label.top("5",(6.5u,3u));
label.top("4",(6.5u,2u));
label.lft("3",(4u,1.5u));
label.rt("3",(4u,1.5u));
label.lft("3",(10u,5.5u));

%net labels
drawoptions();
defaultscale:=1;
label.bot("5",(u,0));
label.bot("2",(3u,0));
label.bot("4",(4u,0));
label.bot("3",(5u,0));
label.bot("6",(8u,0));
label.bot("6",(10u,0));
label.top("1",(u,7u));
label.top("2",(3u,7u));
label.top("3",(7u,7u));
label.top("5",(8u,7u));
label.top("1",(9u,7u));
label.top("4",(10u,7u));

endfig;

beginfig(3);
u:=.7cm;
drawoptions(withcolor black);
pickup pencircle scaled 1pt;
for i=0 upto 11: draw (i*u,0)--(i*u,7*u) dashed withdots; endfor;
for i=0 upto 7: draw (0,i*u)--(11u,i*u)  dashed withdots; endfor;
pair a,b;
a=(u,5u); b=(u,2u);
pickup pencircle scaled 2pt;
draw (u,0)--b--(8u,2u)--(8u,7u);
draw (3u,0)--(3u,7u);
draw (u,7u)--a--(9u,ypart a)--(9u,7u);
draw (5u,0)--(5u,6u)--(7u,6u)--(7u,7u);
draw (8u,0)--(8u,u)--(10u,u)--(10u,0);

pickup pencircle scaled 3.5pt;
draw (4u,0)--(4u,3u)--(10u,3u)--(10u,7u) withcolor blue;
draw (0,7u)--(11u,7u);
draw (0,0)--(11u,0);

label.llft("A", a);
label.lft("B", b);

t:= 8u;

%Column and track labels
for i=1 upto 11: z[i]=(i*u,t); endfor;
labels(1,2,3,4,5,6,7,8,9,10,11);
label(btex $t_1$ etex, (12u,6u));
label(btex $t_3$ etex, (12u,5u));
label(btex $t_2$ etex, (12u,4u));
label(btex $t_4$ etex, (12u,3u));
label(btex $t_5$ etex, (12u,2u));
label(btex $t_6$ etex, (12u,1u));

%text labels
label.lft("Column", (-1u,t));
label.lft("Net", (-1u,t)-(0,u));
label.rt("Track", (11u,7u));

%labels for crosstalk value
defaultscale:=1.3;
drawoptions(withcolor red);
draw (5u,6u)--(7u,6u); 
label.bot("2", (6u,6u));
label.top("0",(6.5u,3u));
label.lft("2",(10u,6u));
drawoptions(withcolor blue);
label.top("4",(6.5u,2u));
label.lft("3",(4u,1.5u));
label.rt("3",(4u,1.5u));

%net labels
drawoptions();
defaultscale:=1;
label.bot("5",(u,0));
label.bot("2",(3u,0));
label.bot("4",(4u,0));
label.bot("3",(5u,0));
label.bot("6",(8u,0));
label.bot("6",(10u,0));
label.top("1",(u,7u));
label.top("2",(3u,7u));
label.top("3",(7u,7u));
label.top("5",(8u,7u));
label.top("1",(9u,7u));
label.top("4",(10u,7u));

endfig;
end
