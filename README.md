#Real Time Filtering

OS - Linux (Debian - Ubuntu 22.04)
IDE - None (CLE using terminal)


Build instructions: 
1. Use CMakeLists.txt to generate make file ten build target and then execute target (Command line execution using terminal in Linux (debian))
2. For first task for image display, the second argument should be the path (absolute or local) of the image that is target name followed by path
3. If any of the filters don't replicate, please execute once again and apply only the filter in question (Please refrain from
   overloading filters)
4. After applying standard median filter to remove salt and pepper noise, the display window becomes choppy. Please execute the program once again.


Applicable Keypresses - 
1.  's' - save an image
2.  'g' - greyscale
3.  'h' - alternate greyscale
4.  'b' - gaussian blur
5.  'x' - sobel x
6.  'y' - sobel y
7.  'm' - gradient magnitude
8.  'l' - blurred and quantized
9.  'c' - cartoon
10. 'n' - negative 
11. 'r' - mirror
12. 'u' - rotate 180 degrees
13. 'p' - add salt and pepper
14. 'f' - remove salt and pepper median filter
15. 'd' - detect blur
