input boxes;

vardef cuta(suffix a,b) expr p =
drawarrow p cutbefore bpath.a cutafter bpath.b;
point .5*length p of p
enddef;

%prologues:=1;
beginfig(1);

  height:=12cm;
  
  % user space
  
  boxjoin(a.se=b.sw; a.ne=b.nw);
  boxit.usercar(btex \strut car etex);
  boxit.userdefine(btex \strut define etex);
  boxit.userfoo2(btex \strut foo2 etex);
  boxit.userfoo3(btex \strut foo3 etex);  
  boxit.userbarcar(btex \strut bar/car etex);
  boxit.userbarhead(btex \strut bar/head etex);
  boxit.userbarminus(btex \strut bar/- etex);
  boxit.userbarstar(btex \strut bar/* etex);
  
  % scheme space
  boxjoin(a.se=b.sw; a.ne=b.nw);
  boxit.schemecar(btex \strut car etex);
  boxit.schemedefine(btex \strut define etex);
  boxit.schemeplus(btex \strut $+$ etex);  
  boxit.schemecons(btex \strut cons etex);
  boxit.schemestar(btex \strut * etex);
  
  % foo space lower part
  boxjoin(a.se=b.sw; a.ne=b.nw);
  boxit.foofoo1(btex \strut foo1 etex);
  boxit.foofoo2(btex \strut foo2 etex);
%  boxit.foofoo3(btex \strut foo3 etex);
  boxit.fooblank(btex \strut \hskip 2em etex);
  
  % foo space upper part
  boxjoin(a.se=b.sw; a.ne=b.nw);
  boxit.foodefine(btex \strut define etex);
  boxit.foocons(btex \strut cons etex);
  boxit.foostar(btex \strut $*$ etex);

  % join two foo parts
  foodefine.nw = foofoo1.sw;
  fooblank.se = foostar.ne;

  
  % bar space
  boxjoin(a.se=b.sw; a.ne=b.nw);
  boxit.barfoo2(btex \strut foo2 etex);
  boxit.barfoo3(btex \strut foo3 etex);  
  boxit.barcar(btex \strut bar/car etex);
  boxit.barhead(btex \strut bar/head etex);
  boxit.bardefine(btex \strut bar/define etex);
  boxit.barminus(btex \strut bar/- etex);
  boxit.barstar(btex \strut bar/* etex);

  boxjoin(a.se=b.sw; a.ne=b.nw);
  boxit.barfoo2up(btex \strut  etex);  
  boxit.barfoo3up(btex \strut  etex);
  boxit.barcarup(btex \strut  etex);
  boxit.barheadup(btex \strut  etex);  
  boxit.bardefineup(btex \strut X etex);  
  boxit.barminusup(btex \strut  etex);
  boxit.barstarup(btex \strut  etex);  
  
  % join two foo parts
  barfoo2.nw = barfoo2up.sw;
  barfoo3.nw = barfoo3up.sw; 
  barcarup.sw = barcar.nw;
  bardefineup.sw = bardefine.nw;
  barheadup.sw = barhead.nw;
  barminusup.sw = barminus.nw;
  barstarup.sw = barstar.nw;
  barstarup.se = barstar.ne;
  
  % locate user and scheme
%  xpart usercar.w = xpart schemecar.w;
  ypart usercar.s = ypart schemecar.n+height;
  xpart usercar.w = 0;

  % locate foo
  xpart barfoo2.w - 1cm = xpart schemecar.w;
  ypart foofoo1.w = ypart schemedefine.n + .5height;

  % locate bar
  xpart barfoo2.w = xpart foofoo2.w + 1in;
  ypart barfoo2.w = ypart foofoo1.s + .16height;
  
  
  drawboxed(usercar,userdefine,userfoo2,userfoo3,userbarcar,userbarhead,userbarminus,userbarstar);
  drawboxed(schemecar,schemedefine,schemeplus,schemecons,schemestar);
  drawboxed(foofoo1,foofoo2,fooblank);
  drawboxed(foodefine,foocons,foostar);
  drawboxed(barfoo2,barfoo3,barcar,bardefine,barhead,barminus,barstar);
  drawboxed(barfoo2up,barfoo3up,barcarup,bardefineup,barheadup,barminusup,barstarup);
  
%  drawarrow schemecar.n{up}..usercar.s{up};

  z0 = foocons.s + (0,-1cm);
  z1 = barhead.sw+(0,-1.5cm);
  
  draw schemecar.n{up}..z1{up};
  drawdot z1 withpen pencircle scaled 4pt;
  label.rt("alias", z1);
  
  drawarrow z1{up}..barcar.s{up};
  drawarrow z1{up}..barhead.s{up};

  z2 = barcar.s+(0,-.5cm);
  z3 = barstar.se+(12pt,-.5cm);
  
  draw z2--z3 dashed evenly;
  label.rt("prefix bar/", z3);
  
  drawarrow barfoo2up.n{up}..userfoo2.s{up};
  drawarrow barcarup.n{up}..userbarcar.s{up};

  drawarrow barheadup.n{up}..userbarhead.s{up};
  drawarrow barminusup.n{up}..userbarminus.s{up};
  drawarrow barstarup.n{up}..userbarstar.s{up};

  drawarrow foofoo2.n{up}..barfoo2.s{up};

%  drawarrow schemedefine.n{up}..userdefine.s{up};
  drawarrow schemeplus.n{up}..barminus.s{up};
  drawarrow schemecons.n{up}..foocons.s{up};
  drawarrow schemestar.n{up}..foostar.s{up};
  drawarrow schemedefine.n{up}..foodefine.s{up};
  drawarrow schemedefine.n{up}..bardefine.s{up};
  drawarrow schemestar.n{up}..barstar.s{up};

  drawarrow barfoo3up.n{up}..userfoo3.s{up} dashed evenly;

  label(btex \strut foo3 etex, fooblank.c) withcolor .7red;

  label.lft("user", usercar.w);
  label.lft("foo", foodefine.w);
  label.lft("bar", barfoo2.w);
  label.lft("scheme", schemecar.w);
  
endfig;

beginfig(2);

  height:=12cm;
  
  % user space
  
  boxjoin(a.se=b.sw; a.ne=b.nw);
  boxit.usercar(btex \strut car etex);
  boxit.userdefine(btex \strut define etex);
  boxit.userfoo2(btex \strut foo2 etex);
  boxit.userfoo3(btex \strut foo3 etex);  
  boxit.userbarcar(btex \strut bar/car etex);
  boxit.userbarhead(btex \strut bar/head etex);
  boxit.userbarminus(btex \strut bar/- etex);
  boxit.userbarstar(btex \strut bar/* etex);
  
  % scheme space
  boxjoin(a.se=b.sw; a.ne=b.nw);
  boxit.schemecar(btex \strut car etex);
  boxit.schemedefine(btex \strut define etex);
  boxit.schemeplus(btex \strut $+$ etex);  
  boxit.schemecons(btex \strut cons etex);
  boxit.schemestar(btex \strut * etex);
  
  % foo space lower part
  boxjoin(a.se=b.sw; a.ne=b.nw);
  boxit.foofoo1(btex \strut foo1 etex);
  boxit.foofoo2(btex \strut foo2 etex);
%  boxit.foofoo3(btex \strut foo3 etex);
  boxit.fooblank(btex \strut \hskip 2em etex);
  
  % foo space upper part
  boxjoin(a.se=b.sw; a.ne=b.nw);
  boxit.foodefine(btex \strut define etex);
  boxit.foocons(btex \strut cons etex);
  boxit.foostar(btex \strut $*$ etex);

  % join two foo parts
  foodefine.nw = foofoo1.sw;
  fooblank.se = foostar.ne;

  
  % bar space
  boxjoin(a.se=b.sw; a.ne=b.nw);
  boxit.barfoo2(btex \strut foo2 etex);
  boxit.barfoo3(btex \strut foo3 etex);  
  boxit.barcar(btex \strut bar/car etex);
  boxit.barhead(btex \strut bar/head etex);
  boxit.bardefine(btex \strut bar/define etex);
  boxit.barminus(btex \strut bar/- etex);
  boxit.barstar(btex \strut bar/* etex);

  boxjoin(a.se=b.sw; a.ne=b.nw);
  boxit.barfoo2up(btex \strut  etex);  
  boxit.barfoo3up(btex \strut  etex);
  boxit.barcarup(btex \strut  etex);
  boxit.barheadup(btex \strut  etex);  
  boxit.bardefineup(btex \strut X etex);  
  boxit.barminusup(btex \strut  etex);
  boxit.barstarup(btex \strut  etex);  
  
  % join two foo parts
  barfoo2.nw = barfoo2up.sw;
  barfoo3.nw = barfoo3up.sw; 
  barcarup.sw = barcar.nw;
  bardefineup.sw = bardefine.nw;
  barheadup.sw = barhead.nw;
  barminusup.sw = barminus.nw;
  barstarup.sw = barstar.nw;
  barstarup.se = barstar.ne;
  
  % locate user and scheme
%  xpart usercar.w = xpart schemecar.w;
  ypart usercar.s = ypart schemecar.n+height;
  xpart usercar.w = 0;

  % locate foo
  xpart barfoo2.w - 1cm = xpart schemecar.w;
  ypart foofoo1.w = ypart schemedefine.n + .5height;

  % locate bar
  xpart barfoo2.w = xpart foofoo2.w + 1in;
  ypart barfoo2.w = ypart foofoo1.s + .3height;
  
  
  drawboxed(usercar,userdefine,userfoo2,userfoo3,userbarcar,userbarhead,userbarminus,userbarstar);
  drawboxed(schemecar,schemedefine,schemeplus,schemecons,schemestar);
  drawboxed(foofoo1,foofoo2,fooblank);
  drawboxed(foodefine,foocons,foostar);
  drawboxed(barfoo2,barfoo3,barcar,bardefine,barhead,barminus,barstar);
  drawboxed(barfoo2up,barfoo3up,barcarup,bardefineup,barheadup,barminusup,barstarup);
  
%  drawarrow schemecar.n{up}..usercar.s{up};

  z0 = foocons.s + (0,-1cm);
  z1 = barhead.sw+(0,-1.5cm);
  
  draw schemecar.n{up}..z1{up};
  drawdot z1 withpen pencircle scaled 4pt;
  label.rt("alias", z1);
  
  drawarrow z1{up}..barcar.s{up};
  drawarrow z1{up}..barhead.s{up};

  z2 = barcar.s+(0,-.5cm);
  z3 = barstar.se+(12pt,-.5cm);
  
  draw z2--z3 dashed evenly;
  label.rt("prefix bar/", z3);
  
  drawarrow barfoo2up.n{up}..userfoo2.s{up};
  drawarrow barcarup.n{up}..userbarcar.s{up};

  drawarrow barheadup.n{up}..userbarhead.s{up};
  drawarrow barminusup.n{up}..userbarminus.s{up};
  drawarrow barstarup.n{up}..userbarstar.s{up};

  drawarrow foofoo2.n{up}..barfoo2.s{up};

%  drawarrow schemedefine.n{up}..userdefine.s{up};
  drawarrow schemeplus.n{up}..barminus.s{up};
  drawarrow schemecons.n{up}..foocons.s{up};
  drawarrow schemestar.n{up}..foostar.s{up};
  drawarrow schemedefine.n{up}..foodefine.s{up};
  drawarrow schemedefine.n{up}..bardefine.s{up};
  drawarrow schemestar.n{up}..barstar.s{up};
  drawarrow fooblank.n{up}..barfoo3.s{up};
  
  drawarrow barfoo3up.n{up}..userfoo3.s{up};

  label(btex \strut foo3 etex, fooblank.c);

  label.lft("user", usercar.w);
  label.lft("foo", foodefine.w);
  label.lft("bar", barfoo2.w);
  label.lft("scheme", schemecar.w);
  
endfig;


end;
