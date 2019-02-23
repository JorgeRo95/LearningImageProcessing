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
using std::cout;
using std::endl;

Mat optimalThresholding(Mat imOriginal, Mat imCopy);
int optimalThreshold(Mat imOriginal);

Mat otsuThresholding(Mat imOrigin, Mat imCopy);
int calculateOtsuThreshold(Mat imOrigin);

Mat adaptiveThresholding(Mat imOrigin, Mat imCopy); // Utiliza uma imagem integral para realizar as computações