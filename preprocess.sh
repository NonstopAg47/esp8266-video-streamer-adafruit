pip install --no-cache-dir -r requirements.txt
echo "Enter full path to input video "
read input_video
#strip away quotes, itill cause EOF error
input_video="${input_video//\"/}"

echo "Enter path to temp directory"
read temp_dir
temp_dir="${temp_dir//\"/}"

echo "Enter interval, affects fps; default = 31"
read interval
interval="${interval:-31}"

echo "Enter path to output dir"
read output_dir
output_dir="${output_dir//\"/}"

python makeup7.py --all="$input_video,$temp_dir,$interval,$output_dir"