    %fclose(instrfind())
    %delete(instrfind())
    s=serial('COM4','BaudRate',38400);
    s.Terminator = 'LF';
    s
    fopen(s);
    
    datalist=[];
    RTD=[];
    IR_amb=[];
    IR_OBJTOT=[];
    IR_OBJ1=[];
    IR_OBJ2=[];    
    COUNTER = 0;
   while 1
        COUNTER = COUNTER +1
        Dataline=fgetl(s); %reads "Ready" 
        a = string(Dataline(1:end-1))
        b= strsplit(a,',');
        if size(b,2)>1
            for j=1:2:size(b,2)
                if b(j) == 'RTD'
                    RTD=[RTD, str2num(b(j+1))];
                end
                if b(j) == 'IR_amb'
                    IR_amb=[IR_amb, str2num(b(j+1))];
                end
                if b(j) == 'IR_OBJTOT'
                    IR_OBJTOT=[IR_OBJTOT, str2num(b(j+1))];
                end        
                if b(j) == 'IR_OBJ2'
                    IR_OBJ2=[IR_OBJ2, str2num(b(j+1))];
                end        
                if b(j) == 'IR_OBJ1'
                    IR_OBJ1=[IR_OBJ1, str2num(b(j+1))];
                end                        
            end
        end
    end
    fclose(s);
    delete(s);

