% Filter = S^2 + lambda1*S + lambda0*S

clear a b c d y lambda1 lambda2 dt dy i phi_1 phi_2 u

[z,zhat,e,theta,y2] = Gradient_integral_cost(var,gamma,S,S.currentMeas);
%%
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
    gamma = 10*i;
    var   =1;
    Gradient_instant_cost(var,gamma,S,S.currentMeas)
    [z,zhat,e,theta,y2] = Gradient_instant_cost(var,gamma,S,S.currentMeas);
    subplot(3,3,i)
    hold on;
    plot(S.millis/1000,z,'-.')
    plot(S.millis/1000,zhat)
    title("Filter coeff: "+var + " Gamma: " + gamma)
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
    plot(S.RTD);
    legend("y2","S.RTD")
end
%%

%%
function dy = plant(y1,y2,u)
    b0 = 1; a1 = 2; a0 = 3;
    dy(1) = y2;
    dy(2) = (b0*u - a1*y2 - a0*y1);

end


