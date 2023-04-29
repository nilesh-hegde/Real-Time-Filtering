/*
Nilesh Hegde
23 January 2023
Project 1 - Real time filtering
*/

#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;
int main(int argc, char** argv )
{
	//Check if path to image is passed as an argument
    if ( argc != 2 )
    {
        printf("Need Image path as argument\n");
        return -1;
    }
    Mat image;
    image = imread( argv[1], IMREAD_COLOR );
    //Check if image is empty
    if ( image.empty() )
    {
        printf("Image could not be read\n");
        return -1;
    }
    //Display Image in window
    imshow("Display Image", image);
    char keypress = waitKey(0);
    //Check if q is pressed to exit window
    while(keypress != 'q')
   	{
    	keypress = waitKey(0);
    }
    return 0;
}
