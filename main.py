# Created by Yash Hingu
# https://github.com/Yash-Hingu/Stepper-project

import time
import serial
import pandas as pd
import json
import keyboard
from calculate_angle import angle_to_steps, dist_to_steps
# Replace with the COM port of your controller (e.g., 'COM1' on Windows or '/dev/ttyUSB0' on Linux)
for t in range(1,11):
  try:
    ser = serial.Serial('COM5', 115200, timeout=1)
    print("Connected to " + ser.port)
    break
  except Exception as e:
          time.sleep(2)
          print(f"Error: {str(e)}")
          print(f"Retrying...  {t}" )
   

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
              "motor1": angle_to_steps(int(row[0])),
              "motor2": angle_to_steps(int(row[1])),
              "motor3": angle_to_steps(int(row[2]))
            }
            data_list.append(data_dict)

        return data_list

    except Exception as e:
        print(f"Error: {str(e)}")
        return None

alldone = None
data_list = read_excel_to_dict(excel_file_path)
global index

read_excel_to_dict(excel_file_path)
print(str(data_list[0]))


data_list =  read_excel_to_dict(excel_file_path)

while True:
  data_list = read_excel_to_dict(excel_file_path)
  ser.write((str(data_list[0]) + '\n').encode())
  time.sleep(1)

  if keyboard.is_pressed("q"):
        break 


ser.close()