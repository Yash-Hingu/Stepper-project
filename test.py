import pandas as pd
from calculate_angle import angle_to_steps, dist_to_steps


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
              "motor3": angle_to_steps(float(row[2]))
            }
            data_list.append(data_dict)

        return data_list

    except Exception as e:
        print(f"Error: {str(e)}")
        return None

print(read_excel_to_dict("Sample.csv")[0])