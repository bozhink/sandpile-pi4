
// Graphics of some histograms

// Z=4 model

tname = "graphics/histograms/t-z4.png";
unix('rm '+tname);
scf(0);clf();
a = get("current_axes");
    
a.x_label.font_size=4;
a.x_label.text="$\log_{10}t$";

a.y_label.font_size=4;
a.y_label.text="$\log_{10}P(t)$";

a.title.foreground=9;
a.title.font_size=4;
a.title.text="$\textrm{Time distribution probability in Z=4 model}$";

DIR='th/z04/';

t = read(DIR+'6000n1',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[1]);

t = read(DIR+'3000n1',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[2]);

t = read(DIR+'1000n1',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[3]);

t = read(DIR+'256',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[4]);

t = read(DIR+'64',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[5]);

ht = legend(['$L=6000$','$L=3000$', '$L=1000$', '$L=256$', '$L=64$']);
ht.font_size=3; ht.visible='on';

xs2png(gcf(), tname);





sname = "graphics/histograms/s-z4.png";
unix('rm '+sname);

scf(1);clf();
a = get("current_axes");
    
a.x_label.font_size=4;
a.x_label.text="$\log_{10}s$";

a.y_label.font_size=4;
a.y_label.text="$\log_{10}P(s)$";

a.title.foreground=9;
a.title.font_size=4;
a.title.text="$\textrm{Size distribution probability in Z=4 model}$";

DIR='sh/z04/';

t = read(DIR+'6000n1',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[1]);

t = read(DIR+'3000n1',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[2]);

t = read(DIR+'1000n1',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[3]);

t = read(DIR+'256',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[4]);

t = read(DIR+'64',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[5]);

hs = legend(['$L=6000$','$L=3000$', '$L=1000$', '$L=256$', '$L=64$']);
hs.font_size=3; hs.visible='on';
xs2png(gcf(), sname);




