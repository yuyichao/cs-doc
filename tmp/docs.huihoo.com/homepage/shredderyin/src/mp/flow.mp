beginfig(7);
vardef drawdots(text t)=
	begingroup;
	pickup pencircle scaled 3pt;
	forsuffixes$=t:
		drawdot $;
	endfor;
	pickup pencircle scaled .4pt;
	endgroup;
enddef;

u:=1cm;
xx1:=0;
xx2:=xx1+2u;
xx3:=xx2+2u;
xx4:=xx3+2u;
xx5:=xx4+2.4u;

yym:=u;
yy5:=yym+2u;
yy4:=yy5+u;
yy3:=yy4+u;
yy2:=yy3+u;
yy1:=yy2+u;

pair ss, s[],sm;
ss=(0, .5[yym,yy1]);
drawoptions (withpen pencircle scaled 3pt);
drawdot ss;
s1=(xx2, yy1);
drawdot s1;
s2=(xx2, yy2);
drawdot s2;
s3=(xx2, yy3);
drawdot s3;
s4=(xx2, yy4);
drawdot s4;
s5=(xx2, yy5);
drawdot s5;
sm=(xx2, yym);
drawdot sm;

drawoptions ();
drawarrow ss--s1;
drawarrow ss--s2;
drawarrow ss--s3;
drawarrow ss--s4;
drawarrow ss--s5;
drawarrow ss--sm;

pair r[], rk;

rk=(xx3,yym-.4u);
r3=(xx3,yy4-.4u);
r2=(xx3,yy2-.4u);
r1=(xx3,yy1+.4u);

drawdots(r1,r2,r3,rk);

drawarrow s1--r1;
drawarrow s2--r1;
drawarrow s3--r2;
drawarrow s4--r2;

pair d[], di, dn;

d1=(xx4,yy1);
d2=(xx4,yy2);
di=(xx4,yy4+.2u);
dn=(xx4,yym);

drawdots(d1,d2,di,dn);
drawarrow r1--di;
drawarrow r2--di;

pair t;
t=(xx5,.5[yy1,yym]);

drawarrow d1--t;
drawarrow d2--t;
drawarrow di--t;
drawarrow dn--t;

label.lft ("s",ss);
label.top ("s1",s1);
label.top ("s2",s2);
label.top ("s3",s3);
label.top ("s4",s4);
label.top ("s5",s5);
label.bot ("sm",sm);
label.top ("r1",r1);
label.top ("r2",r2);
label.top ("r3",r3);
label.bot ("rk",rk);
label.top ("d1",d1);
label.top ("d2",d2);
label.top ("di",di);
label.bot (btex $d_{n-1}$ etex ,dn);
label.rt("t",t);

label.ulft ("14", .5[ss,s1]);
label.ulft ("12", .5[ss,s2]);
label.ulft ("15", .5[ss,s3]);
label.top ("6", .5[ss,s4]);
label.top ("7", .5[s1,r1]);
label.top ("4", .5[s2,r1]);
label.top ("5", .5[s3,r2]);
label.top ("3", .5[s4,r2]);
label.top ("7", .5[r1,di]);
label.top ("5", .5[r2,di]);
endfig;
