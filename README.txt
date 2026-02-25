PREPROCESSING

download image
if single image run makeup3.py from C:\Users\hp\Desktop\oppenheimer\ (make sure to update file and folder names)
if multiple image 
	create a folder at C:/Users/hp/Desktop/esp/junk/
	convert vid to jpg format using iperov
	run makeup7.py instead of using iperov, code modified from iperov 
	run makeup5.py from C:\Users\hp\Desktop\oppenheimer\ (make sure to update file and folder names) it is for adjusting frame rate
	move contents of /data_dst/ to C:/Users/hp/Desktop/esp/junk/death/
	run rename_files.bat
	run rotate.py to rotate vid
	run makeup4.py from C:\Users\hp\Desktop\oppenheimer\ (make sure to update file and folder names)
	run makeup6.py instead of makeup4.py for .bin files

MAIN

run xampp
find address (check ipconfig) eg: 192.168.1.3
run C:\Users\hp\Desktop\oppenheimer\server_to_array_to_display_split_files_2    (modify address and file name in line 37)

NOTE:

i made it 12 files coz more than 1808 pixels being assigned to an array crashes. i rounded and use 1792 pixels (128x14)and split 1 image to 12 sections

makeup3.py was made from img2utft.py so i didnt delete it

maybe combining 12 files to 1 and sectioning it will reduce space



PREPROCESSING (old method)

download image
resize img to 128x160
go to site and get pixel values (http://www.rinkydinkelectronics.com/t_imageconverter565.php)       if i write my own program i should modify makeup1.py and makeup2.py
the output will be a .c file, make it .txt and store at E:\F\file location\xampp\htdocs\testingimage\
remove first 16 lines from doc.txt and }; from the end
create a folder eg:doc (E:\F\file location\xampp\htdocs\testingimage\doc\)
run makeup1.py and makeup2.py from C:\Users\hp\Desktop\oppenheimer\ (make sure to update file and folder names)




