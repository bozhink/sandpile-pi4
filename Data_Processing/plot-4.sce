
// Cumulative histograms for
// Z=3, 4

scf(0);clf();
a = get("current_axes");
    
a.x_label.font_size=4;
a.x_label.text="$\log_{10}x$";

a.y_label.font_size=4;
a.y_label.text="$\log_{10}p(x)$";

a.title.foreground=9;
a.title.font_size=4;
a.title.text="$\textrm{Cumulative distributions}$";

t = read('th/z04/6000n1',-1,4);
nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,4)),[1]);

s = read('sh/z04/6000n1',-1,4);
ns=max(size(s));
plot2d(log10(s(2:ns,1)), log10(s(2:ns,4)),[2]);

t = read('th/z03/6000n1',-1,4);
nt=max(size(t));
plot2d(log10(t(2:nt,1)), log10(t(2:nt,4)),[3]);

s = read('sh/z03/6000n1',-1,4);
ns=max(size(s));
plot2d(log10(s(2:ns,1)), log10(s(2:ns,4)),[4]);


//a.data_bounds=[0,-4;2.5,0];


ht = legend(['$Z=4,x=t$','$Z=4,x=s$', '$Z=3,x=t$', '$Z=3,x=s$']);
ht.font_size=3;
ht.visible='on';

tname = "graphics/histograms/cumulat.png";
unix('rm '+tname);
xs2png(gcf(), tname);


