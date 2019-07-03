function [y,u,w1,w2,z,zhat,e,theta,r,ym,phi1,phi2,phi3,e1] = MRAC(S,var,gamma)

I = size(S.millis,2);
PlantOrder = 2;
RefOrder = 2; 
w0 =3; zeta = 1;
y = zeros(I,PlantOrder);
ym = zeros(I,PlantOrder);
t            = S.millis/1000;
dt           = [0, (S.millis(2:end) - S.millis(1:end-1))/1000];
r = 3*cos(t);
%r = 5*ones(I,1);
dr = -sin(t);
w1 = zeros(I,PlantOrder-1);
w2 = zeros(I,PlantOrder-1);
z  = zeros(I,PlantOrder);
phi1 = zeros(I,RefOrder);
phi2 = zeros(I,RefOrder);
phi3 = zeros(I,RefOrder);
zhat = zeros(I,1);
theta = zeros(I,4);
theta(1,1:end) = [0,0,0,1];
e = zeros(I,1);
e1 = zeros(I,2);
u = zeros(I,1);
%du = -sin(t)
F = -var; g = 1;
Wm = [0 1; -w0^2 -2*zeta*w0]; B = [0 ;w0^2];
du =  zeros(I,1);
for i = 2:I
   % if(i == I/5)
      %  r = 5*ones(I,1);
   % end
    %% 
    %
    % $$W_m(s) = \frac{1}{s^2 + am_1s + am_0},\quad am_1 = 2\zeta\omega_0,\quad am_0 = \omega_0^2   $$
    %
    % $$\phi = W_m(s)\pmatrix{\omega_1 \cr \omega_2  \cr y_p \cr \frac{y_p}{W_m(s)}} <=>$$
    % $$\phi = W_m(s)\pmatrix{\phi_1 \cr \phi_2  \cr \phi_3 \cr y_p} =>$$
    % $$\phi_1 = W_m(s)\omega_j => \dot{\phi_j} = \pmatrix{0 & 1 \cr -\omega_0^2 & -2\zeta\omega} \pmatrix{\phi_{j,1} \cr \phi_{j,2}} + \pmatrix{ 0 \cr 1}\omega_j$$
    %
    % $$\phi_{i-1} = \pmatrix{ \phi_{1_{i-1,1)}} & phi_{2_{i-1,1)} } & phi_{3_{i-1,1)} } &  y_{p_{i-1}} }$$
    %
    % $$\hat{z}_{i-1} = \theta_{i-1}^T\phi $$
    %
    % $$z = W_m(s)u <=> zs^2 + s z am_1 + z am_0 = u \quad => \pmatrix{ \dot{z}_1 \cr \dot{z}_2} = \pmatrix{0 & 1 \cr -\omega_0^2 & -2\zeta\omega} \pmatrix{ z_1 \cr \dot{z}_2} + \pmatrix{ 0 \cr 1}u $$
    %    
    % $$e_{i-1} = \frac{z_{1,i-1} - \hat{z}_{i-1}}{1 + \phi_{i-1}\phi_{i-1}^T} $$
    %
    % $${u}_{i} = \theta_{i-1}^T \pmatrix{ \omega_{1,1_{i-1}} \cr \omega_{2,1_{i-1}} \cr y_{i-1} \cr r_{i-1}  } $$
    %
    % $$\dot{\omega_1} = F\omega_1 + gu, \quad F from \Lambda = s + \lambda_0 => F = -\lambda_0, \quad g = 1 $$
    %
    % $$\dot{\omega_1} = F\omega_1 + gy, $$
    %
    % $$\dot{\theta} = \Gamma e \phi$$
    %    
    phi = [phi1(i-1,1);phi2(i-1,1);phi3(i-1,1);y(i-1)];
    %\pmatrix{ \cr}
    %
    zhat(i-1) = theta(i-1,1:end)*phi;
    
    e(i-1) = (z(i-1,1) - zhat(i-1))/(1 + phi'*phi*0);
   
    u(i) = theta(i-1,1:end)*[w1(i-1);w2(i-1);y(i-1);r(i-1)];
    if(i == I/10)
       r = 2*ones(I,1);
    end   

    %Derivatives calculation
    de1   = Wm*e1(i-1,1:end)' + 1/theta(i-1,4)*(u(i) - theta(i-1,1:end)*[w1(i-1);w2(i-1);y(i-1,1);r(i-1)]); 
    dym   = plantref([ym(i-1,1);ym(i-1,2)],r(i),w0,zeta);
    dy    = plant(y(i-1,1),y(i-1,2),u(i),du(i));
    dw1   = F*w1(i-1,1:end)' + g*u(i);
    dw2   = F*w2(i-1,1:end)' + g*y(i-1,1);
    dz    = Wm*z(i-1,1:end)' + B*u(i);
    dphi1 = Wm*phi1(i-1,1:end)' + B*w1(i-1);
    dphi2 = Wm*phi2(i-1,1:end)' + B*w2(i-1);
    dphi3 = Wm*phi3(i-1,1:end)' + B*y(i-1);
    dtheta = gamma*e(i-1)*phi;
    %Euler progression7
    y(i, 1:2)     =  y(i-1, 1:2)+ dt(i)*[dy(1), dy(2)];
    ym(i, 1:2)    =  ym(i-1, 1:2)+ dt(i)*dym';
    w1(i,1:end)   = w1(i-1,1:end) + dt(i)*dw1';
    w2(i,1:end)   = w2(i-1,1:end) + dt(i)*dw2';
    z(i,1:end)    = z(i-1,1:end) + dt(i)*dz';
    phi1(i,1:end) = phi1(i-1,1:end) + dt(i)*dphi1';
    phi2(i,1:end) = phi2(i-1,1:end) + dt(i)*dphi2';
    phi3(i,1:end) = phi3(i-1,1:end) + dt(i)*dphi3';
    theta(i,1:end) = theta(i-1,1:end) + dt(i)*dtheta';
    e1(i,1:end) = e1(i-1,1:end) + dt(i)*de1';
    
end
z=z(:,1);
end

function dy = plant(y1,y2,u,du)
    n = wgn(3,1,1)/2;
    a0 = 1+n(1); a1 = 5+n(2); b0= 3+n(3); b1 =  0;
    %b0 = 1; a1 = 2; a0 = 3; b1 = 0;
    dy(1) = y2;
    dy(2) = b1*du +  b0*u - a1*y2 - a0*y1 ;

end
function dy = plantref(ym,u,w0,zeta)
    %b0 = 1; a1 = 2; a0 = 3; b1 = 0;
    dy = [0 1; -w0^2 -2*zeta*w0]*ym + [0;w0^2]*u;
end