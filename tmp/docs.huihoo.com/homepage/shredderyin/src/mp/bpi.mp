beginfig(6);
xx0=3cm;
xx1=xx0+2mm;
xx2=xx1+.5cm;
xx3=xx2+1.3cm;
xx4=xx3+.5cm;
xx5=xx4+1cm;
xx6=xx5+.8cm;
xx7=xx6+.6cm;
xx8=xx7+.5cm;
xx9=xx8+.5cm;

yy0=0;
yy1=0;
yy2=yy1+.5cm;
yy3=yy2+.5cm;
yy4=yy3+1cm;
yy5=yy4+1cm;
yy6=yy5+1cm;
yy7=yy6+.6cm;
yy8=yy7+.5cm;
yy9=yy8+.5cm;

xxx0:=1cm;

pickup pencircle scaled 1pt;
draw (xxx0,0)--(xx9+1cm,0) dashed dashpattern (on 2pt off 2pt);
draw (xxx0,yy9)--(xx9+1cm,yy9) dashed dashpattern (on 2pt off 2pt);;

draw (xx1,yy9)--(xx1,yy6)--(xx0,yy6)--(xx0,yy1);
draw (xx1,yy9)--(xx1,yy8)--(xx5,yy8)--(xx5,yy9);
draw (xx7,yy9)--(xx7,yy7)--(xx4,yy7)--(xx4,yy1);
draw (xx2,yy1)--(xx2,yy2)--(xx4,yy2)--(xx4,yy1);
draw (xx3,yy1)--(xx3,yy3)--(xx8,yy3)--(xx8,yy9);
draw (xx7,yy1)--(xx7,yy4)--(xx9,yy4)--(xx9,yy1);

pickup pencircle scaled 2pt;
draw (xx2,yy9)--(xx2,yy5)--(xx6,yy5)--(xx6,yy1);

xxx1:=xx9+.5cm;
xxx2:=0;
pickup pencircle scaled .4pt;
drawoptions(dashed dashpattern(on 1pt off 1pt) withcolor red);
ahlength:=3pt;

draw (xx1,yy6)--(xxx1,yy6);
draw (xx9,yy4)--(xxx1,yy4);
draw (xx8,yy3)--(xxx1,yy3);
draw (xx7,yy7)--(xxx1,yy7);

draw (xx4,yy7)--(xxx2,yy7);
draw (xx3,yy3)--(xxx2,yy3);

drawdblarrow (xxx1,yy7)--(xxx1,yy6); 
drawdblarrow (xxx1,yy6)--(xxx1,yy4); 
drawdblarrow (xxx1,yy4)--(xxx1,yy3); 

drawdblarrow (xxx2,yy3)--(xxx2,yy7); 

drawoptions(withcolor blue);
label.lft("Perturbation Range" infont defaultfont rotated 90,
		.5[(xxx2,yy3),(xxx2,yy7)]);

label.rt("BPIs" infont defaultfont rotated -90, (xxx1,.5[yy6,yy4]));
label.top("T", (xx6,yy5)+(-.5cm,0));
endfig;
