input boxes;
beginfig(1);

boxjoin(a.sw=b.nw; a.se=b.ne);
boxit.state(btex state etex);
boxit.flag(btex flag etex );
boxit.sigpending(btex sigpending etex );
boxit.files(btex files etex );
boxit.dot1(btex \vdots etex);
boxit.pid(btex pid etex );
boxit.dot2(btex \vdots etex);


boxjoin(a.sw=b.nw; a.se=b.ne);
boxit.count(btex count etex );
boxit.max_fd(btex max\_fd etex );
boxit.fd(btex fd etex );
boxit.close_on_exec(btex close\_on\_exec etex );
boxit.open_fds(btex open\_fds etex );

boxjoin(a.sw=b.nw; a.se=b.ne);
boxit.c1();
boxit.c2();
boxit.c3();
boxit.c4();
boxit.c5();
boxit.c6();
boxit.dot3(btex \vdots etex);

boxjoin(a.sw=b.nw; a.se=b.ne);
boxit.dot4(btex \vdots etex);
boxit.fop(btex f\_op etex );
boxit.fpos(btex f\_pos etex );
boxit.dot5(btex \vdots etex);
boxit.privatedata(btex private\_data etex );

boxjoin(a.sw=b.nw; a.se=b.ne);
boxit.dot41(btex \vdots etex);
boxit.fop1(btex f\_op etex );
boxit.fpos1(btex f\_pos etex );
boxit.dot51(btex \vdots etex);
boxit.privatedata1(btex private\_data etex );

dot41.nw = privatedata.sw + (0, -.5u);

boxjoin(a.sw=b.nw; a.se=b.ne);
boxit.llseek(btex llseek etex );
boxit.read(btex read etex );
boxit.mywrite(btex write etex );
boxit.readdir(btex readdir etex );
boxit.dot6(btex \vdots etex);

boxjoin(a.sw=b.nw; a.se=b.ne);
boxit.llseek1(btex llseek etex );
boxit.read1(btex read etex );
boxit.mywrite1(btex write etex );
boxit.readdir1(btex readdir etex );
boxit.dot61(btex \vdots etex);

llseek1.nw = dot6.sw + (0, -u);

ypart count.c = ypart files.c + .6u;
ypart c1.c = ypart fd.c + .6u;
ypart dot4.c = ypart c1.c + .6u;
ypart llseek.c = ypart dot4.c + .6u;

xpart count.w - xpart files.e = xpart c1.w - xpart fd.e = xpart dot4.w - xpart c1.e = xpart llseek.w - xpart dot4.e = u;

w:=.6u;
state.dx=w;
count.dx=w;
c1.dx=8bp;
fpos.dx=w;
fpos1.dx=w;
llseek.dx=w;
llseek1.dx=w;


drawboxed(state, flag, sigpending, files, dot1, pid)
drawboxed(dot2, count,max_fd, fd,close_on_exec, open_fds)
drawboxed(dot4, fop, fpos, dot5, privatedata);
drawboxed(dot41, fop1, fpos1, dot51, privatedata1);
drawboxed(c1,c2,c3,c4,c5, c6,dot3);
drawboxed(llseek, read, mywrite, readdir, dot6);
drawboxed(llseek1, read1, mywrite1, readdir1, dot61);

drawarrow (files.e - (.2u,0)){right}...{right}count.nw;
drawarrow (fd.e- (.2u,0)){right}...{right}c1.nw;
drawarrow c1.c{right}...{right}dot4.nw;
drawarrow c2.c{right}...{right}dot41.nw;
drawarrow c6.c{right}...{right}(dot4.nw+(0, -4bp));
drawarrow (fop.e- (.2u,0)){right}...{right}llseek.nw;
drawarrow (fop1.e- (.2u,0)){right}...{right}llseek1.nw;

label.urt(btex task\_struct etex , state.nw+(0,4bp));
label.urt(btex files\_struct etex , count.nw+(0,4bp));
label.urt(btex file etex , dot4.nw+(0,4bp));
label.urt(btex file\_operations etex, llseek.nw+(0,4bp));

endfig;
