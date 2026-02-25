from PIL import Image
import os

# Input folder path
input_folder = 'C:/Users/hp/Desktop/esp/junk/death/'

# List all files in the input folder
image_files = [f for f in os.listdir(input_folder) if f.endswith(('.jpg', '.jpeg', '.png', '.gif'))]

for filename in image_files:
    input_path = os.path.join(input_folder, filename)

    try:
        # Open the image
        img = Image.open(input_path)

        # Rotate the image by 90 degrees to the right
        img = img.transpose(Image.ROTATE_270)

        # Save the rotated image, overwriting the original file
        img.save(input_path)

        print(f"Rotated and replaced {filename}")
    except Exception as e:
        print(f"Error processing {filename}: {e}")
