beginfig(1);
u:=.7cm;
n:=9;
m:=9;
obstaclewidth:=.5u;
pickup pencircle scaled 1pt;

for i=0 upto n: 
	draw (0*u,i*u)--(m*u,i*u); 
endfor;

for i=0 upto m: 
	draw (i*u,0*u)--(i*u,n*u); 
endfor;

extrasep:=-3pt;

for i=1 upto m-1: 
	label.bot(char (i+ ASCII "1"), (i*u, extrasep));
	label.lft(char (i+ ASCII "1"), (0, i*u));
endfor;

label.llft("1",(0,extrasep));
label.lft("10",(0,9u));
label.bot("10",(9u,extrasep));

for c=(2,5),(5,6),(9,9),(6,1):
	unfill fullcircle scaled .3u shifted ((xpart c-1)*u, (ypart c-1)*u);
	draw fullcircle scaled .3u shifted ((xpart c-1)*u, (ypart c-1)*u);
endfor;

for c=(2,1),(3,8),(5,4),(8,3),(7,7):
	fill fullcircle scaled .3u shifted ((xpart c-1)*u, (ypart c-1)*u);
endfor;

label.urt("a",(1u,4u));
label.lrt("b",(4u,5u));
label.urt("c",(8u,8u));
label.urt("d",(5u,0));

label.urt("1",(2u,7u));
label.urt("2",(6u,6u));
label.urt("3",(4u,3u));
label.urt("4",(1u,0u));
label.urt("5",(7u,2u));

picture saved[];

saved1:=currentpicture;
shipout currentpicture;
charcode:=2;
currentpicture:=saved1;

drawoptions(withpen pencircle scaled 2pt);

draw (2u,7u)--(2u,6u)--(6u,6u)--(6u,3u);
draw (u,0)--(4u,0)--(4u,3u)--(7u,3u)--(7u,2u);

saved2:=currentpicture;
shipout currentpicture;
charcode:=3;
currentpicture:=saved1;

draw (u,0)--(2u,0)--(2u,7u);
draw (2u,3u)--(7u,3u)--(7u,2u);
draw (6u,3u)--(6u,6u);

saved3:=currentpicture;
shipout currentpicture;
charcode:=4;
currentpicture:=saved3;

draw ((1,4)--(1,8)--(8,8)--(8,0)--(5,0)) scaled u;
draw ((4,5)--(4,8)) scaled u;

drawoptions();
for c=(2,5),(5,6),(9,9),(6,1):
	unfill fullcircle scaled .3u shifted ((xpart c-1)*u, (ypart c-1)*u);
	draw fullcircle scaled .3u shifted ((xpart c-1)*u, (ypart c-1)*u);
endfor;

saved4:=currentpicture;
shipout currentpicture;
charcode:=5;
currentpicture:=saved1;

drawoptions(withpen pencircle scaled 2pt);
draw ((6,6)--(0,6)--(0,0)--(1,0)) scaled u;
draw ((2,7)--(2,6)) scaled u;
draw ((0,3)--(7,3)--(7,2)) scaled u;

draw ((1,4)--(1,5)--(8,5)) scaled u;
draw ((8,8)--(8,0)--(5,0)) scaled u;

drawoptions();
for c=(2,5),(5,6),(9,9),(6,1):
	unfill fullcircle scaled .3u shifted ((xpart c-1)*u, (ypart c-1)*u);
	draw fullcircle scaled .3u shifted ((xpart c-1)*u, (ypart c-1)*u);
endfor;

endfig;
end

