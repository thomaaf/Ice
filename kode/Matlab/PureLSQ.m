function [z,zhat,e,theta,y,y2,P,detP,u,du] = PureLSQ(var,S,input)
% Plant  : a*y^^ + b*y^ + c*y = d*u
%Filter coefficients, filter on form (s+var)^2
lambda1 = var*2; lambda0 = var^2; 

%Storage lists and initial values initialization
dt           = [0, (S.millis(2:end) - S.millis(1:end-1))/1000];
t            = S.millis/1000;
y           = zeros(size(S.millis,2),2);
y2          = zeros(size(S.millis,2),1);
u           = zeros(size(S.millis,2),1);
du          = zeros(size(S.millis,2),1);

phi_1       = zeros(size(S.millis,2),2);
phi_2       = zeros(size(S.millis,2),2);
phi         = [phi_1,phi_2];
theta       = zeros(size(S.millis,2),4);
z           = zeros(size(S.millis,2),1);
zhat        = zeros(size(S.millis,2),1);
e           = zeros(size(S.millis,2),1);

P           = zeros(4*size(S.millis,2),4);
detP        = zeros(size(S.millis,2),1);
P(1:4,1:4)  = 10*eye(4,4);
detP(1)     = det(P(1:4,1:4));

%Filter design matrix
Ac = [-lambda1, -lambda0; 1 0]; L = [1;0];
lambda = [lambda1; lambda0];

for i = 2:size(S.millis,2)
    %Input definitions
    u(i) = input(i);
    du(i) = (input(i) - input(i-1))/2;
    %u(i) = sin(t(i))+sin(5*t(i))+sin(0.14*t(i))+sin(0.01*t(i)) + 4 ;
    %du(i) =  cos(S.millis(i)/1000)+5*cos(5*S.millis(i)/1000)+0.14*cos(0.14*S.millis(i)/1000);
    
    %plant
    %dy = plant(y(i-1,1),y(i-1,2),u(i),du(i),theta(i-1,1:4)+[0,0,lambda']);
    
%% Estimation
    % $$\phi = \pmatrix{\phi_1 \cr \phi_2  \cr}$$
    %
    % $$z_{i-1} = y_{i-1} + \lambda^T \phi_{2,i-1} $$
    %
    % $$\hat{z_{i-1}} = \theta^T_{i-1}\phi_{i-1} $$
    %
    % $$e_{i-1} = z_{i-1} - \hat{z_{i-1}} $$
    %
    % $$\hat{y_{i-1}} = \hat{z_{i-1}} - \lambda^T \phi_{2,i-1} $$
    %
    % $$ y = measurment;\quad z = filtered \quad measurment;\quad \hat{z} =
    % filtered, estimated \quad
    % value; $$
    %
    % $$ \phi,\quad \theta \quad obvious;\quad y_2 = \hat{y} = estimated\quad model\quad value;\quad e = estimation
    % \quad error;\quad P = Covariance\quad matrix, detp = det(p)$$
    %
    
    y(i-1,1) = S.RTD(i-1);

    phi(i-1,1:4) = [phi_1(i-1,1:2)';phi_2(i-1,1:2)']';
    z(i-1)       = S.RTD(i-1) + lambda'*[phi_2(i-1,1);phi_2(i-1,2)];    
    zhat(i-1)    = theta(i-1,1:4)*phi(i-1,1:4)';
    e(i-1)       = (z(i-1) - zhat(i-1));
    y2(i-1)      = zhat(i-1) - lambda'*[phi_2(i-1,1);phi_2(  i-1,2)];
    detP(i-1)    = det(P(1+4*(i-2):4*(i-1),1:4)');
%% Derivatives calculations
    % $$\dot{\phi_1} = A_c\phi_1 + Lu $$
    % 
    % $$\dot{\phi_2} = A_c\phi_2 - Ly $$
    %
    % $$\dot{P} = \beta P - P\phi^T \phi P $$
    %
    % $$\dot{\theta} = Pe\phi $$

    dphi_1 = Ac*[phi_1(i-1,1);phi_1(i-1,2)] + L*u(i-1);
    dphi_2 = Ac*[phi_2(i-1,1);phi_2(i-1,2)] - L*S.RTD(i-1);  
    dP     = 0.03*P(1+4*(i-2):4*(i-1),1:4)' + Dp(P(1+4*(i-2):4*(i-1),1:4)',phi(i-1,1:4)'); 
    dtheta = P(1+4*(i-2):4*(i-1),1:4)'*e(i-1)*phi(i-1,1:4)';
%% Euler progression
    % $$\phi_{1,i} = \phi_{1,i-1} + \Delta t\dot{\phi_1}$$
    %
    % $$\phi_{2,i} = \phi_{2,i-1} + \Delta t\dot{\phi_2}$$
    %
    % $$\theta_{i} = \theta_{1,i-1} + \Delta t\dot{\theta}$$
    %
    % $$P_{i} = P_{i-1} + \Delta t\dot{p}$$
    %y(i, 1:2)     =  y(i-1, 1:2)+ dt(i)*[dy(1), dy(2)];
    phi_1(i, 1:2) =phi_1(i-1, 1:2)+ dt(i)*[dphi_1(1), dphi_1(2)];
    phi_2(i, 1:2) =phi_2(i-1, 1:2)+ dt(i)*[dphi_2(1), dphi_2(2)];
    theta(i, 1:4) =theta(i-1, 1:4)+ dt(i)*[dtheta(1), dtheta(2),dtheta(3),dtheta(4)];
    P(1+4*(i-1):4*(i),1:4) = P(1+4*(i-2):4*(i-1),1:4) + dt(i)*dP';
    
end
y = y(:,1);
end

function dy = plant(y1,y2,u,du,param)
    b1 = param(1); b0 = param(2); a1 = param(3); a0 = param(4);
    %b0 = 1; a1 = 2; a0 = 3; b1 = 0;
    dy(1) = y2;
    dy(2) = (b1*du+  b0*u - a1*y2 - a0*y1);

end

function dp = Dp(P,phi)
    dp = -P*(phi*phi')*P;
end
