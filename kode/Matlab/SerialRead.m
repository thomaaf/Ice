function [S,indicator] = SerialRead(indicator,COM,BAUD)
indicator = strsplit(indicator,',');
type  = [];
for k= 1:size(indicator,2)
   type =[type, "double"];
end
T = table('Size',size(indicator),'VariableTypes',type,'VariableNames',indicator);
S = table2struct(T,'ToScalar',true);

if size(instrfind(),1)>0
	fclose(instrfind());
	delete(instrfind());	   
end

figure(1)
ButtonHandle = uicontrol('Style', 'PushButton','String', 'Stop loop','Callback', 'delete(gcbf)');
					 
s=serial(COM,'BaudRate',BAUD);
s.Terminator = 'LF';
fopen(s);

while 1
	if ~ishandle(ButtonHandle)
		disp('Serial read stopped');
		break;
	end	  
	pause(0.0000000001); 
	Dataline=fgetl(s); 
	a = string(Dataline(1:end-1)); 
	b= strsplit(a,',');
	if size(b,2)>1
		for j=1:2:size(b,2)
			disp(a)
			for i=1:size(indicator,2)
				if b(j) == indicator(i)
					S.(string(indicator(i))) = [S.(string(indicator(i))) str2double(b(j+1))];
				end
			end					
		end
	end
end

fclose(s);
delete(s);
clear j i a b ButtonHandle Dataline s T


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
        if (src.String == fields(i))&&(src.Value == 0)
            l = findobj('type','line');
            for j = 1:size(l)
                if l(j).Tag == fields(i)
                   delete(l(j))
                end
            end
        elseif (src.String == fields(i))&&(src.Value == 1)
            a(i) = plot(S.(string(fields(i))),'Tag',fields(i),'DisplayName',fields(i));
        end
    end
    legend
end