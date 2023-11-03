# # Importing Libraries 
# import serial 
# import time 
# arduino = serial.Serial(port='COM5', baudrate=115200, timeout=.1) 
# def write_read(x): 
#     arduino.write(bytes(x, 'utf-8')) 
#     time.sleep(0.05)
#     data = arduino.readline()
#     return data 
# while True: 
#     num = input("Enter a number: ") # Taking input from user 
#     value = write_read(num) 
#     print(value) # printing the value 


#############################################################
# import serial
# import pandas as pd
# import json
# # Replace with the COM port of your ESP32 (e.g., 'COM1' on Windows or '/dev/ttyUSB0' on Linux)
# ser = serial.Serial('COM5', 115200, timeout=1)

# while True:
#     data_to_send = input("Enter data to send to ESP32: ")
    
#     # Send data to the ESP32
#     ser.write((data_to_send + '\n').encode())
    
#     # Read and print the ESP32's response (optional)
#     response = ser.readline().decode().strip()
#     print("Received from ESP32:", response)


##########################

import time
import serial
import pandas as pd
import json

from calculate_angle import angle_to_steps, dist_to_steps
# Replace with the COM port of your ESP32 (e.g., 'COM1' on Windows or '/dev/ttyUSB0' on Linux)
ser = serial.Serial('COM5', 115200, timeout=.1)

excel_file_path = "Sample.csv"
def read_excel_to_dict(file_path):
    global index
    try:
        file_extension = file_path.split('.')[-1].lower()
        
        # Read Excel file using pandas
        if file_extension == "xls" or file_extension == "xlsx":
          xls = pd.ExcelFile(file_path)
          df = xls.parse(0)
        elif file_extension == "csv":
          df = pd.read_csv(file_path)
        else:
          raise Exception("Invalid file type: "+file_extension)
        
        # Create an empty list to store data as dictionaries
        data_list = []

        # Iterate through the rows of the DataFrame and store data in the list
        for index, row in df.iterrows():
            data_dict = {
              "row": index,
              "motor1": angle_to_steps(float(row[0])),
              "motor2": angle_to_steps(float(row[1])),
              "motor3": dist_to_steps(float(row[2]))
            }
            data_list.append(data_dict)

        return data_list

    except Exception as e:
        print(f"Error: {str(e)}")
        return None

alldone = None
data_list = read_excel_to_dict(excel_file_path)
global index
print(index)
connection2esp = False
while not connection2esp:
    ser.write(("connection" + '\n').encode())
    response = ser.readall().decode().strip()
    if response == "0":
       connection2esp = True
       print("connected!")
       break
    print("connecting")
    time.sleep(2)


data_list =  read_excel_to_dict(excel_file_path)



count_row = 0
prvs_res = None
while alldone != 0 and connection2esp:
    # data_to_send = input("Enter data to send to ESP32: ")
    

     
    # Send data to the ESP32
    if prvs_res != response:
     ser.write((str(data_list[count_row]) + '\n').encode())
    
    # Read and print the ESP32's response (optional)
    # response = ser.readline().decode().strip()
    response = ser.readall().decode().strip()
    if (response.isnumeric()):
       count_row = int(response)
       prvs_res = count_row
       count_row = count_row + 1
       alldone = count_row - 1



    if response == "0":
    #    alldone = 2
       print("it's zero")
    # print("Received from ESP32:", response)
    # time.sleep(0.1)
    # print(ser.readall().decode().strip()," <---")
ser.close()