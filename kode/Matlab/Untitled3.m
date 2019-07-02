t = S.millis/1000;
input = sin(t)+sin(5*t)+sin(0.14*t) + 4 ;
    
[z,zhat,e,theta,y,y2,P,detP,u,du]= PureLSQ(1,S,S.currentMeas);
% figure(1)
% clf(1)
%%
% 
% * ITEM1
% * ITEM2
% 
% subplot(2,1,1)
% plot(theta,'DisplayName','theta')
% subplot(2,1,2)
% plot(theta,'DisplayName','theta')