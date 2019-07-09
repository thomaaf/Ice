

%[y,up,uf,w1,w2,e1,e1hat,epsilon,theta,r,ym,phi1,phi2,phi3,phi4]=MRAC2(S,2,i*10);
%[y,u,w1,w2,z,zhat,e,theta,r,ym,phi1,phi2,phi3,e1] =MRAC(S,2,1);

 lambda = 1; w0 = 1; Km = 1; gamma = 0.01;  Kp = 1; b0 = 3; a1 = 3; a0 = 1;
[yp_data,ym_data,w1_data,w2_data,theta_data,up_data,e1_data,t_MRAC] = MRAC3(S,lambda,w0, Km,gamma);


%tspan = [0 20];  y0 =  [0;0;0;0;0;0;0;0;0;0];
%u_prev = 0; t_prev = 0;
%[t,y]= ode23(@(t,y)vdp1(t,y),tspan,y0); 
%%
Sw = [S.w1' S.w2',S.yp(1,1:end)',ones(size(S.w1,2),1) ];
w  = [w1_data, w2_data, yp_data(:,1), ones(size(w2_data,1),1) ];
Sx = [S.yp(1,1:end)', S.ym',S.e1',S.up'];
x  = [yp_data(:,1),ym_data,e1_data,up_data];
xtitle = ['yp','ym','e1','up'];
figure(1);clf(1)
figure(2);clf(2)
figure(3);clf(3)
S.t = S.t(1:end)';
for i=1:4
figure(1)
subplot(2,2,i)
plot(theta_data(:,i),'kx');
hold on;
plot(S.theta(:,i),'r')
title("theta "+i)
legend("Simulation","Arduino")

figure(2)
subplot(2,2,i)
plot(w(:,i),'kx');
hold on;
plot(Sw(:,i),'r')
title("w "+i)
legend("Simulation","Arduino")

figure(3)
subplot(2,2,i)
plot(x(:,i),'kx');
hold on;
plot(Sx(:,i),'r')
title(xtitle((i-1)*2+1:(i-1)*2+2))
legend("Simulation","Arduino")

end


figure(4)
clf(4)
subplot(1,2,1)
plot(x(:,1),'k');
hold on;
plot(x(:,2),'k:');
legend('plant','model')
subplot(1,2,2)
plot(Sx(:,1),'r')
hold on;
plot(Sx(:,2),'r:')
legend('plant','model')
function y = vdp1(t,y)

    lambda = 10; w0 = 1; Km = 1; gamma = 10;  Kp = 1; b0 = 3; a1 = 3; a0 = 1;
    theta = y(1:4); w1 = y(5); w2 = y(6); ym = y(7); yp = y(8:9); upder = y(10);
    
    Ap = [-a1 1; -a0 0]; Bp = [1;b0];
    r = 1;
    

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
    dyp     = Ap*yp     + Bp*up;    
    dyp     = [0;0];

    y = [dtheta;dw1;dw2;dym;dyp;up];
 
end
