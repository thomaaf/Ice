
figure(1)
clf(1)
for i = 1:1
[y,u,w1,w2,z,zhat,e,theta,r,ym,phi1,phi2,phi3,e1]=MRAC(S,2,i*10);
subplot(3,1,i)
plot(y(:,1))
hold on;
plot(ym(:,1))

end
figure(2)
clf(2)
plot(ym(:,1));
hold on
plot(r)
