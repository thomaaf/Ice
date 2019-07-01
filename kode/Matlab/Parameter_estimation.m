% Filter = S^2 + lambda1*S + lambda0*S

clear a b c d y lambda1 lambda2 dt dy i phi_1 phi_2 u



%figure(1)
%clf(1)
%[z,zhat,e,theta] = sim(12,40,S);
figure(1)
clf(1)
figure(2)
clf(2)
for i = 1:3
    disp(i)
    figure(1)
    [z,zhat,e,theta,y2,y] = sim(i+1,10,S);
    subplot(3,3,i)
    hold on;
    plot(S.millis/1000,z,'-.')
    plot(S.millis/1000,zhat)
    title("Filter coeff: "+4*i + " Gamma: " + 40)
    legend("Z real","Z estimated")
    axis([160 170 -1 6])
    subplot(3,3,i + 3);
    title("Parameter error zoomed");
    hold on;
    plot(S.millis/1000,e)
    axis([160 170 -2 2])
    
    subplot(3,3,i + 6);
    hold on;
    title("Parameter error ");
    plot(S.millis/1000,e)
    
    figure(2)
    subplot(3,2,(i-1)*2+1)
    title("theta " + i)
    plot(theta)
    subplot(3,2,(i-1)*2+2)
    plot(y2)
    hold on
    plot(y);
    legend("y2","S.RTD")
end
%%

%%
function [z,zhat,e,theta,y2,y] = sim(var,gamma,S)
% Plant  : a*y^^ + b*y^ + c*y = d*u
a = 1; b=2; c = 3; d = 4; lambda1 = var*2; lambda0 = var^2;
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
[0,d/a, b/a,c/a];

for i = 2:size(S.millis,2)
    %dt(i) = (S.millis(i) - S.millis(i-1))/2000;
    u(i) = sin(S.millis(i)/1000) ;
    %u(i) = S.currentEst(i);
    %plant
    dy = plant(y(i-1,1),y(i-1,2),u(i));
    %dy2 = plant(y2(i-1,1),y2(i-1,2),u(i),du(i));
    
    % Estimation
    Ac = [-lambda1, -lambda0; 1 0]; L = [1;0];
    lambda = [lambda1; lambda0];
    %y(i-1,1) = S.RTD(i-1);
    z(i) = y(i-1,1) + lambda'*[phi_2(i-1,1);phi_2(i-1,2)];
    
    phi(i-1,1:4) = [phi_1(i-1,1:2)';phi_2(i-1,1:2)']';
    
    zhat(i) = theta(i-1,1:4)*phi(i-1,1:4)';
    y2(i) = zhat(i-1) - lambda'*[phi_2(i-1,1);phi_2(  i-1,2)];
    e(i) = (z(i) - zhat(i))/(1 + phi(i-1,1:4)*phi(i-1,1:4)');
    
    %
    
    dphi_1 = Ac*[phi_1(i-1,1);phi_1(i-1,2)] + L*u(i-1);
    dphi_2 = Ac*[phi_2(i-1,1);phi_2(i-1,2)] - L*y(i-1,1);    
    dtheta = gamma*e(i)*phi(i-1,2:4)';
    dtheta = [0 ;dtheta];
    y(i, 1:2)     =y(i-1, 1:2)+ dt(i)*[dy(1), dy(2)];
    
    phi_1(i, 1:2) =phi_1(i-1, 1:2)+ dt(i)*[dphi_1(1), dphi_1(2)];
    phi_2(i, 1:2) =phi_2(i-1, 1:2)+ dt(i)*[dphi_2(1), dphi_2(2)];
    theta(i, 1:4) =theta(i-1, 1:4)+ dt(i)*[dtheta(1), dtheta(2),dtheta(3),dtheta(4)];
end

end
function dy = plant(y1,y2,u)
    b0 = 1; a1 = 2; a0 = 3;
    dy(1) = y2;
    dy(2) = (b0*u - a1*y2 - a0*y1);

end

