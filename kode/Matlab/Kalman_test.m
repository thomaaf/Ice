

clear a b c x P R H Q xtilde Ptilde K
a = -1/1.5680;
b = 38.9209/1.5680;
c = 1;
x(1) = S.RTD(1); n(1) = 2;
P_(1) = x(1)^2;

R = 10;
E = 0.01;
H = 1;

for k = 1:1068
    dt = (S.millis(k+1) - S.millis(k))/1000;
    phi = 1 + a*dt;
    u = -S.currentMeas(k) - 25/b*a
    B = b*dt;
    n(k+1) = phi*n(k) + B*u;
    
    K(k) = P_(k)*H/(H*P_(k)*H + R);
    y(k) = S.Temp_ir(k);
    xhat(k) = x(k) + K(k)*(y(k) - H*x(k));
    Phat(k) = (1-K(k)*H)*P_(k)*(1-K(k)*H) + K(k)*R*K(k);
    x(k+1) = phi*xhat(k) + B*u;
    P_(k+1) = phi*Phat(k) + E;
end

% for k = 2:676
%     dt = (S.millis(k) - S.millis(k-1))/1000;
%     phi = 1 + a*dt;
%     B = b*dt;
%     Q = 1/3*dt^3*B^2 + 1/2*B^2*a*dt^4 + 1/5*a^2*B^2*dt^5;
%     Q = 1;
%     xtilde(k) = phi*x(k-1);
%     Ptilde(k) = phi*P(k-1)*phi + Q;
%     K(k) = Ptilde(k)*H/(H*Ptilde(k)*H + R);
%     z = S.Temp_ir(k-1);
%     z - H*xtilde(k)
%     x(k+1) = xtilde(k) + K(k)*(z - H*xtilde(k));
%     P(k+1) = (1-K(k)*H)*Ptilde(k);
%     
% end
figure(1)
clf(1)
subplot(2,1,1);
hold on;
plot(S.RTD)
plot(S.Temp_ir)
plot(x)
plot(n)
legend("tmp real","tmp meas","temp est","Pure model")
subplot(2,1,2);
hold on
plot(x)
plot(xhat);
legend("x","xhat")
figure(2)
clf(2)
subplot(2,1,1)
hold on;
plot(P_)
plot(Phat)
legend("P","Phat")
subplot(2,1,2)

plot(K);
legend("K")