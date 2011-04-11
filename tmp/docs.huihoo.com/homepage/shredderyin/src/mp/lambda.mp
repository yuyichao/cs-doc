% File: lambda.mp
beginfig(1);
r = 2cm;

path c; c = fullcircle scaled 2r;

z.lambda.top = (0,(r-2mm)) rotated 15;
z.lambda.lft = (0,-(r-2mm)) rotated -40;
z.lambda.rt = (0,-(r-2mm)) rotated 40;
z.pi = (0,r+3mm);

path d; d = quartercircle scaled 2r rotated -90 & 
            halfcircle scaled 2r;

fill c withcolor red;

z.lambda.mid = .5[z.lambda.top,z.lambda.rt];

path lambda.top, lambda.rt;
lambda.top = z.lambda.mid .. (0,(r+1mm)) rotated 15;
lambda.rt = z.lambda.mid .. (0,-(r+1mm)) rotated 40;

path bluepart; bluepart = buildcycle(lambda.top, d, lambda.rt);

fill bluepart withcolor blue;

drawdblarrow (-r,-(r+6mm)) -- (r, -(r+6mm));
label.bot(btex $1$ etex, (0,-(r+7mm)));

drawarrow z.pi rotated 8 .. z.pi rotated 80 .. 
          z.pi rotated 178;
drawarrow z.pi rotated -8 .. z.pi rotated -80 .. 
          z.pi rotated -178;
label(btex $\pi$ etex, z.pi);

pickup pencircle scaled 4mm;
draw z.lambda.top .. z.lambda.rt withcolor white;
draw .4[z.lambda.top,z.lambda.rt] .. z.lambda.lft withcolor white;

endfig;
end.

