beginfig(1);
gndw := 12pt;
gndh := 10pt;
srcw := 16pt;
vu := 5pt;
wavelen := .5cm;
sepp:=1.5cm;

def gnd expr z = 
	begingroup;
	picture pic;
	pic := currentpicture;
	pickup pencircle scaled 1pt;
	path p;
	pair zz;
	p = (z-(.5gndw,0))--(z+(.5gndw,0))--
		(z+(.5gndw,-.5gndh))--(z+(-.5gndw,-.5gndh))--cycle;
	for i=0 upto 9: 
		zz:=(z-(.5gndw,0)+(4i*pt,0));
		draw zz--(zz-(.5gndw,.5gndh));
	endfor;

	clip currentpicture to p;

	addto currentpicture also pic;

	draw (z-(.5gndw,0))--(z+(.5gndw,0));
	endgroup;
enddef;

def source (expr z) (suffix sss) =
	begingroup;
	save pp;
	pair pp[];
	pp1:=z-(0,vu);
	draw z--pp1;
	pickup pensquare scaled 1pt;
	draw (pp1-(.5srcw,0))--(pp1+(.5srcw,0));
	pp2:=pp1-(0,vu);
	pickup pensquare scaled 3pt;
	draw (pp2-(.3srcw,0))--(pp2+(.3srcw,0));
	pickup pensquare scaled 1pt;
	pp3:=pp2-(0,vu);
	pickup pencircle scaled 1pt;
	draw pp2--pp3;
	pair sss.bot;
	sss.bot=pp3;
	endgroup;
enddef;

def cap (expr z) (suffix sss) =
	begingroup;
	save pp,vu;
	vu:=7pt;
	pickup pencircle scaled 1pt;
	pair pp[];
	pp1:=z-(0,vu);
	draw z--pp1;
	draw (pp1-(.5srcw,0))--(pp1+(.5srcw,0));
	pp2:=pp1-(0,.6vu);
	draw (pp2-(.5srcw,0))--(pp2+(.5srcw,0));
	pp3:=pp2-(0,vu);
	draw pp2--pp3;
	pair sss.bot, sss.mid, sss.top;
	sss.top=z;
	sss.bot=pp3;
	sss.mid=.5[pp3,z];
	endgroup;
enddef;

def line (expr z, len) (suffix sss) =
	begingroup;
	save pp;
	vu:=3pt;
	pair pp[];
	pickup pencircle scaled 1pt;
	pp3:=z;
	for i=0 upto len:
		pp1:=pp3+(vu,vu);
		draw pp3--pp1;
		pp2:=pp1+(2vu,-2vu);
		draw pp1--pp2;
		pp3:=pp2+(vu,vu);
		draw pp2--pp3;
	endfor;
	pair sss.right, sss.mid, sss.left;
	sss.right=pp3;
	sss.mid=.5[z,pp3];
	sss.left=z;
	endgroup;
enddef;

def wave (expr p)(suffix sss) =
	pickup pencircle scaled 1pt;
	pair ppp[];
	ppp0=p;
	ppp1=ppp0+(wavelen,0);
	ppp2=ppp1+(0,wavelen);
	ppp3=ppp2+(wavelen,0);
	draw ppp0--ppp1--ppp2--ppp3;
	pair sss.a, sss.b, sss.c;
	sss.a=ppp0; sss.b=ppp1; sss.c=ppp3;
enddef;
		
pair pp[];
wave((40,0), wa);
label.lft(btex 0 etex, wa.a);
label.bot(btex $t=0$ etex, wa.b);
label.rt(btex $E_1$ etex, wa.c);

pp1:=wa.c+(sepp,10pt);

source(pp1, sou);
gnd(sou.bot);
pp2=pp1+(.5cm,0);
draw pp1--pp2;
line(pp2, 2, l);
label.top(btex $R_{11}$ etex, l.mid+(0,7pt));
drawdot(l.right) withpen pencircle scaled 3pt;
pp3=l.right+(.5cm,0);
draw l.right--pp3;
line(pp3, 5, ll);

sep:=6pt;
draw (ll.left+(0,-sep))--(ll.right+(0,-sep));

pp4=ll.mid+(0,-sep);
pp5=(pp4-(0,7pt));
draw pp4--pp5;
gnd(pp5);
pp6=ll.right+(1cm,0);
draw ll.right--pp6;
dotlabel.top(btex $V_1(l,t)$ etex, pp6);
cap(pp6, c);
label.rt(btex $C_{t2}$ etex, c.mid+(.5srcw,0));
gnd(c.bot);

%second line
gndw := 12pt;
gndh := 10pt;
srcw := 16pt;
vu := 5pt;
pair pp[];
wave((0, 70), waa);
label.lft(btex 0 etex, waa.a);
label.bot(btex $t=0$ etex, waa.b);
label.rt(btex $E_2$ etex, waa.c);

pp1:=waa.c+(sepp,10pt);

source(pp1, souu);
gnd(souu.bot);
pp2=pp1+(.5cm,0);
draw pp1--pp2;
line(pp2, 2, d);
label.top(btex $R_{12}$ etex, d.mid+(0,7pt));
drawdot(d.right) withpen pencircle scaled 3pt;
pp3=d.right+(.5cm,0);
draw d.right--pp3;
line(pp3, 5, dd);

sep:=6pt;
draw (dd.left+(0,-sep))--(dd.right+(0,-sep));

pp4=dd.mid+(0,-sep);
pp5=(pp4-(0,7pt));
draw pp4--pp5;
gnd(pp5);
pp6=dd.right+(1cm,0);
draw dd.right--pp6;
dotlabel.top(btex $V_2(l,t)$ etex, pp6);
cap(pp6, c);
label.rt(btex $C_{t1}$ etex, c.mid+(.5srcw,0));
gnd(c.bot);

pp7=.5[ll.mid, dd.mid];
pp8=pp7+(0,vu);
pp9=pp7-(0,vu);
cap(pp8, ccc);
draw dd.mid--ccc.top dashed evenly;
draw ll.mid--ccc.bot dashed evenly;
label.rt(btex $C_c$ etex, ccc.mid+(.5srcw,0));
label.top(btex $R$ etex, ll.left+(0, 3pt));
label.top(btex $R$ etex, dd.left+(0, 3pt));

endfig;
end
