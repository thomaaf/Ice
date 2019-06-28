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
		disp('Loop stopped by user');
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
close(figure(1))
fclose(s);
delete(s);
clear j i a b ButtonHandle Dataline s T
end

