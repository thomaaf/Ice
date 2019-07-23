function [S,readindicator] = SerialRead(indicator,COM,BAUD)
indicator = strsplit(indicator,',');
type  = [];
readindicator = 1;
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
clf(1);
hold on;
ButtonHandle = uicontrol('Position',[0 0 160 20], 'Style', 'PushButton','String',     'Stop read, discard and plot','Callback', 'delete(gcbo)');
ButtonHandle2= uicontrol('Position',[160 0 160 20], 'Style', 'PushButton','String',   'Stop read and discard','Callback', 'delete(gcbo)');
ButtonHandle3= uicontrol('Position',[160*2 0 160 20], 'Style', 'PushButton','String', 'Stop read and save','Callback', 'delete(gcbo)');
s=serial(COM,'BaudRate',BAUD);
s.Terminator = 'LF';
fopen(s);
theta = zeros(1,4);
Q   = zeros(1,4);
R     = zeros(1,16);
phip = zeros(1,4);
W = zeros(1,2);
ypSim = zeros(1,2);
while 1
    %plot(size(S.duty,2),S.duty(end),'.');
	if ~ishandle(ButtonHandle)
		disp('Serial read stopped, discarding, plotting');
        S.theta = theta;
		break;
    elseif ~ishandle(ButtonHandle2)
		disp('Serial read stopped, skipping plotting...');
        readindicator = 0;
		break;   
    elseif ~ishandle(ButtonHandle3)
		disp('Serial read stopped, saving...');
        S.theta = theta;
        load("./logs/lognr.mat","lognr");
        save("./logs/log"+lognr+".mat","S");
        lognr=lognr+1;
        save("./logs/lognr.mat","lognr");
		break;           
	end	  
	pause(0.0000000001); 
	Dataline=fgetl(s); 
	a = string(Dataline(1:end-1)); 
    disp(a)
	b= strsplit(a,',');
	if size(b,2)>1
		for j=1:2:size(b,2)
			
			for i=1:size(indicator,2)
				if b(j) == indicator(i)
                    if string(indicator(i)) == "theta"
                       theta = [theta;str2num(b(j+1))' ];
                    elseif string(indicator(i)) == "R"
                       R = [R; str2num(b(j+1))'];
                    elseif string(indicator(i)) == "phip"
                       phip = [phip; str2num(b(j+1))'];      
                    elseif string(indicator(i)) == "w"
                       W = [W; str2num(b(j+1))']; 
                    elseif string(indicator(i)) == "ypSim"
                       ypSim = [ypSim; str2num(b(j+1))'];                             
                    elseif string(indicator(i)) == "t"
                       S.(string(indicator(i))) = [S.(string(indicator(i))) str2num(b(j+1))];
                       S.theta = theta;
                       S.R = R;
                       S.Q = Q;
                       S.W = W;
                       S.ypSim = ypSim;
                       S.phip = phip;
                       if str2num(b(j+1))' >1000000
                           return;
                       end
                       
                    else
                        S.(string(indicator(i))) = [S.(string(indicator(i))) str2num(b(j+1))];
                    end
				end
			end					
		end
	end
end


fclose(s);
delete(s);
clear j i a b ButtonHandle Dataline s T
