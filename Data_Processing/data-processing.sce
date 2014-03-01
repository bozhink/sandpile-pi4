//clc();

n = 41; // number of q-moments' values

fname = "t_moments";

ll = 4;
k = 10;

global q;
global LL;
global sigma;

function [z,tau]=Exponents(n,fname,low,high,k)
    global q;
    global LL;
    global sigma;
    a = read(fname, -1, n+1);
    m = size(a);
    m = m(1);
    q = a(1,2:n+1);

    LL = a(2:m,1);
    L = a(low:high,1);
    a = a(low:high, 2:n+1);
    m=size(a); m=m(1);

    loga = log(a);
    logL = log(L);

    sigma = zeros(1:n);
    sigb  = zeros(1:n);
    for i=1:n
        [sigma(i), sigb(i)] = linlsq(logL(1:m), loga(1:m,i));
    end

    [z, zb] = linlsq(q(k:n), sigma(k:n));
    tau = 1-zb/z;
endfunction


function [z,tau] = criticalExponents(m, n, k, q, logL, loga)
    sa = zeros(1:n);
    sb = zeros(1:n);
    for i=1:n
        [sa(i), sb(i)] = linlsq(logL(1:m), loga(1:m,i));
    end

    [z, zb] = linlsq(q(k:n), sa(k:n));
    tau = 1-zb/z;
endfunction





//ll=2:1:10; k=5:1:20;
//nll=max(size(ll)); nk=max(size(k));
//D=zeros(nll,nk); tauS=zeros(nll,nk); Z=zeros(nll,nk); tauT=zeros(nll,nk);
//for i=min(ll):max(ll)
//    for j=min(k):max(k)
//        [D(i,j),tauS(i,j)]=Exponents(n,"s-moments",i,j);
//        [Z(i,j),tauT(i,j)]=Exponents(n,"t-moments",i,j);
//    end;
//end;

//ts1=[0,LL(ll-1)';[q(k)',tauS(ll,k)']];
//td1=[0,LL(ll-1)';[q(k)',D(ll,k)']];
//tt1=[0,LL(ll-1)';[q(k)',tauT(ll,k)']];
//tz1=[0,LL(ll-1)';[q(k)',Z(ll,k)']];

n=41;
k=14;

//q12345=[9,17,25,33,41];

[D1,taus1]=Exponents(n, 'moments/z01/s', 2, 7, 14);
[z1,taut1]=Exponents(n, 'moments/z01/t', 2, 7, 16);
[a1,taua1]=Exponents(n, 'moments/z01/a', 2, 7, 16);
[x1,taux1]=Exponents(n, 'moments/z01/x', 2, 7, 35);

[D3,taus3]=Exponents(n, 'moments/z03/s', 2, 7, 14);
[z3,taut3]=Exponents(n, 'moments/z03/t', 2, 7, 16);
[a3,taua3]=Exponents(n, 'moments/z03/a', 2, 7, 16);
[x3,taux3]=Exponents(n, 'moments/z03/x', 2, 7, 35);

[D4,taus4]=Exponents(n, 'moments/z04/s', 2, 7, 14);
[z4,taut4]=Exponents(n, 'moments/z04/t', 2, 7, 16);
[a4,taua4]=Exponents(n, 'moments/z04/a', 2, 7, 16);
[x4,taux4]=Exponents(n, 'moments/z04/x', 2, 7, 35);

z01exp=[taus1,D1,taut1,z1,taua1,a1,taux1,x1];
z03exp=[taus3,D3,taut3,z3,taua3,a3,taux3,x3];
z04exp=[taus4,D4,taut4,z4,taua4,a4,taux4,x4];

//[D1,tauS1] = Exponents(n, 'moments/z01/s', 2, 8, k);
//sigma_s1=sigma;
//[z1,tauT1] = Exponents(n, 'moments/z01/t', 2, 8, k);
//sigma_t1=sigma;

//[D3,tauS3] = Exponents(n, 'moments/z03/s', 2, 13, k);
//sigma_s3=sigma;
//[z3,tauT3] = Exponents(n, 'moments/z03/t', 2, 13, k);
//sigma_t3=sigma;

//[D4,tauS4] = Exponents(n, 'moments/z04/s', 4, 13, k);
//sigma_s4=sigma;
//[z4,tauT4] = Exponents(n, 'moments/z04/t', 4, 13, k);
//sigma_t4=sigma;

//[D10,tauS10] = Exponents(n, 'moments/z10/s', 6, 13, k);
//sigma_s10=sigma;
//[z10,tauT10] = Exponents(n, 'moments/z10/t', 6, 13, k);
//sigma_t10=sigma;



//[iD1,itauS1] = Exponents(n, 'moments/z01/si', 2, 8, k/2);
//[iz1,itauT1] = Exponents(n, 'moments/z01/ti', 2, 8, k/2);
//
//[iD3,itauS3] = Exponents(n, 'moments/z03/si', 2, 13, k/2);
//[iz3,itauT3] = Exponents(n, 'moments/z03/ti', 2, 13, k/2);
//
//[iD4,itauS4] = Exponents(n, 'moments/z04/si', 2, 13, k/2);
//[iz4,itauT4] = Exponents(n, 'moments/z04/ti', 2, 13, k/2);
//
//[iD10,itauS10] = Exponents(n, 'moments/z10/si', 2, 13, k/2);
//[iz10,itauT10] = Exponents(n, 'moments/z10/ti', 2, 13, k/2);




//nn=9;
//D04=zeros(nn,1); taus04=D04; iD04=D04; itaus04=D04;
//D010=zeros(nn,1); taus010=D010; iD010=D010; itaus010=D010;
//D03=zeros(nn,1); taus03=D03; iD03=D03; itaus03=D03;
//for i=1:nn
//    [D04(i),  taus04(i)]  = Exponents(n, 'moments/z04/s.'+string(i), 2, 8, k);
//    [iD04(i), itaus04(i)] = Exponents(n, 'moments/z04/si.'+string(i), 2, 8, 1);
//    [D010(i),  taus010(i)]  = Exponents(n, 'moments/z10/s.'+string(i), 2, 8, k);
//    [iD010(i), itaus010(i)] = Exponents(n, 'moments/z10/si.'+string(i), 2, 8, 1);
//    [D03(i),  taus03(i)]  = Exponents(n, 'moments/z03/s.'+string(i), 2, 8, k);
//    [iD03(i), itaus03(i)] = Exponents(n, 'moments/z03/si.'+string(i), 2, 8, 1);
//end
//
