#include "pch.h"
#include "Constants.h"
#include "Uteis.h"

void createProbDistribution(int hist[], float *distHist, int totalPixels) {
	double result;
	for (int i = 0; i < BINARYLENGTH; i++) {
		result = hist[i];
		result = result / totalPixels;
		distHist[i] = result; // Calcula a probabilidade de cada pixel
	}
}

int calculateTotalPixels(int hist[]) {
	int totalPixels = 0;
	for (int i = 0; i < BINARYLENGTH; i++) totalPixels += hist[i]; // Calcula o total de pixels na imagem
	return totalPixels;
}

Mat computeIntegralImage(Mat imOrigin, Mat imIntegral) {
	int scalar;

	for (int i = 1; i < imOrigin.rows; i++) {
		imIntegral.at<uchar>(i, 0) = imIntegral.at<uchar>(i - 1, 0) + imOrigin.at<uchar>(i, 0);
	}
	for (int j = 1; j < imOrigin.cols; j++) {

		scalar = imOrigin.at<uchar>(0,j); //acumulador de escala
		imIntegral.at<uchar>(0, j) = imIntegral.at<uchar>(0, j - 1) + scalar;
		
		for (int i = 1; i < imOrigin.rows; i++) {
			scalar += imOrigin.at<uchar>(i, j);
			imIntegral.at<uchar>(i, j) = imIntegral.at<uchar>(i, j - 1) + scalar;
		}
				
	}
	
	return imIntegral;
}

void histImageCreate(Mat im, int hist[]) {
	createHistogram(im, hist);
	Mat histImage(512, 400, CV_8UC3, Scalar(0,0));
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / 256);

	for (int i = 0; i < 256; i++)
	{
		line(histImage, Point(bin_w*(i), hist_h), Point(bin_w*(i), hist_h - hist[i]), Scalar(0, 0, 0), 1, 8, 0);
	}
	namedWindow("Histogram", cv::WINDOW_AUTOSIZE);
	imshow("Histogra", histImage);
}