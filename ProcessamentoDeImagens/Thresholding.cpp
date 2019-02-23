#include "pch.h"
#include "Thresholding.h"
#include "Uteis.h"
#include "Transformations.h"

Mat optimalThresholding(Mat imOriginal, Mat imCopy) {
	int threshold = optimalThreshold(imOriginal);

	for (int i = 0; i < imOriginal.rows; i++)
		for (int j = 0; j < imOriginal.cols; j++) {
			if (imOriginal.at<uchar>(i, j) < threshold) {
				imCopy.at<uchar>(i, j) = 0;
			}
			else imCopy.at<uchar>(i, j) = 255;
		}

	return imCopy;
}

int optimalThreshold(Mat imOriginal) {
	int optThreshold = 125, optThresholdUp = 0, hist[BINARYLENGTH], totalPixels = 0;
	float histProb[BINARYLENGTH], averageBack, averageFore, probSumBack, probSumFore;
	double result;

	createHistogram(imOriginal, hist);
	createProbDistribution(hist, histProb, calculateTotalPixels(hist));

	while (optThreshold != optThresholdUp) {
		averageBack = 0, averageFore = 0;
		probSumBack = 0, probSumFore = 0;
		if (optThresholdUp != 0) optThreshold = optThresholdUp;

		for (int i = 0; i < optThreshold; i++) {
			probSumBack += histProb[i];
			averageBack += histProb[i] * i;
		}
		averageBack = averageBack / probSumBack; // Calcula a média

		probSumFore = 1 - probSumBack;

		for (int i = optThreshold; i < BINARYLENGTH; i++) {
			averageFore += histProb[i] * i;
		}
		averageFore = averageFore / probSumFore;

		optThresholdUp = (averageBack + averageFore) / 2;
	}

	return optThreshold;
} // Faz parte do Algoritmo acima

Mat otsuThresholding(Mat imOrigin, Mat imCopy) {
	int threshold = calculateOtsuThreshold(imOrigin);
	cout << "Threshold is " << threshold << endl;
	for (int i = 0; i < imOrigin.rows; i++)
		for (int j = 0; j < imOrigin.cols; j++) {
			imCopy.at<uchar>(i, j) = (imOrigin.at<uchar>(i, j) <= threshold) ? 0 : 255;
		}
	return imCopy;
}

int calculateOtsuThreshold(Mat imOrigin) {
	int hist[BINARYLENGTH], threshold;
	double meanBack = 0, meanFore = 0, maximum = 0, varianceTotal;
	float sumBack = 0, sumFore = 0, histProb[BINARYLENGTH];;
	createHistogram(imOrigin, hist);
	createProbDistribution(hist, histProb, calculateTotalPixels(hist));
	for (int otsuThreshold = 0; otsuThreshold < BINARYLENGTH; otsuThreshold++) {
		for (int i = 0; i < BINARYLENGTH; i++) {
			if (i < otsuThreshold) {
				sumBack += histProb[i];
			}
			else {
				sumFore += histProb[i];
			}
		}

		for (int i = 0; i < BINARYLENGTH; i++) {
			if (i < otsuThreshold) {
				meanBack += (histProb[i] * i);
			}
			else {
				meanFore += (histProb[i] * i);
			}
		}

		if (sumBack != 0) meanBack / sumBack;
		if (sumFore != 0) meanFore / sumFore;

		varianceTotal = sumFore * sumBack * pow(meanFore - meanBack, 2);

		if (varianceTotal >= maximum) {
			maximum = varianceTotal;
			threshold = otsuThreshold;
		}
	}
	return threshold;
}

Mat adaptiveThresholding(Mat imOrigin, Mat imCopy) {
	int S = imOrigin.size().width/8, S2 = S/2;
	int x1, x2, y1, y2, count, sum;
	int t = 15; // Definido por Wellner(1993)
	//Mat imIntegral;
	//imOrigin.copyTo(imIntegral);
	//
	//for (int i = 0; i < imOrigin.size().width; i++) {
	//	sum = 0;
	//	for (int j = 0; j < imOrigin.size().height; j++) {
	//		sum += imOrigin.at<uchar>(i, j);

	//		if (i == 0) imIntegral.at<uchar>(i, j) = sum;
	//		else imIntegral.at<uchar>(i, j) = imIntegral.at<uchar>(i - 1, j) + sum;

	//	}
	//}
	//
	//for(int i = 0; i < imOrigin.size().width; i++)
	//	for (int j = 0; j < imOrigin.size().height; j++) {

	//		x1 = i - S2;
	//		x2 = i + S2;
	//		y1 = j - S2;
	//		y2 = j + S2;

	//		if (x1 < 0) x1 = 0;
	//		if (x2 >= imOrigin.cols) x2 = imOrigin.size().width - 1;
	//		if (y1 < 0) y1 = 0;
	//		if (y2 >= imOrigin.rows) y2 = imOrigin.size().height - 1;

	//		count = (x2 - x1) * (y2 - y1);
	//		/*
	//		sum = imIntegral.at<uchar>(i + S - 1, j + S - 1);
	//		if (i > 0) sum = sum - imIntegral.at<uchar>(i - 1, j - 1 + S);
	//		if (j > 0) sum = sum - imIntegral.at<uchar>(i - 1 + S, j - 1);
	//		if (i > 0 && j > 0) sum = sum + imIntegral.at<uchar>(i - 1, j - 1);*/

	//		sum = imIntegral.at<uchar>(x2, y2) - imIntegral.at<uchar>(x2, y1 - 1) - imIntegral.at<uchar>(x1 - 1, y2) + imIntegral.at<uchar>(x1 - 1, y1 - 1);

	//		if ((imOrigin.at<uchar>(i, j) * count) < ((sum * (100 - t)) / 100)) {
	//			imCopy.at<uchar>(i, j) = 0;
	//		} else {
	//			imCopy.at<uchar>(i, j) = 255;
	//		}
		//}
	cv::adaptiveThreshold(imOrigin, imCopy, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 101, 20);
	return imCopy;
}