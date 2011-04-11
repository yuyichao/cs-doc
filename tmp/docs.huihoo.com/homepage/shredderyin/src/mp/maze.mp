vardef massivelabel(expr l)(text coords)=
	defaultscale:=0.8;
	for c=coords: 
		unfill unitsquare scaled (defaultscale*obstaclewidth) 
				  shifted (u*c+(-.5*defaultscale*obstaclewidth,-.5*defaultscale*obstaclewidth));
		label(l,u*c); 
	endfor;
enddef;

beginfig(1);
u:=.7cm;
n:=9;
m:=9;
obstaclewidth:=.5u;
pickup pencircle scaled 1pt;
for i=0 upto n: draw (0*u,i*u)--(m*u,i*u); endfor;
for i=0 upto m: draw (i*u,0*u)--(i*u,n*u); endfor;
pickup pensquare scaled obstaclewidth;
drawoptions(withcolor .7white);
draw (u,3u)--(u,7u);
draw (3u,2u)--(3u,2u);
draw (5u,u)--(5u,u);
draw (5u,3u)--(5u,6u);
draw (7u,5u)--(7u,7u)--(6u,7u);
drawoptions(withcolor black);
fill fullcircle scaled .8obstaclewidth shifted (u,u);
fill fullcircle scaled .8obstaclewidth shifted (6u,6u);
shipout currentpicture;

charcode:=2;
pickup pencircle scaled 2pt;
draw (u,u)--(4u,u)--(4u,2u)--(6u,2u)--(6u,6u);

massivelabel("2", (0,3),(0,4),(0,7),(0,8),(2,3),(2,4),(2,7),(2,8),(3,0),(3,3),(3,7),(3,7),(4,0),(4,1),(4,2),(4,5),(4,6),(6,1),(6,3),(6,4),(7,0),(7,1),(7,2),(7,3),(8,0),(8,2));
massivelabel("1", (0,0),(0,1),(0,2),(0,5),(0,6),(0,9),(1,0),(1,2),(1,8),(1,9),(2,0),(2,1),(2,2),(2,5),(2,6),(2,9),(3,1),(3,4),(3,5),(3,8),(3,9),(4,3),(4,4),(4,7),(4,8),(5,0),(5,2),(5,7),(6,0),(6,2),(6,5),(7,4),(8,1),(8,3),(8,4),(9,0),(9,1),(9,2),(9,3),(9,4),(9,5));

endfig;

end
