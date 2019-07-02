RTD_filtered = zeros(size(S.millis,2),1);
alfa = 0.05;
for i = 2:size(S.millis,2)
    if isnan(S.RTD(i))
        S.RTD(i) = S.RTD(i-1);
    end
    if isnan(S.currentEst(i)) || isnan(S.millis(i))
       disp (i)
       disp("Current");
       disp(S.currentEst(i));
       disp("millis");
       disp(S.millis(i));
       
    end
    RTD_filtered(i) = RTD_filtered(i-1) + alfa*(S.RTD(i-1) - RTD_filtered(i-1));
    
end
%%
S.RTD = RTD_filtered(1:89210)';
S.millis = S.millis(1:89210);
S.currentEst = S.currentEst(1:89210);
S.currentMeas = S.currentMeas(1:89210);
%%
figure(1)
clf(1)
plot(S.RTD)
hold on
plot(RTD_filtered)