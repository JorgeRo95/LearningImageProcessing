#pragma once
#include "pch.h"
#include <iostream>
#include <algorithm>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "Constants.h"

using cv::Mat;
int calculateTotalPixels(int hist[]);
void createProbDistribution(int hist[], float *distHist, int totalPixels);