//clc();

n = 41; // number of q-moments' values

fname = "t_moments";

ll = 4;
k = 10;

global q;
global LL;

function [z,tau]=pExponents(n, fname, ll, k, fig, Sigma, name, clr)
    global q;
    global LL;
    a = read(fname, -1, n+1);
    m = size(a);
    m = m(1);
    q = a(1,2:n+1);

    LL = a(2:m,1);
    L = a(ll:m,1);
    a = a(ll:m, 2:n+1);
    m=m-ll+1;

    loga = log(a);
    logL = log(L);
    
    scf(fig(1));
    a = get("current_axes");
    
    a.x_label.font_size=4;
    a.x_label.text="$q$";

    a.y_label.font_size=4;
    a.y_label.text="$\log(\langle "+name+"^q\rangle)$";

    a.title.foreground=9;
    a.title.font_size=4;
    a.title.text="$q\ \textrm{vs.}\ \log(\langle "+name+"^q\rangle)$";

    plot2d(q, loga');
    
    scf(fig(2));
    a = get("current_axes");
    
    a.x_label.font_size=4;
    a.x_label.text="$\log(L)$";

    a.y_label.font_size=4;
    a.y_label.text="$\log(\langle "+name+"^q\rangle)$";

    a.title.foreground=9;
    a.title.font_size=4;
    a.title.text="$\log(L)\ \textrm{vs.}\ \log(\langle "+name+"^q\rangle)$";
    
    plot2d(logL, loga);

    sigma = zeros(1:n);
    sigb  = zeros(1:n);
    for i=1:n
        [sigma(i), sigb(i)] = linlsq(logL(1:m), loga(1:m,i));
    end

    scf(fig(3));
    a = get("current_axes");
    
    a.x_label.font_size=4;
    a.x_label.text="$q$";

    a.y_label.font_size=4;
    a.y_label.text="$"+Sigma+name+"(q)$";

    a.title.foreground=9;
    a.title.font_size=4;
    a.title.text="$q\ \textrm{vs.}\ "+Sigma+name+"(q)$";
    
    //plot2d(q, [sigma', sigma'], [1,-1]);
    plot2d(q, [sigma'], [clr]);

    [z, zb] = linlsq(q(k:n), sigma(k:n));
    tau = 1-zb/z;
endfunction

scf(0);scf(1);scf(2);scf(3);scf(4);scf(5);
fig=2;
clf(0);clf(1);clf(fig);
pExponents(n,'moments/z01/t',2,10,[0,1,fig],'\sigma_','t',1);
pExponents(n,'moments/z03/t',2,10,[0,1,fig],'\sigma_','t',2);
pExponents(n,'moments/z04/t',2,10,[0,1,fig],'\sigma_','t',3);
pExponents(n,'moments/z10/t',2,10,[0,1,fig],'\sigma_','t',5);
//
hs = legend(['$Z=1$','$Z=3$','$Z=4$','$Z=10$'],2);  hs.font_size=3;  hs.visible='on';
tname = "graphics/sigma/sigma_t.eps";  unix('rm '+tname);  xs2eps(fig, tname);
//
//
fig=3;
clf(0);clf(1);clf(fig);
pExponents(n,'moments/z01/s',2,10,[0,1,fig],'\sigma_','s',1);
pExponents(n,'moments/z03/s',2,10,[0,1,fig],'\sigma_','s',2);
pExponents(n,'moments/z04/s',2,10,[0,1,fig],'\sigma_','s',3);
pExponents(n,'moments/z10/s',2,10,[0,1,fig],'\sigma_','s',5);
//
hs = legend(['$Z=1$','$Z=3$','$Z=4$','$Z=10$'],2);  hs.font_size=3;  hs.visible='on';
tname = "graphics/sigma/sigma_s.eps";  unix('rm '+tname);  xs2eps(fig, tname);
//
//
// Generalized moment analysis
//
clf(0);clf(1);clf(4);
pExponents(n,'moments/z01/ti',2,10,[0,1,4],'\Sigma_','t',1);
pExponents(n,'moments/z03/ti',2,10,[0,1,4],'\Sigma_','t',2);
pExponents(n,'moments/z04/ti',2,10,[0,1,4],'\Sigma_','t',3);
pExponents(n,'moments/z10/ti',2,10,[0,1,4],'\Sigma_','t',5);
//
hs = legend(['$Z=1$','$Z=3$','$Z=4$','$Z=10$'],2);  hs.font_size=3;  hs.visible='on';
tname = "graphics/sigma/isigma_t.eps";  unix('rm '+tname);  xs2eps(4, tname);
//
//
clf(0);clf(1);clf(5);
pExponents(n,'moments/z01/si',2,10,[0,1,5],'\Sigma_','s',1);
pExponents(n,'moments/z03/si',2,10,[0,1,5],'\Sigma_','s',2);
pExponents(n,'moments/z04/si',2,10,[0,1,5],'\Sigma_','s',3);
pExponents(n,'moments/z10/si',2,10,[0,1,5],'\Sigma_','s',5);
//
hs = legend(['$Z=1$','$Z=3$','$Z=4$','$Z=10$'],2);  hs.font_size=3;  hs.visible='on';
tname = "graphics/sigma/isigma_s.eps";  unix('rm '+tname);  xs2eps(5, tname);
//
