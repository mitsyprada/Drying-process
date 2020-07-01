import serial
import time
import csv
import numpy as np
import gspread
import datetime
from oauth2client.service_account import ServiceAccountCredentials
scope=['https://spreadsheets.google.com/feeds','https://www.googleapis.com/auth/drive']
credentials=ServiceAccountCredentials.from_json_keyfile_name('./erska-281810-f8b478c94bb8.json')
gc=gspread.authorize(credentials)
wks=gc.open('Trial4_updatedtimestamps').sheet1



ser = serial.Serial('/dev/ttyACM1')
ser.flushInput()
# f1-1,t3-2, h4-3, af-4, t1-5, t2-6, h1-7, f2-8, t4-9 , wm-10,  t5-11,  h3-12,  h2-13 
#
readings=list(-1.0*np.ones(15))
flag=0
while True:
    ser_bytes = ser.readline()
    decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode("utf-8")
    i=0
    for cn in range(len(decoded_bytes)):
        i=cn
        if decoded_bytes[i]==':':
            break
        
    if decoded_bytes[0:i]=='f1':
        readings[1]=decoded_bytes[i+1:len(decoded_bytes)]
    if decoded_bytes[0:i]=='f2':
        readings[2]=decoded_bytes[i+1:len(decoded_bytes)]
    if decoded_bytes[0:i]=='t1':
        readings[3]=decoded_bytes[i+1:len(decoded_bytes)]
    if decoded_bytes[0:i]=='h1':
        readings[4]=decoded_bytes[i+1:len(decoded_bytes)]
    if decoded_bytes[0:i]=='t2':
        readings[5]=decoded_bytes[i+1:len(decoded_bytes)]
    if decoded_bytes[0:i]=='h2':
        readings[6]=decoded_bytes[i+1:len(decoded_bytes)]
    if decoded_bytes[0:i]=='t3':
        readings[7]=decoded_bytes[i+1:len(decoded_bytes)]
    if decoded_bytes[0:i]=='h4':
        readings[8]=decoded_bytes[i+1:len(decoded_bytes)]
    if decoded_bytes[0:i]=='af':
        readings[9]=decoded_bytes[i+1:len(decoded_bytes)]
    if decoded_bytes[0:i]=='t4':
        readings[10]=decoded_bytes[i+1:len(decoded_bytes)]
    if decoded_bytes[0:i]=='wm':
        readings[11]=decoded_bytes[i+1:len(decoded_bytes)]
    if decoded_bytes[0:i]=='t5':
        readings[12]=decoded_bytes[i+1:len(decoded_bytes)]
    if decoded_bytes[0:i]=='h3':
        readings[13]=decoded_bytes[i+1:len(decoded_bytes)]  
    for j in readings[1:len(readings)-1]:
        if float(j)==-1.0:
            flag=0
            break
        else:
            flag=1
    if flag==1:
        readings[0]=str(1000*time.time())
        readings[14]=str(datetime.datetime.fromtimestamp(float(readings[0])/1000).isoformat())
        readings[0]=str(int(float(readings[0])))        
        print(readings)
        with open("Trial_4_01-07-2020.csv","a") as f:
            writer = csv.writer(f,delimiter=",")
            writer.writerow(readings)
        wks.append_row(readings)
        readings=list(-1.0*np.ones(15))
    flag=0
    time.sleep(0.1)
        

