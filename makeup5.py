import os
import shutil

# Replace with the path to your folder containing images
folder_path = 'E:/F/iperov - used/DeepFaceLab/DeepFaceLab_DirectX12/workspace/data_dst/'

# Get a list of all files in the folder
files = os.listdir(folder_path)

# Sort the files to ensure they are in the correct order
files.sort()

# Specify the interval (keep every 35th image)   set it bw 30 and 40  if vid is fast reduce interval
interval = 31

# Initialize a counter to keep track of the current file
current_file = 1

# Loop through the files in the folder
for file_name in files:
    if current_file % interval != 1:
        # If it's not the 44th image (or the first image), delete it
        file_path = os.path.join(folder_path, file_name)
        os.remove(file_path)
    current_file += 1

print("Images have been processed. Every 44th image has been kept, and the rest have been deleted.")
