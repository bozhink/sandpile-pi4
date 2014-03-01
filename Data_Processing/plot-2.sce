
// Graphics of DSM histograms

// Z=1 model

scf(0);clf();
a = get("current_axes");
    
a.x_label.font_size=4;
a.x_label.text="$\log_{10}t$";

a.y_label.font_size=4;
a.y_label.text="$\log_{10}P(t)$";

a.title.foreground=9;
a.title.font_size=4;
a.title.text="$\textrm{Time distribution probability in Z=1 model}$";

DIR='th/z01/';

t = read(DIR+'3000',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[2]);

t = read(DIR+'2000',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[3]);

t = read(DIR+'1000',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[4]);

ht = legend(['$L=3000$', '$L=2000$', '$L=1000$']);
ht.font_size=3; ht.visible='on';

tname = "graphics/histograms/t-z1.png";
unix('rm '+tname);
xs2png(gcf(), tname);



scf(1);clf();
a = get("current_axes");
    
a.x_label.font_size=4;
a.x_label.text="$\log_{10}s$";

a.y_label.font_size=4;
a.y_label.text="$\log_{10}P(s)$";

a.title.foreground=9;
a.title.font_size=4;
a.title.text="$\textrm{Size distribution probability in Z=1 model}$";

DIR='sh/z01/';

t = read(DIR+'3000',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[2]);

t = read(DIR+'2000',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[3]);

t = read(DIR+'1000',-1,3); nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,3)),[4]);

hs = legend(['$L=3000$','$L=2000$', '$L=1000$']);
hs.font_size=3; hs.visible='on';

sname = "graphics/histograms/s-z1.png";
unix('rm '+sname);
xs2png(gcf(), sname);
