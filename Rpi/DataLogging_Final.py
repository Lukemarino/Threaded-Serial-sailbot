import serial
import logging

port = "/dev/ttyACM0" # if ACM0 doesnt work, use ACM1. This is unreliable and we
                        # will need to find a universal port or other solution
ser = serial.Serial(port,115200,timeout=0) 
package = []
logging.basicConfig(filename="/home/pi/BootupTest8.txt",level=logging.INFO) #function that writes to declared file line by line
i=0

while True:
    
    try:
        data = ser.read(1024)
        i = i+1
    except:
        x=1

    if len(data) > 0: #if there is any data coming in
        if i < 50: #gets rid of jibberish we are receiving at the inital startup
            dataTr = 0.0 #^
            data = 0.0 #^^
            

        datastr = str(data) #typecast data to string
        end = len(datastr) #grab end index
        dataTr = datastr[2:end-1] #truncate data on either end down to just the number
            
        if dataTr == 'q': #our delimiter
            if len(package) == 6: #check to make sure package size matches what we plan to send to file
                logging.info(' Time: ' + package[0] + ':' + package[1] + ':' + package[2] + ' Encoder: ' + package[3] + ' Lat: ' + package[4] + ' Long: ' + package[5]+ ' Heading: ' + '\n')
                #^ use logger function to write data to file
                package.clear() #empty package so it can receive new data
            else:
                package.clear() #if the package size doesnt match what we expect, drop it
        else: #if the data is not the delimiter it must be the content we want to record
            
            package.append(dataTr) #append it to the package so it will write to the file once it hits the delimiter
        
        print(dataTr) #shows up in terminal so you can keep track of what is being sent
        


