#include <stdio.h>
#include <time.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "filter.hpp"
#include <cstring>


using namespace cv; 

int main(int argc, char *argv[]) 
{
	VideoCapture *capdev;
	// open the video device
    capdev = new VideoCapture(0);
    if( !capdev->isOpened() ) 
    {
    	printf("Unable to open video device\n");
        return(-1);
    }

    // get some properties of the image
    Size refS( (int) capdev->get(CAP_PROP_FRAME_WIDTH ),(int) capdev->get(CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    namedWindow("Video", 1); // identifies a window
        
	Mat frame, dst, sx, sy;
	int function = 0;
    
    for (;;) 
    {
        //get a new frame from the camera and treat it as a stream
        *capdev >> frame;
        switch(function)
        {
        	case 1:
        	convertScaleAbs( frame, frame );
        	break;
        	case 2:
        	cvtColor(frame, dst, COLOR_RGBA2GRAY, 0);
            frame = dst;
            break;
            case 3:
            greyscale(frame, dst);
            frame = dst;
            break;
            case 4:
            blur5x5(frame, dst);
            frame = dst;
            break;
            case 5:
            sobelX3x3(frame, dst);
            convertScaleAbs( dst, frame );
            break;
            case 6:
            sobelY3x3(frame, dst);
            convertScaleAbs( dst, frame );
            break;
            case 7:
            sobelX3x3(frame, sx);
            sobelY3x3(frame, sy);
            magnitude(sx, sy, dst);
            convertScaleAbs( dst, frame );
            break;
            case 8:
            blurQuantize(frame, dst, 15);
            convertScaleAbs( dst, frame );
            break;
            case 9:
            cartoon(frame, dst, 15, 10);
            convertScaleAbs( dst, frame );
            break;
            case 10:
            negative(frame, dst);
            convertScaleAbs( dst, frame );
            break; 
            case 11:
            mirror(frame, dst);
            convertScaleAbs( dst, frame );
            break;
            case 12:
            upsidedown(frame, dst);
            convertScaleAbs( dst, frame );
            break; 
            case 13:
            addsaltandpepper(frame, dst);
            convertScaleAbs( dst, frame );
            break; 
            case 14:
            medianfilter(frame, dst);
            convertScaleAbs( dst, frame );
            break; 
            case 15:
            float threshold = 120;
            detectblur(frame, threshold);
            break; 
        }
        char keypress = waitKey(5);
        if(keypress == 'q' || frame.empty()) 
        {
            break;
        } 
        else if (keypress == 's') 
        {
            imwrite("savedimage.jpg", frame);
        }
        else if (keypress == 'g') 
        {
            function = 2;
        } 
        else if (keypress == 'h')
        {
            function = 3;
        } 
        else if (keypress == 'b') 
        {
            function = 4;
        } 
        else if (keypress == 'x') 
        {
            function = 5;
        } 
        else if (keypress == 'y') 
        {
            function = 6;
        }
        else if (keypress == 'm') 
        {
            function = 7;
        }
        else if (keypress == 'l') 
        {
            function = 8;
        }
        else if (keypress == 'c') 
        {
            function = 9;
        }
        else if (keypress == 'n') 
        {
            function = 10;
        }
        else if (keypress == 'r') 
        {
            function = 11;
        }
        else if (keypress == 'u') 
        {
            function = 12;
        }
        else if (keypress == 'p') 
        {
            function = 13;
        }
        else if (keypress == 'f') 
        {
            function = 14;
        }
        else if (keypress == 'd') 
        {
            function = 15;
        }
        else if (keypress == 27) 
        {
            function = 1;
        }
        imshow("Video", frame);  
    }
    delete capdev;
    return 0;
}
