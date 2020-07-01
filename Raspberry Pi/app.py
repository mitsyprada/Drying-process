from flask import Flask, render_template, request# t3h4, t2h2
import json
import csv
import datetime
filename='Trial_4_01-07-2020.csv'
app=Flask(__name__)

@app.route('/', methods=['GET'])
def index():
    return(render_template('index.html'))

@app.route('/', methods=['POST'])
def result():
    count=request.form['count']
    dates,timestamps=getDate(int(count))
    T_5=getTemp_t5(timestamps)
    H_3=getHum_h3(timestamps)
    T_3=getTemp_t3(timestamps)
    H_4=getHum_h4(timestamps)
    T_2=getTemp_t2(timestamps)
    H_2=getHum_h2(timestamps)    
    return(render_template('index.html',dates=dates, t5=T_5,h3=H_3,t3=T_3,h4=H_4,t2=T_2,h2=H_2))

def getDate(count):
    with open(filename, "r") as f:
        reader = csv.reader(f)
        dateList=[]
        ts=[]
        for header in reader:
            readable = datetime.datetime.fromtimestamp(float(header[0])/1000).isoformat()
            dateList.append(readable)
            ts.append(float(header[0]))
        ts.sort()
        ts.reverse()
        dateList.sort()
        dateList.reverse()
        #datelist.reverse()
        return list(reversed(dateList[0:count])),list(reversed(ts[0:count]))
def getTemp_t5(timestamps):
    with open(filename, "r") as f:
        reader = csv.reader(f)
        tempList=[]
        for i in timestamps:
            for header in reader:
                if float(header[0])==i:
                    tempList.append(header[12])
                    break
        return list(tempList)
def getHum_h3(timestamps):
    with open(filename, "r") as f:
        reader = csv.reader(f)
        tempList=[]
        for i in timestamps:
            for header in reader:
                if float(header[0])==i:
                    tempList.append(header[13])
                    break
        return list(tempList)
def getTemp_t3(timestamps):
    with open(filename, "r") as f:
        reader = csv.reader(f)
        tempList=[]
        for i in timestamps:
            for header in reader:
                if float(header[0])==i:
                    tempList.append(header[7])
                    break
        return list(tempList)
def getHum_h4(timestamps):
    with open(filename, "r") as f:
        reader = csv.reader(f)
        tempList=[]
        for i in timestamps:
            for header in reader:
                if float(header[0])==i:
                    tempList.append(header[8])
                    break
        return list(tempList)

def getTemp_t2(timestamps):
    with open(filename, "r") as f:
        reader = csv.reader(f)
        tempList=[]
        for i in timestamps:
            for header in reader:
                if float(header[0])==i:
                    tempList.append(header[5])
                    break
        return list(tempList)
def getHum_h2(timestamps):
    with open(filename, "r") as f:
        reader = csv.reader(f)
        tempList=[]
        for i in timestamps:
            for header in reader:
                if float(header[0])==i:
                    tempList.append(header[6])
                    break
        return list(tempList)

    
if __name__ == '__main__':
    app.run(debug=True, port=5003)
