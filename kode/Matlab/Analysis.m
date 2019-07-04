%indicator ="test,Temp_ir,Kalman_p_est_,Kalman_gain,Kalman_est,Kalman_cov,duty,currentMeas,currentEst,RTD,millis";
indicator = "phi1,phi2,phi3,u,duty,tmp,w1,w2,ym,z,theta";
COM = 'COM4';
BAUD = 38400;
[S,indicator,theta] = SerialRead(indicator,COM,BAUD);
clear COM BAUD indicator

%%
%%


figure(1)
clf(1)
%for 
ButtonHandle =  uicontrol('Position',[0 0 60 20],'Style', 'PushButton','String', 'Stop loop','Callback', 'delete(gcbf)');	
ButtonHandle =  uicontrol('Position',[60 0 60 20],'Style', 'togglebutton','String', 'HOLD','Callback',@Togglehold);	

fields = fieldnames(S);
for i= 1:size(fieldnames(S),1)
    uicontrol('Position',[0 i*20 60 20],'Style', 'checkbox','String', fields(i),'Callback', @plot_new);
end

while 1
    pause(0.0000000001); 
	if ~ishandle(ButtonHandle)
		disp('Loop stopped by user');
		break;
	end	      
end

function Togglehold(src,event)
    src.Value
    if src.Value == 0
        hold off
    elseif src.Value == 1
        hold on
    end

end
	
function plot_new(src,event)
    S = evalin('base','S');
    fields = string(fieldnames(S));
    for i= 1:size(fieldnames(S),1)
        if src.String == fields(i)
            if src.Value == 0  
                l = findobj('type','line');
                for j = 1:size(l)
                    if l(j).Tag == fields(i)
                       delete(l(j))
                    end
                end
            elseif src.Value == 1
                a(i) = plot(S.(string(fields(i))),'Tag',fields(i),'DisplayName',fields(i));
            end
        end
    end
    legend
end

