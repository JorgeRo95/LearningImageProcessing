#pragma once
#include "pch.h"
#include <iostream>
#include <algorithm>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "Constants.h"
#include "Uteis.h"

using cv::Mat;

void binaryImage(Mat img);
void fillWithZerosOnMargins(Mat img);
void convolMask(Mat img, int convolMask[3][3]);
void createHistogram(Mat im, int *hist);
void calculateProbabilities(int histogram[], int totalPixels);
Mat linearTransformation(Mat imOrigin, Mat imCopy);
Mat nonLinearTransformation(Mat imOrigin, Mat imCopy);
Mat autoScaleTransformation(Mat imOrigin, Mat imCopy);
