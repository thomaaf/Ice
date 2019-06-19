k=273.15
To_real = 47.12 +k;
To_new =  49.97 +k;
Ta_real = 23 +k;
Ta_new =  25 +k; 

e = (To_new^4 - Ta_new^4)/(To_real^4 - Ta_real^4)*0.33


new_em = round(65536*e -1);
prev_em  = hex2dec('555C');
prem_em2 = hex2dec('16CC');
adress = dec2hex(new_em)
adress1 = dec2hex(round(prev_em/new_em*prem_em2))