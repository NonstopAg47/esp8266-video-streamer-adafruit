from PIL import Image
import os
import struct  # for packing data as binary

# Function to convert an image to binary RGB565 and save it
def convert_and_save_image_binary(image_path, output_dir):
    # Open an image
    image = Image.open(image_path)

    # Convert the image to RGB mode if it's not already
    image = image.convert('RGB')

    # Resize the image to 128x160
    new_size = (128, 160)
    image = image.resize(new_size)

    # Get the dimensions of the resized image
    width, height = image.size

    # Create a list to store binary pixel data
    binary_data = bytearray()

    # Loop through each pixel and convert to 16-bit RGB565
    for y in range(height):
        for x in range(width):
            r, g, b = image.getpixel((x, y))
            rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
            # Pack as 2-byte binary (big endian)
            binary_data += struct.pack('>H', rgb565)

    # Get the image file name without extension
    image_name = os.path.splitext(os.path.basename(image_path))[0]

    # Create the folder if it doesn't exist
    save_dir = os.path.join(output_dir, image_name)
    os.makedirs(save_dir, exist_ok=True)

    # Split binary data into chunks of 1792 pixels (each pixel = 2 bytes)
    chunk_size = 1792 * 2  # 1792 pixels * 2 bytes per pixel
    for i in range(0, len(binary_data), chunk_size):
        chunk = binary_data[i:i + chunk_size]
        output_file = os.path.join(save_dir, f'output_{i // chunk_size + 1}.bin')
        with open(output_file, 'wb') as f:
            f.write(chunk)

    print(f"Processed and split '{image_name}' into {len(binary_data) // chunk_size + (1 if len(binary_data) % chunk_size else 0)} binary files.")

# Path to the folder containing images
input_folder = 'C:/Users/hp/Desktop/esp/junk/death/'

# Path to the output directory
output_dir = 'E:/F/file location/xampp/htdocs/testingimage/'

# Iterate through all image files in the input folder and process each one
for filename in os.listdir(input_folder):
    if filename.lower().endswith('.jpg'):
        image_path = os.path.join(input_folder, filename)
        convert_and_save_image_binary(image_path, output_dir)
