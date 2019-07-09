N = 1000;
dt = 0.1;

Up = zeros(N,1);
Epsilon = zeros(N,1);
Theta = zeros(N,4); Theta(1,1:4) = [0,0,0,1];
Phip = zeros(N,4);
W = zeros(N,2);
Z = zeros(N,1);
Yp = zeros(N,2);
Ym = zeros(N,1);

gamma = 1;
noise = zeros(N,1);
for n = 1:N
    noise(n,1) =  wgn(1,1,1)/10;
    yp = Yp(n,1);
    r = sin(n*dt);
    w1 = W(n,1);
    w2 = W(n,2);
    phip    = [Phip(n,1),Phip(n,2),Phip(n,3),Phip(n,4)];
    theta   = [Theta(n,1), Theta(n,2), Theta(n,3), Theta(n,4)];
    z       = Z(n,1); 
    ym = Ym(n,1);
    up = theta(1)*w1 + theta(2)*w2 + theta(3)*yp + theta(4)*r +noise(n);
    epsilon = epsilonCalc(phip,theta,z);
    Epsilon(n+1,1) = epsilon;
    
    
    dtheta  = dthetaCalc(epsilon,phip,gamma);
    dphip   = dphipCalc(w1,w2,yp,phip);
    dw      = dwCalc(up,yp,w1,w2);
    dz      = dzCalc(up,z);
    dyp     = dypCalc(up,[Yp(n,1),Yp(n,2)]);

    Theta(n+1,1:4) = theta + dt*dtheta;
    Phip(n+1,1:4 ) = [phip(1:3)  + dt*dphip, yp];
    W(n+1, 1:2)    = [w1, w2] + dt* dw;
    Z(n+1,1)       = z + dt*dz;
    Yp(n+1,1:2)    = [Yp(n,1),Yp(n,2)] + dt*dyp;
    Ym(n+1,1)      = Ym(n,1) + dt*(-am*ym + r);
    
end
figure(2)
clf(2)
subplot(2,1,1)
plot(0:dt:N*dt,Ym)
hold on
plot(out.Ym)
legend("matlab","Simulink")
subplot(2,1,2)
plot(0:dt:N*dt,sin(0:dt:N*dt));
hold on
plot(out.ref);
figure(1)
clf(1)
subplot(2,1,1)
plot(0:dt:N*dt, Theta)
hold on
plot(out.simout)
subplot(2,1,2);
plot(0:dt:N*dt,Epsilon)
hold on
plot(out.epsilon)
function epsilon = epsilonCalc(phip,theta,z)
    m = 1 + phip*phip';
    zhat = theta*phip';
    %for i = 1:4
    %    m = m + phip(i)*phip(i);
    %    zhat = theta(i)*phip(i);
    %end
    epsilon_unormalized = z - zhat;
    epsilon = epsilon_unormalized/m;
    
end

function dtheta = dthetaCalc(epsilon,phip,gamma)
    dtheta = zeros(1,4);
    dtheta = gamma*epsilon*phip;
    %for i = 1:4
    %    dtheta(i) = gamma*epsilon*phip(i);
    %end
end

function dphip = dphipCalc(w1,w2,yp,phip)
    am  = 1;
    dphip1  = -am*phip(1)   + w1;
    dphip2  = -am*phip(2)   + w2;
    dphip3  = -am*phip(3)   + yp;
    dphip   =  [dphip1, dphip2, dphip3];
end

function dw = dwCalc(up,yp,w1,w2)
    lambda = 1;
    dw1     = -lambda*w1 + up;
    dw2     = -lambda*w2 + yp;    
    dw = [dw1, dw2];
end

function dz = dzCalc(up,z)
    am = 1;
    dz = -am*z + up;
end

function dyp = dypCalc(up,yp)
    a1 = 1; a0 = 1;
    dyp = zeros(1,2);
    dyp(1) = yp(2);
    dyp(2) = up - a1*yp(2) - a0*yp(1);
end
