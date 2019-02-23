#pragma once
#include "pch.h"
#include <iostream>
#include <algorithm>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "Constants.h"
#include "Transformations.h"

using cv::Mat;
using cv::Scalar;
using cv::Point;
using cv::namedWindow;
using cv::imshow;
using std::cout;
int calculateTotalPixels(int hist[]);
void createProbDistribution(int hist[], float *distHist, int totalPixels);
Mat computeIntegralImage(Mat imOrigin, Mat imIntegral);
void histImageCreate(Mat im, int hist[]);