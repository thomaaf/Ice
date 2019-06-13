import serial
import json
ser = serial.Serial("COM5", 9600)
datalist = ["duty","temp_amb","temp_obj"]
data=[[],[],[]]
string ="{\"data\": \n \t [ \n" 
while True:
	try:

		cc=str(ser.readline())[2:][:-5]
		print(cc)
		line = cc.split(',')
		string = string + ("\t \t {")
		for n in range(len(datalist)):
			try:
				if n == len(datalist)-1:
					i = line.index(datalist[n])
					str(float(line[i+1].replace('"','')))
					string = string +("\"" + str(datalist[n])) + "\":" + str(float(line[i+1].replace('"',''))) 
				else:
					i = line.index(datalist[n])
					str(float(line[i+1].replace('"','')))
					string = string +("\"" + str(datalist[n])) + "\":" + str(float(line[i+1].replace('"',''))) + ","
			except ValueError:
				if n == len(datalist)-1:
					string = string +("\"" + str(datalist[n])) + "\":" + str(0) 
				else:
					string = string +("\"" + str(datalist[n])) + "\":" + str(0) + ","
	
		string = string +("},\n")
	except KeyboardInterrupt:
		string = string[0:-2] + "\n \t] \n}"
		f= open("datalist.json","w")
		f.write(string)
		f.close()
		break

#{"data": [{"Duty":0.34, "temp:": 0 },
#		  {"Duty":0.34, "temp:": 0}
#		  ]
#}