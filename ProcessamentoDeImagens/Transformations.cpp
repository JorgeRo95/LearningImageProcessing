#include "pch.h"
#include "Transformations.h"

using namespace std;
using cv::Mat;

void binaryImage(Mat img) {
	for (int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++) {
			img.at<uchar>(i, j) = (img.at<uchar>(i, j) <= 128) ? 0 : 255;
		}
}

void convolMask(Mat img, int convolMask[3][3]) {
	for (int i = 1; i < img.rows - 1; i++)
		for (int j = 1; j < img.cols - 1; j++) {
			img.at<uchar>(i, j) =
				(img.at<uchar>(i - 1, j - 1) * convolMask[0][0])
				+ (img.at<uchar>(i - 1, j) * convolMask[0][1])
				+ (img.at<uchar>(i - 1, j + 1) * convolMask[0][2])
				// Segunda linha
				+ (img.at<uchar>(i, j - 1) * convolMask[1][0])
				+ (img.at<uchar>(i, j) * convolMask[1][1])
				+ (img.at<uchar>(i, j + 1) * convolMask[1][2])
				// Terceira Linha
				+ (img.at<uchar>(i + 1, j - 1) * convolMask[2][0])
				+ (img.at<uchar>(i + 1, j) * convolMask[2][1])
				+ (img.at<uchar>(i + 1, j + 1) * convolMask[2][2]);
		}
}

void fillWithZerosOnMargins(Mat img) {
	for (int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++) {
			if (i == 0 || j == 0 || i == (img.cols - 1) || j == (img.rows - 1)) {
				img.at<uchar>(i, j) = 0;
			}
		}
}

void createHistogram(Mat im, int *hist) {
	int index, total = 0;
	for (int i = 0; i <= 255; i++) {
		hist[i] = 0;
	}
	for (int k = 0; k < im.rows; k++) {
		for (int j = 0; j < im.cols; j++) {
			index = im.at<uchar>(k, j);
			hist[index]++;
		}
	}

	for (int i = 0; i <= 255; i++) total += hist[i];

	cout << "O total de pixels e: " << total << endl;

	calculateProbabilities(hist, total);
}

void calculateProbabilities(int histogram[], int totalPixels) {
	float prob[8];
	int faixa;
	for (int i = 0; i < 8; i++) prob[i] = 0;

	for (int i = 0; i < 256; i++) {
		if (i <= 0.125 * 255) prob[0] += histogram[i];
		else if (i <= 0.250 * 255) prob[1] += histogram[i];
		else if (i <= 0.375 * 255) prob[2] += histogram[i];
		else if (i <= 0.500 * 255) prob[3] += histogram[i];
		else if (i <= 0.625 * 255) prob[4] += histogram[i];
		else if (i <= 0.750 * 255) prob[5] += histogram[i];
		else if (i <= 0.875 * 255) prob[6] += histogram[i];
		else prob[7] += histogram[i];
	}

	for (int i = 0; i < 8; i++) prob[i] = prob[i] / totalPixels;

	for (int i = 0; i < 8; i++) {
		faixa = ((i + 1) * 0.125) * 255;
		cout << "A probablidadade de um pixel estar ate o valor "
			<< faixa << " e : " << prob[i] * 100 << endl;
	}
}

Mat linearTransformation(Mat imOrigin, Mat imCopy) {
	int contraste = 1, brilho = 32, hist[256];
	int result;
	for (int i = 0; i < imOrigin.rows; i++) {
		for (int j = 0; j < imOrigin.cols; j++) {
			result = (contraste * imOrigin.at<uchar>(i, j)) + brilho;
			if (result > 255) imCopy.at<uchar>(i, j) = 255;
			else imCopy.at<uchar>(i, j) = result;
		}
	}

	cout << "Criando novo histograma com imagem transformada " << endl;
	createHistogram(imCopy, hist);

	return imCopy;
}

Mat nonLinearTransformation(Mat imOrigin, Mat imCopy) {
	float logConst = 31.875;
	int result, hist[BINARYLENGTH];
	for (int i = 0; i < imOrigin.rows; i++) {
		for (int j = 0; j < imOrigin.cols; j++) {
			result = logConst * log2(imOrigin.at<uchar>(i, j) + 1);
			if (result > 255) imCopy.at<uchar>(i, j) = 255;
			else imCopy.at<uchar>(i, j) = result;
		}
	}

	cout << "Calculando histograma da imagem transformada nao linearmente" << endl;
	createHistogram(imCopy, hist);

	return imCopy;
}

Mat autoScaleTransformation(Mat imOrigin, Mat imCopy) {
	int result, hist[BINARYLENGTH], minValueIndex = 0, maxValueIndex = BINARYLENGTH;

	createHistogram(imOrigin, hist);

	while (hist[minValueIndex] == 0) {
		minValueIndex++;
	}

	while (hist[maxValueIndex] == 0) {
		maxValueIndex--;
	}

	for (int i = 0; i < imOrigin.rows; i++) {
		for (int j = 0; j < imOrigin.cols; j++) {
			result = (255 / (maxValueIndex - minValueIndex))*(imOrigin.at<uchar>(i, j) - minValueIndex);
			if (result < 0) imCopy.at<uchar>(i, j) = 0;
			else imCopy.at<uchar>(i, j) = result;
		}
	}

	cout << "Calculando histograma da imagem transformada auto escalada" << endl;
	createHistogram(imCopy, hist);
	return imCopy;
}
