function  [yp_data,ym_data,w1_data,w2_data,theta_data,up_data,e1_data,t] = MRAC3(S,lambda,w0, Km,gamma)
    %Plant parameters
    %I = 4554;
    I = size(S.t,2);
    Kp = 1; b0 = 3; a1 = 3; a0 = 1;
 
    
    Ap = [-a1 1; -a0 0]; Bp = [1;b0];
    n = 2; m = 1;
    
    w1_data = zeros(I,n-1);
    w2_data = zeros(I,n-1);
    ym_data = zeros(I,m);
    yp_data  = zeros(I,n);
    theta_data = zeros(I,4);%theta(1,1:end) = [3,-10,2,3];
    up_data = zeros(I,1);
    e1_data = zeros(I,1);
    t = (S.t(1:I)/1000)';
    %r = ones(I,1);
    %r = 0.5*sin(0.7*t)+2*sin(5.9*t);
    r = -10;
    dt = (S.t(2:end) - S.t(1:end-1))/1000;
    gamma = diag([0.01,0.01,0.005,0.005]);
for i = 1:I-1
    theta = theta_data(i,1:end)'; w1 = w1_data(i,1:end)'; w2 = w2_data(i,1:end)';
    ym = ym_data(i,1:end)'; yp = S.yp(1,i);%yp = yp_data(i,1:end)'; 

    w        = [w1,w2,yp(1),r]';
    e1      = yp(1) - ym;
    if theta'*w>0
        up = min(1.2,theta'*w);
    else
        up =max(-1.2,theta'*w);
    end
    dtheta = -gamma*e1'*w;
    dw1    = -lambda*w1 + up;
    dw2    = -lambda*w2 + yp(1);
    dym    = -w0*ym     + Km*r;
    %dyp     = Ap*yp     + Bp*up; 
    %dyp     = [0;0];

    up_data(i+1)  =up;
    e1_data(i+1) = e1;
    w1_data(i+1,1:end)    = eulerProgression(w1_data(i,1:end)'    , dt(i),dw1);
    w2_data(i+1,1:end)    = eulerProgression(w2_data(i,1:end)'    , dt(i),dw2);
    theta_data(i+1,1:end) = eulerProgression(theta_data(i,1:end)' , dt(i),dtheta);
    ym_data(i+1,1:end)    = eulerProgression(ym_data(i,1:end)'    , dt(i),dym);
    %yp_data(i+1,1:end)    = eulerProgression(yp_data(i,1:end)'    , dt(i),dyp);
    yp_data(i,1) = S.yp(i);
end


end
function y_next = eulerProgression(y_prev,dt,dy)
    y_next = (y_prev + dt * dy)';
end
