input boxes;

vardef cuta(suffix a,b)(expr r,c) expr p=
	draw p cutbefore bpath.a cutafter bpath.b withpen pencircle scaled (c*pt);
	point r*length p of p
enddef;

sp:=2cm;
beginfig(1);
circleit.n1("1");
circleit.n2("2");
circleit.n3("3");
circleit.n4("4");
circleit.n5("5");

n3.c=origin;
n4.c-n3.c=(sp,0);

n5.c-n4.c=(n4.c-n3.c) rotated 72;
n1.c-n5.c=(n5.c-n4.c) rotated 72;
n2.c-n1.c=(n1.c-n5.c) rotated 72;
%n3.c-n2.c=(n2.c-n1.c) rotated 72;

drawboxed(n1,n2,n3,n4,n5);

defaultscale:=0.8;
label.ulft("5",cuta(n1,n2)(0.5,1.2) n1.c--n2.c);
label.llft("5",cuta(n3,n2)(0.5,1.2) n3.c--n2.c);
label.bot("6",cuta(n3,n4)(0.5,1.2) n3.c--n4.c);
label.lrt("8",cuta(n5,n4)(0.5,.4) n5.c--n4.c);
label.urt("10",cuta(n5,n1)(0.5,.4) n5.c--n1.c);
label.ulft("6",cuta(n3,n1)(0.5,.4) n3.c--n1.c);
label.top("5",cuta(n2,n5)(0.8,.4) n2.c--n5.c);
label.rt("4",cuta(n3,n5)(0.8,1.2) n3.c--n5.c);
label.lft("8",cuta(n1,n4)(0.5,.4) n1.c--n4.c);
label.lft("11",cuta(n2,n4)(0.8,.4) n2.c--n4.c);


endfig;

beginfig(2);
circleit.a(btex \strut a etex );
circleit.b(btex \strut b etex );
circleit.c(btex \strut c etex );
circleit.d(btex \strut d etex );

b.c=origin;
c.c=b.c+(3cm,0);
d.c=c.c+(0,2cm);
a.c+(3cm,0)=d.c;

drawboxed(a,b,c,d);

label.lft("10",cuta(a,b)(0.5,1.2) a.c--b.c);
label.bot("7",cuta(c,b)(0.5,1.2) c.c--b.c);
label.rt("11",cuta(c,d)(0.5,1.2) c.c--d.c);
label.top("22",cuta(a,d)(0.5,.4) a.c--d.c);
label.top("11",cuta(a,c)(0.7,.4) a.c--c.c);
label.top("16",cuta(b,d)(0.7,.4) b.c--d.c);

endfig;

end
