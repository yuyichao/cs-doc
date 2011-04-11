input boxes;

beginfig(1);
z0=origin;
z1-z0=(2cm,0);
z2=2/3[z0,z1];
z3-z2=whatever * ((z1-z0) rotated 60);
y3-y2=2cm;
draw z0--z1;
draw z2--z3;
drawdot z2 withpen pencircle scaled 3pt;
dotlabel.bot(btex a etex, z0);
dotlabel.bot(btex b etex, z1);
endfig;

beginfig(2);
z0=origin;
z1-z0=(2cm,0);
x2=2/3[x0,x1];
y2=y0+1cm;
z3-z2=whatever * (z1-z0);
x3-x2=2cm;
z2-z4=z3-z2;

draw z0--z1;
draw z3--z4;

drawdot z2 withpen pencircle scaled 3pt;
dotlabel.bot(btex a etex, z0);
dotlabel.bot(btex b etex, z1);
endfig;

beginfig(3);
circleit.a(btex \strut a etex);
circleit.b(btex \strut b etex);
a.c=origin;
b.c=a.c+(3cm,-3.4cm);
drawboxed(a,b);
drawarrow a.c{right}..{down}b.c cutbefore bpath.a cutafter bpath.b;
endfig;

beginfig(4);
u := 1cm ;
z1 = (0,0) ;
z2 = (2u,4u) ;
z3 = (4u,0) ;
color yellow;
yellow:=(1,1,0);

def sample (expr p, c) = 
	draw p withpen pencircle scaled 2.5mm withcolor white ;
	draw p withpen pencircle scaled 2.0mm withcolor c ;
enddef ;

for i=.75 step .05 until 1 : 
	sample (z1 .. tension i .. z2 .. z3, .625red) ;
endfor ;

for i=1 step .05 until 2 : 
	sample (z1 .. tension i .. z2 .. z3, .625yellow) ;
endfor ;

sample (z1 .. z2 .. z3, .625white) ;
sample (z1 ... z2 ... z3, .625white) ;
endfig;

end
