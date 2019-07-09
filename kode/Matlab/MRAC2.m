function [y,up,uf,w1,w2,e1,e1hat,epsilon,theta,r,ym,phi1,phi2,phi3,phi4] = MRAC2(S,var,gamma0)

%I = size(S.millis,2);
I = 1000;
PlantOrder = 2;
RefOrder = 2; 
t  = S.millis/1000;
dt = [0, (S.millis(2:end) - S.millis(1:end-1))/1000];
r = 3*cos(t);
r = -2*ones(I,1);
dr = -sin(t);

%Matricies and constants
gamma1 = 1;
w0 = 3 ; zeta = 1;
F  = -var; g = 1;
Wm = [0 1; -w0^2 -2*zeta*w0]; B = [0 ;w0^2];
du =  zeros(I,1);

%Algebraic states
e1 = zeros(I,1);
e1hat = zeros(I,1);
epsilon = zeros(I,1);
up = zeros(I,1);

%Derivative states;
y  = [2,2;zeros(I-1,PlantOrder)];
ym = zeros(I,PlantOrder);
w1 = zeros(I,PlantOrder-1);
w2 = zeros(I,PlantOrder-1);
rho = zeros(I,1);
w00 = zeros(I,1);
uf  = zeros(I,2);
phi1 = zeros(I,RefOrder);
phi2 = zeros(I,RefOrder);
phi3 = zeros(I,RefOrder);
phi4 = zeros(I,RefOrder);
theta = zeros(I,4); theta(1,1:end) = [0,0,0,1];



for i = 2:I
    %Algebraic variables 
    x0 = w00(i-1) + rho(i-1)^2/(2*gamma1);
    phi = [phi1(i-1,1);phi2(i-1,1);phi3(i-1,1);phi4(i-1)];
    m = 1 + phi'*phi + uf(i-1)^2;
    squiggle = uf(i-1) - theta(i-1,1:end)*phi;
    
    %Algebraic arrays
    e1 (i)    = y(i-1,1) - ym(i-1,1);
    
    e1hat(i) =  (x0^2*cos(x0))*rho(i-1)*(uf(i-1) - theta(i-1,1:end)*phi);
    epsilon(i) = (e1(i,1) - e1hat(i))/m;
    up(i) = theta(i-1,1:end)*[w1(i-1);w2(i-1);y(i-1);r(i-1)];


    %Derivatives calculation
    dy    = plant(y(i-1,1),y(i-1,2),up(i),du(i));
    dym   = Wm*ym(i-1,1:end)' + B*r(i-1);
    
    dw1   = F*w1(i-1,1:end)' + g*up(i);
    dw2   = F*w2(i-1,1:end)' + g*y(i-1,1);
    drho  = (x0^2*cos(x0))*gamma1*epsilon(i)*squiggle;
    dw00  = epsilon(i)^2*m;
    duf   = Wm*uf(i-1,1:end)' + B*up(i-1);
    dphi1 = Wm*phi1(i-1,1:end)' + B*w1(i-1);
    dphi2 = Wm*phi2(i-1,1:end)' + B*w2(i-1);
    dphi3 = Wm*phi3(i-1,1:end)' + B*y(i-1,1);
    dphi4 = Wm*phi4(i-1,1:end)' + B*r(i-1);
    dtheta = -(x0^2*cos(x0))*gamma0*epsilon(i)*phi;

    %Euler progression7
    y(i, 1:2)     =  y(i-1, 1:2)+ dt(i)*[dy(1), dy(2)];
    %y(i, 1:2)    =  [2,2];
    ym(i, 1:2)    =  ym(i-1, 1:2)+ dt(i)*dym';
    w1(i,1:end)   = w1(i-1,1:end) + dt(i)*dw1';
    w2(i,1:end)   = w2(i-1,1:end) + dt(i)*dw2'; 
    rho(i,1:end)  = rho(i-1,1:end) + dt(i)*drho';
    w00(i,1:end)  = w00(i-1,1:end) + dt(i)*dw00';
    uf(i,1:end)   = uf(i-1,1:end) + dt(i)*duf';
    phi1(i,1:end) = phi1(i-1,1:end) + dt(i)*dphi1';
    phi2(i,1:end) = phi2(i-1,1:end) + dt(i)*dphi2';
    phi3(i,1:end) = phi3(i-1,1:end) + dt(i)*dphi3';
    phi4(i,1:end) = phi4(i-1,1:end) + dt(i)*dphi4';    
    theta(i,1:end) = theta(i-1,1:end) + dt(i)*dtheta';
    
end
end

function dy = plant(y1,y2,u,du)
    n = wgn(3,1,1)/2;
    a0 = 1+n(1); a1 = 5+n(2); b0= 3+n(3); b1 =  0;
    %b0 = 1; a1 = 2; a0 = 3; b1 = 0;
    dy(1) = y2;
    dy(2) = b1*du +  b0*u - a1*y2 - a0*y1 ;

end
