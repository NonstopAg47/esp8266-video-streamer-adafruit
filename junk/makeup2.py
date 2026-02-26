# Specify the input file path (dont change)
input_file = 'E:/F/file location/xampp/htdocs/testingimage/hex_values.txt'

# Initialize a counter for the output file names
output_counter = 1

# Open the input file
with open(input_file, 'r') as infile:
    # Initialize variables to keep track of the current content and space count
    current_content = ""
    space_count = 0

    # Iterate through each character in the input file
    for char in infile.read():
        # Append the character to the current content
        current_content += char

        # If the character is a space, increment the space count
        if char == ' ':
            space_count += 1

        # If we've encountered 1792 spaces, write the current content to a new file (change folder name)
        if space_count == 1792:
            output_file = f'E:/F/file location/xampp/htdocs/testingimage/tobey/output_{output_counter}.txt'
            with open(output_file, 'w') as outfile:
                outfile.write(current_content)
            
            # Reset current content and space count
            current_content = ""
            space_count = 0
            output_counter += 1

    # If there is remaining content, write it to a new file (change folder name)
    if current_content:
        output_file = f'E:/F/file location/xampp/htdocs/testingimage/tobey/output_{output_counter}.txt'
        with open(output_file, 'w') as outfile:
            outfile.write(current_content)

# Print a message to indicate the task is complete
print("Task complete. Split content into multiple files.")
