/*
Nilesh Hegde
23 January 2023
Project 1 - Real time filtering
*/

#include "filter.hpp"
#include <stdlib.h> 
#include <bits/stdc++.h>
#include <cstring>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int greyscale( Mat &src, Mat &dst)
{
    dst.create(src.size(), src.type());
    for (int i = 0; i < src.rows; i++) 
    {
        for (int j = 0; j < src.cols; j++) 
        {
        	//Compute the average of all channels
        	int grayvalue = (src.at<Vec3b>(i,j)[0] + src.at<Vec3b>(i,j)[1] + src.at<Vec3b>(i,j)[2])/3; 
            dst.at<Vec3b>(i,j)[0] = grayvalue;
            dst.at<Vec3b>(i,j)[1] = grayvalue;
            dst.at<Vec3b>(i,j)[2] = grayvalue;
        }
    }
    return 0;
}

int blur5x5( Mat &src, Mat &dst) 
{
    Mat temp;
    dst.create(src.size(), src.type());
    //Create a temporary image matrix to hold values after applying gauss operator in horizontal direction
    temp.create(src.size(), src.type());
    //Horizontal direction convolution
    for (int i = 2; i < src.rows-2; i++) 
    {
        for (int j = 2; j < src.cols-2; j++)
        {
            for (int c = 0; c < src.channels(); c++) 
            {
            	//Convolve using 1x5 gauss operator on source image
                temp.at<Vec3b>(i,j)[c] = (src.at<Vec3b>(i-2, j-2)[c] + src.at<Vec3b>(i-2, j-1)[c]*2 + src.at<Vec3b>(i-2, j)[c]*4 + src.at<Vec3b>(i-2, j+1)[c]*2 + src.at<Vec3b>(i-2, j+2)[c])/10;
            }
        }
    }
    //Vertical direction convolution
    for (int i = 2; i < temp.rows-2; i++) 
    {
        for (int j = 2; j < temp.cols-2; j++)
        {
            for (int c = 0; c < temp.channels(); c++) 
            {
            	//Convolve using 5x1 gauss operator on temporary image
                dst.at<Vec3b>(i,j)[c] = (temp.at<Vec3b>(i-2, j-2)[c] + temp.at<Vec3b>(i-1, j-2)[c]*2 + temp.at<Vec3b>(i, j-2)[c]*4 + temp.at<Vec3b>(i+1, j-2)[c]*2 + temp.at<Vec3b>(i+2, j-2)[c])/10;
            }
        }
    }
    return 0;
}

int sobelX3x3( Mat &src, Mat &dst )
{
    Mat temp;
    dst.create(src.size(), CV_16SC3);
    //Create a temporary image matrix to hold values after applying gauss operator in horizontal direction
    temp.create(src.size(), CV_16SC3);
    //Horizontal direction convolution
    for (int i = 1; i < src.rows-1; i++) 
    {
        for (int j = 1; j < src.cols-1; j++)
        {
            for (int c = 0; c < src.channels(); c++) 
            {
            	//Convolve using 1x3 sobel operator on source image
                temp.at<Vec3s>(i,j)[c] = src.at<Vec3b>(i-1, j-1)[c]*(-1) + src.at<Vec3b>(i-1, j)[c]*0 + src.at<Vec3b>(i-1, j+1)[c];
            }
        }
    }
    //Vertical direction convolution
    for (int i = 1; i < temp.rows-1; i++) 
    {
        for (int j = 1; j < temp.cols-1; j++)
        {
            for (int c = 0; c < temp.channels(); c++) 
            {
            	//Convolve using 3x1 sobel operator on temporary image
                dst.at<Vec3s>(i,j)[c] = (temp.at<Vec3s>(i-1, j-1)[c] + temp.at<Vec3s>(i, j-1)[c]*2 + temp.at<Vec3s>(i+1, j-1)[c])/4 ;
            }
        }
    }
    return 0;
}
int sobelY3x3( Mat &src, Mat &dst )
{
    Mat temp;
    dst.create(src.size(), CV_16SC3);
    temp.create(src.size(), CV_16SC3);
    //Horizontal direction convolution
    for (int i = 1; i < src.rows-1; i++) 
    {
        for (int j = 1; j < src.cols-1; j++)
        {
            for (int c = 0; c < src.channels(); c++) 
            {
            	//Convolve using 1x3 sobel operator on source image
                temp.at<Vec3s>(i,j)[c] = (src.at<Vec3b>(i-1, j-1)[c] + src.at<Vec3b>(i-1, j)[c]*2 + src.at<Vec3b>(i-1, j+1)[c])/4;
            }
        }
    }
    //Vertical direction convolution
    for (int i = 1; i < temp.rows-1; i++) 
    {
        for (int j = 1; j < temp.cols-1; j++)
        {
            for (int c = 0; c < temp.channels(); c++) 
            {
            	//Convolve using 3x1 sobel operator on temporary image
                dst.at<Vec3s>(i,j)[c] = temp.at<Vec3s>(i-1, j-1)[c] + temp.at<Vec3s>(i, j-1)[c]*0 + temp.at<Vec3s>(i+1, j-1)[c]*(-1) ;
            }
        }
    }
    return 0;
}
 

int magnitude( Mat &sx, Mat &sy, Mat &dst )
{
    dst.create(sx.size(), sx.type());
    for (int i = 0; i < sx.rows; i++) 
    {
        for (int j = 0; j < sx.cols; j++)
        {
            for (int c = 0; c < sx.channels(); c++) 
            {
            	//Find euclidean magnitude for each channel in each pixel
                dst.at<Vec3s>(i,j)[c] = sqrt(pow(sx.at<Vec3s>(i, j)[c], 2) + pow(sy.at<Vec3s>(i, j)[c], 2));
            }
        }
    }  
    return 0;
}

int blurQuantize( Mat &src, Mat &dst, int levels )
{
    Mat temp;
    dst.create(src.size(), src.type());
    //Create temporary to hold blur image
    temp.create(src.size(), src.type());
    //Blur image using gaussian 5x5
    blur5x5(src, temp);
    //Calculate buckets
    int b = 255 / levels;
    for (int i = 0; i < temp.rows; i++) 
    {
        for (int j = 0; j < temp.cols; j++)
        {
            for (int c = 0; c < temp.channels(); c++) 
            {
            	//Manipulate channel values with bucket value
                dst.at<Vec3b>(i,j)[c] = temp.at<Vec3b>(i,j)[c] / b;
                dst.at<Vec3b>(i,j)[c] = dst.at<Vec3b>(i,j)[c] * b;
            }
        }
    }
    return 0;
}


int cartoon( Mat &src, Mat&dst, int levels, int magThreshold ) 
{
	Mat sobelx,sobely,magnitude1,blurquantized;
	dst.create(src.size(), CV_16SC3);
	//Calculate sobel x and store
    sobelX3x3(src, sobelx);
    //Calculate sobel y and store
    sobelY3x3(src, sobely);
    //Find magnitude
    magnitude(sobelx, sobely, magnitude1);
    //Blur-quantize image
    blurQuantize(src, blurquantized, levels);
    //bgr value for black
    Vec3s black = {0,0,0};
    for (int i = 0; i < blurquantized.rows; i++) 
    {
        for (int j = 0; j < blurquantized.cols; j++)
        {
        	int channelavg = 0;
            for (int c = 0; c < blurquantized.channels(); c++) 
            {
            	//Assign blur quantized value to final image
            	dst.at<Vec3s>(i,j)[c] = blurquantized.at<Vec3b>(i,j)[c];
            	//Find sum of all channels
                channelavg += magnitude1.at<Vec3s>(i,j)[c];
            }
            //Find average of all channels
            channelavg /= magnitude1.channels();
            //If average is greater than threshold, make pixel black
            if (channelavg > magThreshold)
            {
            	dst.at<Vec3s>(i,j) = black;
            }
        }
    }
    
    return 0;
}

int negative( Mat &src, Mat &dst)
{
	dst.create(src.size(), src.type());
	for (int i = 0; i < src.rows; i++) 
    {
        for (int j = 0; j < src.cols; j++) 
        {
        	for (int c = 0; c < src.channels(); c++) 
        	{
        		//Subtract from 255 the channel value
        		dst.at<Vec3b>(i,j)[c] = 255 - src.at<Vec3b>(i,j)[c];
        	}
        }
    }
    return 0;
}


int addsaltandpepper( Mat &src, Mat &dst)
{
	Mat gsrc;
	gsrc.create(src.size(), src.type());
	//Find greyscale
	greyscale(src, gsrc);
	//Find number of pixels
	int pixels = gsrc.rows * gsrc.cols;
	//Setting the number of pixels to noise as 0.5%
	int noisepixels = 0.005*pixels;
	dst.create(gsrc.size(), gsrc.type());
	//Make a copy of grescale image
	dst = gsrc.clone();
	for(int i = 0; i < noisepixels; i++)
	{
		if(i%2==0)
		{
			//Randomly pick a pixel and make it black
			dst.at<Vec3b>(rand()%dst.rows, rand()%dst.cols) = {0,0,0};
		}
		else
		{
			//Randomly pick a pixel and make it white
			dst.at<Vec3b>(rand()%dst.rows, rand()%dst.cols) = {255,255,255};
		}
	}
	return 0;
}

int medianfilter( Mat &src, Mat &dst )
{
	dst.create(src.size(), src.type());
	for(int i = 1; i < src.rows; i++)
	{
		for(int j = 1; j < src.cols; j++)
		{
			for(int c = 0; c < src.channels(); c++)
			{
				//Add all elements in 3x3 window to an array
				int arr[] = { src.at<Vec3b>(i-1,j-1)[c], src.at<Vec3b>(i-1,j)[c], src.at<Vec3b>(i-1,j+1)[c], src.at<Vec3b>(i,j-1)[c], src.at<Vec3b>(i,j)[c], src.at<Vec3b>(i,j+1)[c], src.at<Vec3b>(i+1,j-1)[c], src.at<Vec3b>(i+1,j)[c], src.at<Vec3b>(i+1,j+1)[c] };
				int n = sizeof(arr) / sizeof(arr[0]);
				//Sort array
				std::sort(arr, arr + n);
				//Assign median of array as output channel value
				dst.at<Vec3b>(i,j)[c] = arr[4];
			}
		}
	}
    greyscale(dst, dst);
	return 0;
}


int mirror( Mat &src, Mat &dst )
{
	dst.create(src.size(), src.type());
	for(int i = 0; i < src.rows; i++)
	{
		for(int j = 0; j < src.cols; j++)
		{
			//Mirror the image by reversing columns
			dst.at<Vec3b>(i,j) = src.at<Vec3b>(i,src.cols-j-1);
		}
	}
	return 0;	
}

int upsidedown( Mat &src, Mat &dst )
{
	dst.create(src.size(), src.type());
	for(int i = 0; i < src.rows; i++)
	{
		for(int j = 0; j < src.cols; j++)
		{
			//Rotate the image by 180 degrees by reversing rows
			dst.at<Vec3b>(i,j) = src.at<Vec3b>(src.rows-i-1,j);
		}
	}
	return 0;	
}

int detectblur( Mat &src, float threshold )
{
	Mat dst;
	Scalar mean,stddev;
    dst.create(src.size(), src.type());
    for (int i = 1; i < src.rows - 1; i++) 
    {
        for (int j = 1; j < src.cols - 1; j++) 
        {
        	for (int c = 0; c < src.channels(); c++) 
        	{
        		//COnvolve using 3x3 discrete laplacian operator
		        dst.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(1*src.at<Vec3b>(i-1,j)[c] + 1*src.at<Vec3b>(i+1,j)[c] + 1*src.at<Vec3b>(i,j-1)[c] + 1*src.at<Vec3b>(i,j+1)[c] + (-4)*src.at<Vec3b>(i,j)[c]);
		    }
        }
    }
    //Find standard mean
    meanStdDev(dst,mean,stddev,Mat());
    //Find variance
    float variance = stddev.val[0]*stddev.val[0];
    if(variance > threshold)
    {
    	//Imprint Not blurry text with variance value if it is more than threshold
    	putText(src,"Not Blurry - "+std::to_string(stddev.val[0]*stddev.val[0]),Point(10, 15),FONT_HERSHEY_DUPLEX,0.5,CV_RGB(0, 255, 0),1);
    }
    else
    {
    	//Imprint Blurry text with variance value if it is less than threshold
    	putText(src,"Blurry - "+std::to_string(stddev.val[0]*stddev.val[0]),Point(10, 15),FONT_HERSHEY_DUPLEX,0.5,CV_RGB(255, 0, 0),1);
    }
    return 0;   
}


