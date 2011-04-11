beginfig(16);
u:=1.4cm;
pickup pencircle scaled 1pt;
numeric l,r,t,b;

z0=(.3u,5u);
t:=y0;
l:=x0-.5u;
z1=z0+(0,-.3u);
z14-z1=whatever*(dir -45);
z2=z1+(0,-u);
z3-z2=whatever*(dir -45);
x3-x2=1u;
x4=x3;
y4=y3-.4u;
z5-z4=whatever*(dir -45);
x5=x4+.4u;
x14=x5;
x6=x5+.4u;
y6=y5;
z7-z6=whatever*(dir -45);
x7=x6+.5u;
x8=x7+.4u;
y8=y7;
z9-z8=whatever*(dir -45);
x9-x8=.4u;
x10-x9=1.5u;
y10=y9;
b:=y10-.3u;
r:=x10+.5u;
x20=x10;
y20=b;
z15-z10=whatever*(dir -45);
x15+.5u=x10;
x12+.8u=x15;
y12=y15;
z13-z12=whatever*(dir -45);
y13=y6;


draw z0--z1--z2--z3--z4--z5--z6--z7--z8--z9--z10--z15--z12--z13--z6--z5--z14--z1;
draw z10--z20;
draw (l,t)--(r,t);
draw (l,b)--(r,b);

x21=x3;
y21=y14;
x22=x13;
y22=y15;
x23=x13+.3u;
y23=y14+.4u;

drawarrow z23--z21;
label.lrt(btex $S_i$ etex, z21+(0,-3pt));
drawarrow z23--z22;
label.rt("MC regions", z23);

z30=whatever[z1,z14];
x30=.3[x1,x14];
z31=whatever[z2,z3];
x31=x30;

draw z30--z31 dashed evenly;

z32=whatever[z1,z14];
x32=.6[x1,x14];
z33=whatever[z2,z3];
x33=x32;

draw z32--z33 dashed evenly;

z34=whatever[z1,z14];
x34=.1[x1,x14];
z35=whatever[z2,z3];
x35=x34;

draw z34--z35 dashed evenly;

pickup pencircle scaled .5pt;
drawoptions(withcolor blue);
drawdblarrow .6[z34,z35]--.6[z30,z31];
drawdblarrow .4[z30,z31]--.4[z32,z33];

endfig;
