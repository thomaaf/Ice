%indicator = "ambTemp,DryTempRef,DewTempRef,refstate,ref,light,temperature,duty,dir,dt,integral,t,error";
indicator = "zeroMean,mean,pVal,ambTemp,ref,light,temperature,duty,dir,dt,integral,t,error";
COM = 'COM4'; 
BAUD = 500000;
[S,readindicator] = SerialRead(indicator,COM,BAUD);
clear COM BAUD indicator
if size(instrfind(),1)>0
	fclose(instrfind());
	delete(instrfind());	   
end
%%
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
    fig = figure(1)
    clf(1)

    fig.Position = [760 390 780 400];
    
    %for 
    %ButtonHandle =  uicontrol('Position',[0 0 60 20],'Style', 'PushButton','String', 'Stop loop','Callback', 'delete(gcbo)');	
    bg = uibuttongroup('Visible','on','Position',[.08 0 0.22 .05],'SelectionChangedFcn',@bselection);
    bg1 = uibuttongroup('Visible','on','Position',[0 0 .08 1]);
    bg2 = uibuttongroup('Visible','on','Position',[.3 0 1 .05]);
    HoldButton = uicontrol(bg1,'Position',[0 0 60 20],'Style', 'pushbutton','String', 'Hold off','Callback',@Togglehold);	
    
    uicontrol(bg,'Style','radiobutton','String','YYaxis left','Position',[0 0 80 20],'HandleVisibility','off');             
    uicontrol(bg,'Style','radiobutton','String','YYaxis right','Position',[80 0 80 20],'HandleVisibility','off');
      
    uicontrol(bg2,'Style','edit','String','X-axis','Position',[0 0 80 20],'HandleVisibility','off','Callback',@setXaxis);
    uicontrol(bg2,'Style','edit','String','Y-axis','Position',[80*1 0 80 20],'HandleVisibility','off','Callback',@setYaxis);
    uicontrol(bg2,'Style','pushbutton','String','Auto axis','Position',[80*2 0 80 20],'HandleVisibility','off','Callback',@bselection2);
    uicontrol(bg2,'Style','slider','String','ColorStyle','Position',[80*3 0 160 20],'HandleVisibility','off','Callback',@plotStyle);
    
    
    
        
    fields = fieldnames(S);
    for i= 1:size(fieldnames(S),1)
        uicontrol(bg1,'Position',[0 i*20 60 20],'Style', 'checkbox','String', fields(i),'Callback', @plot_new);
    end
    HoldButton = uicontrol(bg1,'Position',[0 size(fieldnames(S),1)*20 60 20],'Style', 'pushbutton','String', 'Clear Fig','Callback',@Togglehold);	
    tmpfields = string(fieldnames(S));
    
    plot(S.(string(fields(1))),'Tag',tmpfields(1),'DisplayName',tmpfields(1));
    grid on
end
end

function Togglehold(src,event)
    if src.String== "Hold off"
        hold on
        src.String = "Hold on";
            
    elseif src.String == "Hold on"
        hold off
        src.String = "Hold off";
    elseif src.String == "Clear Fig"
        
    end
      
end

function bselection(source,event)
    if event.NewValue.String == "YYaxis left"
       figure(1)
       yyaxis left
       disp("yyaxis Left")
    else 
        figure(1)
        yyaxis right
        disp("yyaxis Right")
    end
end

function setXaxis(source,event)

    xlim(str2num(source.String));
    disp(source.String)

end

function setYaxis(source,event)

    ylim(str2num(source.String));
    disp(source.String)

end

function bselection2(source,event)

    axis("auto")
    disp(source.String)

end

function plotStyle(source,event)
    %fig = gco()
    %get(fig)
    l = findobj('type','line');
    fig = l(1);
    fig.Color = "#"+dec2hex(round(source.Value*16777215));
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
                plot(S.(string(fields(i)))(2:end),'-','Tag',fields(i),'DisplayName',fields(i));
                grid on
            end
        end
    end
    legend
end

