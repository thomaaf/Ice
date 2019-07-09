indicator ="w1,w2,ym,yp,z,phi1,phi2,phi3,up,current,duty,e,t,theta,R,Q";
%Mrac2
%indicator = "e1Int,w1,w2,ym,yp,up,current,duty,e1,t,theta";
COM = 'COM4';
BAUD = 57600;
[S,readindicator] = SerialRead(indicator,COM,BAUD);
clear COM BAUD indicator
if size(instrfind(),1)>0
	fclose(instrfind());
	delete(instrfind());	   
end
Retroplot(readindicator,S);

%%
%%
% [y,u,w1,w2,z,zhat,e,theta,r,ym,phi1,phi2,phi3,e1] =MRAC(S,2,1);
% gridx = 3;
% gridy = 3;
% figure(1)
% clf(1)
% subplot(gridy ,gridx ,1);
% 
% plot(S.u); hold on; plot(u);title("U");
% subplot(gridy ,gridx,2);
% plot(S.z); hold on; plot(z);title("z");
% subplot(gridy ,gridx,3);
% plot(S.ym); hold on; plot(ym(:,1));plot(S.tmp);title("ym");
% 
% subplot(gridy ,gridx,4);
% plot(S.w1); hold on; plot(w1);title("w1");
% subplot(gridy ,gridx,5);
% plot(S.w2); hold on; plot(w2);title("w2");
% subplot(gridy ,gridx,6);
% plot(S.theta); hold on;plot(theta); title("theta");
% subplot(gridy ,gridx,7);
% plot(S.phi1); hold on; plot(phi1(:,1));title("ph1");
% subplot(gridy ,gridx,8);
% plot(S.phi2); hold on; plot(phi2(:,1));title("phi2");
% subplot(gridy ,gridx,9);
% plot(S.phi3); hold on; plot(phi3(:,1));title("phi3");
% 
% figure(2)
% clf(2)
% subplot(gridy ,gridx ,1);
% 
% plot(S.u);title("U");
% subplot(gridy ,gridx,2);
% plot(S.z); hold on; title("z");
% subplot(gridy ,gridx,3);
% plot(S.ym); hold on; plot(S.tmp);title("ym");
% 
% subplot(gridy ,gridx,4);
% plot(S.w1); hold on; title("w1");
% subplot(gridy ,gridx,5);
% plot(S.w2); hold on; title("w2");
% subplot(gridy ,gridx,6);
% plot(S.theta); hold on; title("theta");
% subplot(gridy ,gridx,7);
% plot(S.phi1); hold on; title("ph1");
% subplot(gridy ,gridx,8);
% plot(S.phi2); hold on; title("phi2");
% subplot(gridy ,gridx,9);
% plot(S.phi3); hold on; title("phi3");
% 





function Retroplot(readindicator,S)
if readindicator == 1
    figure(1)
    clf(1)
    
    %for 
    %ButtonHandle =  uicontrol('Position',[0 0 60 20],'Style', 'PushButton','String', 'Stop loop','Callback', 'delete(gcbo)');	
    ButtonHandle2 =  uicontrol('Position',[0 0 60 20],'Style', 'togglebutton','String', 'HOLD','Callback',@Togglehold);	

    fields = fieldnames(S);
    for i= 1:size(fieldnames(S),1)
        uicontrol('Position',[0 i*20 60 20],'Style', 'checkbox','String', fields(i),'Callback', @plot_new);
    end
    tmpfields = string(fieldnames(S));
    plot(S.(string(fields(1))),'Tag',tmpfields(1),'DisplayName',tmpfields(1));
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
                plot(S.(string(fields(i))),'Tag',fields(i),'DisplayName',fields(i));
            end
        end
    end
    legend
end

