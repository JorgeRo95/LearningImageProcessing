#include "Thresholding.h"

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
