
// Data collapse
// Z=1,3,4,10 models

taus=[1.347    1.442    1.437    1.436];
D   =[1.499    1.784    1.754    1.738];
taut=[1.510    1.793    1.745    1.710];
z   =[0.999    0.996    0.993    0.992];


function processT(DIR, suffix, L, Z, betax, taux, fig, figname)
    scf(fig);clf();
    a = get("current_axes");
    a.x_label.font_size=4;  a.x_label.text="$\log_{10}(t/L^{z})$";
    a.y_label.font_size=4;  a.y_label.text="$\log_{10}\left(p\;(t)\,L^{z(\tau_t-1)}\right)$";
    a.title.foreground=9;   a.title.font_size=4;   a.title.text="$\textrm{Data collapse for Z="+string(Z)+" model}$";
    nl=max(size(L)); leg=[];
    for i=1:nl
        fname = DIR + string(L(i)) + suffix;
        t = read(fname,-1,4); nt=max(size(t));
        x = t(:,1)/(L(i)**betax);
        y = t(:,4).*(L(i)**(betax*(taux-1)));
       plot2d((x(2:nt)), log10(y(2:nt)),[i]);
       leg(i) = '$L='+string(L(i))+'$';
    end
    ht = legend(leg,3); ht.font_size=3; ht.visible='on';
    unix('rm ' + figname);
    xs2png(fig, figname);
endfunction


function processS(DIR, suffix, L, Z, betax, taux, fig, figname)
    scf(fig);clf();
    a = get("current_axes");
    a.x_label.font_size=4;  a.x_label.text="$\log_{10}(s/L^{D})$";
    a.y_label.font_size=4;  a.y_label.text="$\log_{10}\left(p\;(s)\,L^{D(\tau_s-1)}\right)$";
    a.title.foreground=9;   a.title.font_size=4;   a.title.text="$\textrm{Data collapse for Z="+string(Z)+" model}$";
    nl=max(size(L)); leg=[];
    for i=1:nl
        fname = DIR + string(L(i)) + suffix;
        t = read(fname,-1,4); nt=max(size(t));
        x = t(:,1)/(L(i)**betax);
        y = t(:,4).*(L(i)**(betax*(taux-1)));
       plot2d((x(2:nt)), log10(y(2:nt)),[i]);
       leg(i) = '$L='+string(L(i))+'$';
    end
    ht = legend(leg,3); ht.font_size=3; ht.visible='on';
    unix('rm ' + figname);
    xs2png(fig, figname);
endfunction


// Z=4
Z=4;
L = [1000,2000,3000,6000];

DIR='th/z04/';
processT(DIR, "n1", L, Z, 1.00, 1.78, 0, 'graphics/data-collapse/L/t-z4-178.png');
processT(DIR, "n1", L, Z, 1.00, 1.75, 0, 'graphics/data-collapse/L/t-z4-175.png');

DIR='sh/z04/';
processS(DIR, "n1", L, Z, 1.78, 1.43, 1, 'graphics/data-collapse/L/s-z4-178.png');
processS(DIR, "n1", L, Z, 1.75, 1.43, 1, 'graphics/data-collapse/L/s-z4-175.png');


// Z=3
Z=3;
L = [1000,2000,3000,6000];

DIR='th/z03/';
processT(DIR, "n1", L, Z, 1.00, 1.75, 2, 'graphics/data-collapse/L/t-z3-175.png');
processT(DIR, "n1", L, Z, 1.00, 1.78, 2, 'graphics/data-collapse/L/t-z3-178.png');

DIR='sh/z03/';
processS(DIR, "n1", L, Z, 1.75, 1.43, 3, 'graphics/data-collapse/L/s-z3-175.png');
processS(DIR, "n1", L, Z, 1.78, 1.43, 3, 'graphics/data-collapse/L/s-z3-178.png');



// Z=10
Z=10;
L = [1000,1500,2000,2500,3000];

DIR='th/z10/';
processT(DIR, "n1", L, Z, 1, 1.75, 4, 'graphics/data-collapse/L/t-z10.png');

DIR='sh/z10/';
processS(DIR, "n1", L, Z, 1.75, 1.42, 5, 'graphics/data-collapse/L/s-z10.png');


// Z=1
Z=1;
L = [1000,1500,2000,2500,3000];

DIR='th/z01/';
processT(DIR, "", L, Z, 1, 1.50, 6, 'graphics/data-collapse/L/t-z1.png');

DIR='sh/z01/';
processS(DIR, "", L, Z, 1.50, 1.33, 7, 'graphics/data-collapse/L/s-z1.png');

