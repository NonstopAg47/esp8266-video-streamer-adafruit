from PIL import Image
import os

#image is resized to 128x160 and converted to 16bit rgb hex format eg: 0xABCD and saved and a new folder is
#also created at /file location/xampp/htdocs/testingimage/

# Path to your image file  (change this)
image_path = '/Desktop/esp/junk/test.jpg'

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

# Close the image when you're done
image.close()

# Combine all hexadecimal values into a single string with spaces
hex_string = ' '.join(hex_values)

# Specify the name of the output text file
output_file = '/file location/xampp/htdocs/testingimage/hex_values.txt'

# Write the hexadecimal values to the text file
with open(output_file, 'w') as file:
    file.write(hex_string)

print(f"Hexadecimal values saved to {output_file}")

#create folder
# Get the name of the image file without the extension
image_name = os.path.splitext(os.path.basename(image_path))[0]

# Specify the directory path where the folder should be created
output_dir = '/file location/xampp/htdocs/testingimage/'

# Create the folder if it doesn't exist
if not os.path.exists(os.path.join(output_dir, image_name)):
    os.makedirs(os.path.join(output_dir, image_name))
