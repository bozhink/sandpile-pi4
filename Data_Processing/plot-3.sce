
// Graphics of some histograms
// Nonlinear initial regions
// Z=1, 3, 4, 10 model

scf(0);clf();
a = get("current_axes");
    
a.x_label.font_size=4;
a.x_label.text="$\log_{10}t$";

a.y_label.font_size=4;
a.y_label.text="$\log_{10}P(t)$";

a.title.foreground=9;
a.title.font_size=4;
a.title.text="$\textrm{Time distribution probability}$";

DIR='th/z01/';
t = read(DIR+'1000',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[1]);

DIR='th/z03/';
t = read(DIR+'1000n1',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[3]);

DIR='th/z04/';
t = read(DIR+'1000n1',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[5]);

DIR='th/z10/';
t = read(DIR+'1000n1',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[6]);

a.data_bounds=[0,-4;2.5,0];


ht = legend(['$Z=1$','$Z=3$', '$Z=4$', '$Z=10$']);
ht.font_size=3;
ht.visible='on';

tname = "graphics/histograms/tt.eps";
unix('rm '+tname);
xs2eps(gcf(), tname);



scf(1);clf();
a = get("current_axes");
    
a.x_label.font_size=4;
a.x_label.text="$\log_{10}s$";

a.y_label.font_size=4;
a.y_label.text="$\log_{10}P(s)$";

a.title.foreground=9;
a.title.font_size=4;
a.title.text="$\textrm{Size distribution probability}$";

DIR='sh/z01/';
t = read(DIR+'1000',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[1]);

DIR='sh/z03/';
t = read(DIR+'1000n1',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[3]);

DIR='sh/z04/';
t = read(DIR+'1000n1',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[5]);

DIR='sh/z10/';
t = read(DIR+'1000n1',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[6]);

a.data_bounds=[0,-4;2.5,0];


hs = legend(['$Z=1$','$Z=3$', '$Z=4$', '$Z=10$']);
hs.font_size=3;
hs.visible='on';

sname = "graphics/histograms/ss.eps";
unix('rm '+sname);
xs2eps(gcf(), sname);
