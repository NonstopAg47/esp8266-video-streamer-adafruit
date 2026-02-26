# Specify the input and output file paths
input_file = '/file location/xampp/htdocs/testingimage/iron.txt'
output_file = '/file location/xampp/htdocs/testingimage/ironoutput.txt'

# Open the input and output files
with open(input_file, 'r') as infile, open(output_file, 'w') as outfile:
    # Initialize an empty string to store the trimmed and comma-removed content
    cleaned_content = ""
    
    # Iterate through each line in the input file
    for line in infile:
        # Trim the line to the first 128 characters
        trimmed_line = line[:128]
        # Remove all commas from the trimmed line
        cleaned_line = trimmed_line.replace(",", "")
        # Append the cleaned line to the cleaned content
        cleaned_content += cleaned_line
    
    # Write the cleaned content to the output file
    outfile.write(cleaned_content)

# Print a message to indicate the task is complete
print("Task complete. Trimmed lines to 128 characters and removed commas.")
