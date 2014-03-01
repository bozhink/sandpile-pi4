dir='~/Documents/Diplom/Master/Sandpile/pi4/histograms/';

a=load(strcat(dir,'th/z03/10000'));
n=max(size(a));

x=a(2:n,1);
y=a(2:n,3);

plot(log10(x), log10(y));
xlabel("log_{10}t");