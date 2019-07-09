I = 5000;
w0 = 1; zeta = 1; lambda = 1; gamma = 1; beta = 1; g = 1;
dt = 0.01;
a0 = -1; a1 = -1; b0 = 1; 
Ap = [0 1 a0 a1]; Bp = [0 b0];
Am = [0 1 -w0^2 -2*zeta*w0^2]; Bm = [0 w0^2];
Yp = zeros(I,2);
Ym = zeros(I,2);
Z= zeros(I,2);
W2=zeros(I,1);
W1=zeros(I,1);
Phi1= zeros(I,2);
Phi2= zeros(I,2);
Phi3= zeros(I,2);
Q= zeros(I,4);
R= zeros(I,16);
Theta= zeros(I,4); Theta(1,1:4) = [1,1,1,1];
Up = zeros(I,1);
Ref = 10*sin(0:dt:I*dt);
figure(1)
clf(1)

for n = 1:I
    w1 = W1(n); w2 = W2(n); theta = Theta(n,1:4); phi1 = Phi1(n,1:2);
    phi2 = Phi2(n,1:2); phi3 = Phi3(n,1:2); yp = Yp(n,1:2); ym = Ym(n,1:2);
    r = R(n,1:16); q= Q(n,1:4); z= Z(n,1:2);
    
    ref =Ref(n);
    up = 0;
    w = [w1,w2,yp(1),ref];
    phiP = [phi1,phi2,phi3,yp(1)];
    m = 1;
    for i = 1:4
        up = up + theta(i)*w(i);
        m   = m + phiP(i)*phiP(i);
    end
    
    dw1 = -lambda*w1 + g*up;
    dw2 = -lambda*w2 + g*yp(1);
    dyp = [0,0]; dym = [0,0]; dz = [0,0]; dphi1 = [0,0]; dphi2 = [0,0]; dphi3 = [0,0]; 

    
    for i = 1:2
        
       dyp(i)   = Ap(2*i-1)*yp(1)   + Ap(2*i)*yp(2)   + Bp(i)*up;
       dym(i)   = Am(2*i-1)*ym(1)   + Am(2*i)*ym(2)   + Bm(i)*ref;   
       dz (i)   = Am(2*i-1)*z(1)    + Am(2*i)*z(2)    + Bm(i)*up;
       dphi1(i) = Am(2*i-1)*phi1(1) + Am(2*i)*phi1(2) + Bm(i)*w1;
       dphi2(i) = Am(2*i-1)*phi2(1) + Am(2*i)*phi2(2) + Bm(i)*w2;
       dphi3(i) = Am(2*i-1)*phi3(1) + Am(2*i)*phi3(2) + Bm(i)*yp(1);
    end
    dq = [0,0,0,0]; dr = zeros(1,16); dtheta = zeros(1,4);
    for i = 1:4
        var = 0;
        dq(i) = -beta*q(i) - phiP(i)*z(1)/m;
        for j = 1:4
            dr((i-1)*4 + j) = -beta*r((i-1)*4 + j) + phiP(i)*phiP(j)/m;
            var = var + r(j);
        end
        dtheta(i) = -gamma*var*theta(i) + q(i);
    end
    Z(n+1,1:2)      = Eulerprogression(z,dz,2,dt)';
    W1(n+1,1)      = Eulerprogression(w1,dw1,1,dt)';
    W2(n+1,1)      = Eulerprogression(w2,dw2,1,dt)';
    Phi1(n+1,1:2)  = Eulerprogression(phi1,dphi1,2,dt)';
    Phi2(n+1,1:2)  = Eulerprogression(phi2,dphi2,2,dt)';
    Phi3(n+1,1:2)  = Eulerprogression(phi3,dphi3,2,dt)';
    Q(n+1,1:4)     = Eulerprogression(q,dq,4,dt)';
    R(n+1,1:16)    = Eulerprogression(r,dr,16,dt)';
    Theta(n+1,1:4) = Eulerprogression(theta,dtheta,4,dt)';
    Ym(n+1,1:2)    = Eulerprogression(ym,dym,2,dt);
    Yp(n+1,1:2)    = Eulerprogression(yp,dyp,2,dt);
    Up(n+1,1) = up;
    %plot(n,Theta(n+1,1:4),'kx')
    %hold on
    %plot(n,Up(n+1,1),'rx')
    %pause(0.05);
end
function ynext = Eulerprogression(State, Derivative, cols,dt)
    ynext = zeros(1,cols);
    for i = 1:cols
        ynext(i) = State(i) + dt*Derivative(i); 
    end
end

