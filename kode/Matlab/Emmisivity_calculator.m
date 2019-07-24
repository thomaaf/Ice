k=273.15;
To_real = 37.37  + k;
To_new =  29.9 + k;
Ta_real = 26.67 + k;
Ta_new =  26.67 + k; 

e = (To_new^4 - Ta_new^4)/(To_real^4 - Ta_real^4)
%e = (e1 + e2 + e3)/3
%%e = 0.04;
%First: 0.2809;
e = 0.26;
new_em = round(65536*e -1);
prev_em  = hex2dec('451E');
prem_em2 = hex2dec('0A59');
adress24 = dec2hex(new_em)
adress2F = dec2hex(round(prev_em/new_em*prem_em2))

%e1 = e
%e2 = e
