y = zeros(100,2);
y2 = zeros(100,2);
w0 = 2; zeta = 1;
r = -20;
B = [0 w0^2];
Wm = [0, 1, -w0^2, -2*zeta*w0];
for i = 1:500
    dt = 0.01;
    ym = y(i,1:2);
    ym2 = y2(i,1:2);
   for j = 1:2
       %wm1 = Wm(2*j - 1))*ym(1)     %j = 1: 1, j = 2: 3
       %wm2 = Wm(2*j)*ym(2)          %j = 1: 2, j = 2: 4
      dym(j) = Wm(2*j -1)*ym(1) + Wm(2*j)*ym(2) + B(j)*r
      
   end
   dym2 = [0,1;-w0^2 -2*zeta*w0]*ym2' + [0;w0^2]*r;
   
   y(i+1,1) = y(i,1) + dt*dym(1);
   y(i+1,2) = y(i,2) + dt*dym(2);
   
   y2(i+1,1:2) = y2(i,1:2) + dt*dym2(1:2)';
   
end