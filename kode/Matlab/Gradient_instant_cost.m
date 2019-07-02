function [z,zhat,e,theta,y2] = Gradient_instant_cost(var,gamma,S,input)
% Plant  : a*y^^ + b*y^ + c*y = d*u
lambda1 = var*2; lambda0 = var^2;
% y = y1, y2
y              = zeros(size(S.millis,2),2);

y2              = zeros(size(S.millis,2),1);
phi_1          = zeros(size(S.millis,2),2);
phi_2          = zeros(size(S.millis,2),2);
phi            = [phi_1,phi_2];
u              = zeros(size(S.millis,2),1);
theta          = zeros(size(S.millis,2),4);
z              = zeros(size(S.millis,2),1);
zhat           = zeros(size(S.millis,2),1);
e              = zeros(size(S.millis,2),1);
dt             = [0, (S.millis(2:end) - S.millis(1:end-1))/1000];
Ac = [-lambda1, -lambda0; 1 0]; L = [1;0];
lambda = [lambda1; lambda0];

for i = 2:size(S.millis,2)
    u(i) = input(i);
    
    %plant
    dy = plant(y(i-1,1),y(i-1,2),u(i));
    % Estimation
    % y = measurment; z = filtered measurment; zhat = filtered, estimated
    % value; phi, theta obvious; y2 = estimated model value; e = estimation
    % error;
    
    %y(i-1,1) = S.RTD(i-1);

    z(i) = y(i-1,1) + lambda'*[phi_2(i-1,1);phi_2(i-1,2)];    

    phi(i-1,1:4) = [phi_1(i-1,1:2)';phi_2(i-1,1:2)']';
    
    zhat(i) = theta(i-1,1:4)*phi(i-1,1:4)';
    
    y2(i) = zhat(i-1) - lambda'*[phi_2(i-1,1);phi_2(  i-1,2)];
    
    e(i) = (z(i) - zhat(i))/(1 + phi(i-1,1:4)*phi(i-1,1:4)');
    
% Euler progression
    dphi_1 = Ac*[phi_1(i-1,1);phi_1(i-1,2)] + L*u(i-1);
    dphi_2 = Ac*[phi_2(i-1,1);phi_2(i-1,2)] - L*y(i-1,1);    
    dtheta = gamma*e(i)*phi(i-1,1:4)';
    y(i, 1:2)     =y(i-1, 1:2)+ dt(i)*[dy(1), dy(2)];
%Data storage
    phi_1(i, 1:2) =phi_1(i-1, 1:2)+ dt(i)*[dphi_1(1), dphi_1(2)];
    phi_2(i, 1:2) =phi_2(i-1, 1:2)+ dt(i)*[dphi_2(1), dphi_2(2)];
    theta(i, 1:4) =theta(i-1, 1:4)+ dt(i)*[dtheta(1), dtheta(2),dtheta(3),dtheta(4)];
end

end

function dy = plant(y1,y2,u)
    b0 = 1; a1 = 2; a0 = 3; b1 = 0;
    dy(1) = y2;
    dy(2) = (  b0*u - a1*y2 - a0*y1) + wgn(1,1,1)*2;

end
