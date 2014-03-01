funcprot(0);

function [z]=qMoment(q,x,px)
    z=sum(px.*x.^q);
endfunction
d=[]; xd=[]; xc=[]; cc=[]; a=[]; c=[];

L = 6000;

a = read("sh/z10/" +string(L) + "n1",-1,4);
n=max(a(:,1));
c = zeros(n,1);
c(a(:,1)+1) = a(:,3);

endc=L;
cc=c(1:endc);
xc=(1:1:endc)';
nexp=800;
exponents=linspace(log10(endc), log10(n), nexp);
bounds=int(10**exponents);





q=5;
aq = qMoment(q, a(:,1), a(:,3));
scc=sum(cc);
cq = qMoment(q, xc, cc);
for i=1:nexp-1
    d(i) = sum(c(bounds(i)+1:bounds(i+1)));
end
scd = scc + sum(d); 

for iter = 1:10000
    for i=1:nexp-1
        r = rand();
        xd(i) = (bounds(i+1) + r*bounds(i))/(1+r);
    end
    dq = qMoment(q, xd, d);
    qerr(iter) = (cq+dq)/scd;
end
qq=mean(qerr);
sqq=stdev(qerr);

disp("L="+string(L)+", q="+string(q)+" bins="+string(nexp)+" err="+string(log(aq/qq))+" +/- "+string(sqq/qq*100)+" %.");





