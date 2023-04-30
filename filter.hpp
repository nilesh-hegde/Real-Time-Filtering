#ifndef FILTER_HPP
#define FILTER_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int greyscale( cv::Mat &src, cv::Mat &dst);
int blur5x5( cv::Mat &src, cv::Mat &dst) ;
int sobelX3x3( cv::Mat &src, cv::Mat &dst );
int sobelY3x3( cv::Mat &src, cv::Mat &dst );
int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst );
int blurQuantize( cv::Mat &src, cv::Mat &dst, int levels );
int cartoon( cv::Mat &src, cv::Mat&dst, int levels, int magThreshold ) ;
int negative( cv::Mat &src, cv::Mat &dst);
int addsaltandpepper( cv::Mat &src, cv::Mat &dst);
int medianfilter( cv::Mat &src, cv::Mat &dst );
int mirror( cv::Mat &src, cv::Mat &dst );
int upsidedown( cv::Mat &src, cv::Mat &dst );
int detectblur( cv::Mat &src, float threshold );

#endif 

