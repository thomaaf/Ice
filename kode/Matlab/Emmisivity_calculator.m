k=273.15;
To_real = 43.80  + k;
To_new =  32.0 + k;
Ta_real = 23.41 + k;
Ta_new =  23.41 + k; 

e = (To_new^4 - Ta_new^4)/(To_real^4 - Ta_real^4)
%e = (e1 + e2 + e3)/3
%%e = 0.04;
e = 0.3;
new_em = round(65536*e -1);
prev_em  = hex2dec('FFFF');
prem_em2 = hex2dec('773');
adress = dec2hex(new_em)
adress1 = dec2hex(round(prev_em/new_em*prem_em2))

%e1 = e
%e2 = e
