from PIL import Image
import os

#image to utft conversion
#image is resized to 128x160 and converted to 16bit rgb hex format eg: 0xABCD  and a new folder is
#created at /file location/xampp/htdocs/testingimage/ and the hex content is split and saved

# Path to your image file  (change file name)
image_path = '/Users/hp/Desktop/esp/junk/night.jpg'

# Open an image
image = Image.open(image_path)

# Convert the image to RGB mode if it's not already
image = image.convert('RGB')

# Resize the image to 128x160
new_size = (128, 160)
image = image.resize(new_size)

# Get the dimensions of the resized image
width, height = image.size

# Create an empty list to store the hexadecimal values
hex_values = []

# Loop through each pixel and get its 16-bit hexadecimal value
for y in range(height):
    for x in range(width):
        pixel = image.getpixel((x, y))
        # Convert the RGB tuple to a 16-bit hexadecimal string (565 format)
        hex_value = "0x{:04X}".format(((pixel[0] & 0xF8) << 8) | ((pixel[1] & 0xFC) << 3) | (pixel[2] >> 3))
        hex_values.append(hex_value)

# Create folder
# Get the name of the image file without the extension
image_name = os.path.splitext(os.path.basename(image_path))[0]

# Specify the directory path where the folder should be created
output_dir = '/file location/xampp/htdocs/testingimage/'

# Create the folder if it doesn't exist
if not os.path.exists(os.path.join(output_dir, image_name)):
    os.makedirs(os.path.join(output_dir, image_name))

#save hex files as text in folder
# Initialize a counter for the output file names
output_counter = 1

# Initialize variables to keep track of the current content and space count
current_content = ""
space_count = 0

# Iterate through the hexadecimal values and split them into output files
for hex_value in hex_values:
    current_content += hex_value + ' '
    space_count += 1

    # If we've encountered 1792 spaces, write the current content to a new file
    if space_count == 1792:
        output_file = f'{output_dir}/{image_name}/output_{output_counter}.txt'
        with open(output_file, 'w') as outfile:
            outfile.write(current_content)
        
        # Reset current content and space count
        current_content = ""
        space_count = 0
        output_counter += 1

# If there is remaining content, write it to a new file
if current_content:
    output_file = f'{output_dir}/{image_name}/output_{output_counter}.txt'
    with open(output_file, 'w') as outfile:
        outfile.write(current_content)

print("Task complete. Split content into multiple files.")
