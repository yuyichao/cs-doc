u:=2cm;
beginfig(1);
numeric theta;
theta:=20;
z0=origin;
z2-z0=2u*left;

pair delta;
delta=unitvector (z2-z0 rotated theta) * 3;
z1-z0=((z2-z0) rotated -theta) + delta;
z3-z0=(z2-z0) rotated theta + 2delta;

draw z0--z1--z2--z0--z3--z2 withpen pencircle scaled .7pt;
label.rt("0",z0);
label.urt("1",z1);
label.lft("n-2",z2);
label.llft("n-1",z3);


currentpicture:=currentpicture shifted -z3;
endfig;

beginfig(2);
numeric theta;
theta:=45;
z0=origin;
z2-z0=2u*left;

pair delta;
delta=unitvector (z2-z0 rotated theta);
z1-z0=((z2-z0) rotated -theta) + 50delta;
z3-z0=((z1-z0) rotated 10) -5delta;
z4-z0=((z3-z0) rotated 10) -5delta;

z2'=whatever[z3,z4];
z2'=whatever[z0,z2];

z5=whatever[z3,z4];
z5=whatever[z2,z0];

pickup pencircle scaled .6pt;
draw z0--z1;
draw z3--z4;
draw z1--z3 dashed dashpattern (on 1bp off 1bp);
draw z0--z2;
drawarrow z0--z4;
label.top(btex $r$ etex, .5[z4,z0]);
draw z1--z2 dashed evenly;
draw z3--z2 dashed evenly;
draw z4--z2 dashed evenly;

path circ;
circ=halfcircle scaled 2length(z4-z0) rotated 90;
	
draw circ cutbefore (z0--z1) cutafter (z0--(z2'-(20pt,0)));
draw z2'--z2 dashed evenly;

draw z5--z4 dashed evenly;

z6-z0=(z2'-z0) rotated 10;
z7=(z5+z2+z4)/3;
z8=whatever[z0,z6];
z8=whatever[z7,z2];
z9=whatever[z2',z4];
z9=whatever[z7,z2];

draw z8--z9;
draw z0--z8;
fill z9--z2--z4--cycle withcolor .7white;
label.bot(btex $n-1$ etex,z8);


label.rt("0",z0);
label.urt("1",z1);
label.llft(btex $n-2$ etex,z2);
dotlabel.lft(btex $k-1$ etex,z3);
dotlabel.lft(btex $k$ etex,z4);
drawdot (z9+z2+z4)/3 withpen pencircle scaled 2pt;

endfig;

beginfig(3);
numeric theta;
theta:=10;
z0=origin;
z2-z0=2u*left;

pair delta;
delta=unitvector ((z2-z0) rotated theta) * 3;
z3-z0=((z2-z0) rotated -theta);
z1-z0=(z2-z0) rotated theta + 2delta;

for i:=3 upto 6:
	z[i]-z0=(z[i-1]-z0) rotated -theta;
	draw z0--z[i];
endfor;
draw z0--z2;
path arc;
arc = halfcircle scaled 2length(z2-z0) rotated 90 cutbefore (z0--(2[z0,z6])) cutafter (z0--z2);
draw arc;

label.rt("0", z0);
label.urt("x", z6);
label.lft(btex $1'$ etex, z5);
label.lft(btex $2'$ etex, z4);
label.lft(btex $n-2$ etex, z3);
label.lft(btex $n-1$ etex, z2);
label.top(btex \vdots etex, .8[z0,z3]);
endfig;

beginfig(4);
numeric theta;
theta:=20;
z0=origin;
z2-z0=2u*left;

for i:=3 upto 5:
	z[i]-z0=(z[i-1]-z0) rotated -theta;
	draw z0--z[i];
endfor;

draw z0--z2;
draw z2--z4;
draw z3--z5;
path arc;
arc = halfcircle scaled 2length(z2-z0) rotated 90 cutbefore (z0--z5) cutafter (z0--z2);
draw arc;

z6=(z0--z3) intersectionpoint (z2--z4);
z7=(z0--z4) intersectionpoint (z3--z5);
z8=.3[z3,z6]; z9=.7[z4,z7];

draw z2--z8--z9 dashed evenly withpen pencircle scaled .8pt;
draw z3--z6 withpen pencircle scaled 2pt;

label.urt(btex $r$ etex, .5[z0,z4]);
%dotlabels.bot(0,2,3,4,5,6,7);
endfig;

end
