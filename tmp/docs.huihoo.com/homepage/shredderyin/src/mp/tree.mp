input boxes;
vardef cuta(suffix a,b) expr p=
	drawarrow p cutbefore bpath.a cutafter bpath.b;
	point .5*length p of p
enddef;

beginfig(1);
u:=0.7cm;
circleit.cc(btex\strut c etex);
circleit.n1(btex\strut 1 etex);
circleit.n3(btex\strut 3 etex);
circleit.n4(btex\strut 4 etex);
circleit.n6(btex\strut 6 etex);
circleit.n7(btex\strut 7 etex);
circleit.n71(btex\strut 7 etex);
circleit.n8(btex\strut 8 etex);
circleit.n81(btex\strut 8 etex);
circleit.n82(btex\strut 8 etex);
cc.c-n1.c=whatever*(dir 30);
(xpart cc.c) - (xpart n1.c) = 3u;
n1.c-n7.c=(0,2u);
n7.c-n8.c=(0,2u);
cc.c-n3.c=whatever*(dir 150);
(xpart cc.c) - (xpart n3.c) = -2u;
n3.c-n4.c=whatever*(dir 50);
n3.c-n6.c=whatever*(dir 130);
(xpart n6.c) - (xpart n3.c) = (xpart n3.c) - (xpart n4.c) = 2u;
n4.c-n71.c=(0,2u);
n71.c-n81.c=(0,2u);
n6.c-n82.c=(0,2u);
n8.c-z0=(0,2u);
n81.c-z1=(0,2u);
n82.c-z2=(0,2u);
drawboxed(cc,n1, n3, n4, n6, n7, n71, n8, n81, n82);
label.ulft("4", cuta(cc,n1) cc.c--n1.c);
label.lft("4", cuta(n1,n7) n1.c--n7.c);
label.lft("4", cuta(n7,n8) n7.c--n8.c);
label.urt("4", cuta(cc,n3) cc.c--n3.c);
label.urt("4", cuta(n3,n6) n3.c--n6.c);
label.ulft("4", cuta(n3,n4) n3.c--n4.c);
label.lft("4", cuta(n4,n71) n4.c--n71.c);
label.lft("4", cuta(n71,n81) n71.c--n81.c);
label.lft("4", cuta(n6,n82) n6.c--n82.c);

drawarrow n8.s--z0;
drawarrow n81.s--z1;
drawarrow n82.s--z2;

label.lft("2",n1.w);
label.lft("3",n7.w);
label.lft("3",n8.w);
label.lft("2",n4.w);
label.lft("3",n71.w);
label.lft("3",n81.w);
label.rt("1",n3.e);
label.rt("2",n6.e);
label.rt("3",n82.e);

label.bot("24", z0);
label.bot("22", z2);
drawoptions(withcolor red);
label.bot("29", z1);

endfig;
end
