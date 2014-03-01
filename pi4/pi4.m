global Z;
global s;
global t;
global L;
global L1;
global T;

L=6000;
L1=L+1;
Z=fix(2*rand(L1,L1));
t=0;
s=0;

function update()
   global L;
   global L1;
   global T;
   global Z;
   global t;
   global s;
   s=0;
   t=1;
   for y=1:L
      y1=y+1;
      for x=1:y
         while (Z(x,y)>=2) % There is toppling
            s++;
            t=y1;
            Z(x,y) -= 2;
            Z(x,y1)++;
            Z(x+1,y1)++;
         endwhile
      endfor
   endfor
   T=t;
endfunction

function nextStep()
   global Z;
   global L;
   global L1;
   global T;
   for y=1:T
       Z(1:y,y) = fix(2*rand(y,1));
   endfor
   %Z(:,1:T) = fix(2*rand(L1,T));
   Z(1,1)=2;
   update();
endfunction;

nstat=1000;
for j=1:nstat
    fname=sprintf('data-%6.6d', j);
    for i=1:nstat
        nextStep();
        fp = fopen(fname, 'a');
        if (!fp)
            printf('Error opening data file\n');
            exit(1);
        endif;
        fprintf(fp, '%d %d\n', t, s);
        fclose(fp);
    endfor
endfor



