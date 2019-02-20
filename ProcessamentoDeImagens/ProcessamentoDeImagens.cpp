// ProcessamentoDeImagens.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include "pch.h"
#include <iostream>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#define BINARYLENGTH 256

using namespace std;
using namespace cv;

void binaryImage(Mat &img);
void fillWithZerosOnMargins(Mat &img);
void convolMask(Mat &img, int convolMask[3][3]);
void createHistogram(Mat &im, int *hist);
void calculateProbabilities(int histogram[], int totalPixels);
Mat linearTransformation(Mat imOrigin,  Mat imCopy);
Mat nonLinearTransformation(Mat imOrigin,  Mat imCopy);
Mat autoScaleTransformation(Mat imOrigin,  Mat imCopy);

Mat optimalThresholding(Mat imOriginal, Mat imCopy);
int optimalThreshold(Mat imOriginal);




int main()
{
	int maskMatrizH[3][3] = { {0,0,-1}, {0,1,0}, {0,0,0} };
	int maskMatrizV[3][3] = { {-1,0,0}, {0,1,0}, {0,0,0} };
	int hist[256];
	string picture[] = { "teste.jpg", "teste2.jpg", "teste3.jpg" };
	string aux;
	Mat im, imLinear, imNonLinear, imAutoScale, imOptThreshold;
	//for (int i = 0; i < 3; i++) {
	//	aux = "photos/" + picture[i];
	//	im = imread(aux, IMREAD_GRAYSCALE);
	//	if (!im.data)
	//		std::cout << "Não foi possivel carregar a imagem : " + picture[i];
	//	// binaryImage(im);
	//	namedWindow(picture[i].c_str(),cv::WINDOW_NORMAL);
	//	imshow(picture[i].c_str(), im);
	//}
	im = imread("photos/" + picture[1], IMREAD_GRAYSCALE);
	im.copyTo(imLinear);
	im.copyTo(imNonLinear);
	im.copyTo(imAutoScale);
	im.copyTo(imOptThreshold);

	if (!im.data) {
		std::cout << "Nao foi possivel carregar a imagem : " << picture[1];
	}

	//fillWithZerosOnMargins(im);
	//convolMask(im, maskMatrizH);
	//convolMask(im, maskMatrizV);
	namedWindow(picture[1], cv::WINDOW_AUTOSIZE);
	imOptThreshold = optimalThresholding(im, imOptThreshold);
	///*createHistogram(im, hist);
	//imLinear = linearTransformation(im, imLinear);
	//imNonLinear = nonLinearTransformation(im, imNonLinear);
	//imAutoScale = autoScaleTransformation(im, imAutoScale);
	imshow(picture[1].c_str(), im);
	//imshow("Tranformacao Linear", imLinear);
	//imshow("Transformacao Nao Linear", imNonLinear);
	//imshow("Auto Escalado", imAutoScale);
	//*
	imshow("Optimal Threshold", imOptThreshold);
	waitKey(0);
	cv::destroyAllWindows();
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
void binaryImage(Mat &img){
	for(int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++) {
			img.at<uchar>(i, j) = (img.at<uchar>(i, j) <= 128) ? 0 : 255;
		}
}

void convolMask(Mat &img, int convolMask[3][3]){
	for (int i = 1; i < img.rows -1; i++)
		for (int j = 1; j < img.cols -1; j++) {
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

void fillWithZerosOnMargins(Mat &img){
	for(int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++) {
			if (i == 0 || j == 0 || i == (img.cols - 1) || j == (img.rows - 1)) {
				img.at<uchar>(i,j) = 0;
			}
		}
}

void createHistogram(Mat &im, int *hist) {
	int index, total = 0;
	for (int i = 0; i <= 255; i++)
		hist[i] = 0;

	for (int i = 0; i < im.rows; i++)
		for (int j = 0; j < im.cols; j++) {
			index = im.at<uchar>(i, j);
			hist[index]++;
		}

	for (int i = 0; i <= 255; i++) total += hist[i];

	cout << "O total de pixels e: " << total << endl;

	calculateProbabilities(hist, total);
}

void calculateProbabilities(int histogram[], int totalPixels){
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
			<< faixa  << " e : " << prob[i] * 100 << endl;
	}
}

Mat linearTransformation(Mat imOrigin, Mat imCopy) {
	int contraste = 1, brilho = 32, hist[256];
	int result;
	for (int i = 0; i < imOrigin.rows; i++) {
		for (int j = 0; j < imOrigin.cols; j++) {
			result = contraste * imOrigin.at<uchar>(i, j) + brilho;
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
			result = logConst * log2(imOrigin.at<uchar>(i,j) + 1);
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

// Thresholding

Mat optimalThresholding(Mat imOriginal, Mat imCopy){
	int threshold = optimalThreshold(imOriginal);

	for(int i = 0; i < imOriginal.rows; i++)
		for (int j = 0; j < imOriginal.cols; j++) {
			if (imOriginal.at<uchar>(i, j) < threshold) {
				imCopy.at<uchar>(i, j) = 0;
			} else imCopy.at<uchar>(i, j) = 255;
		}

	return imCopy;
}

int optimalThreshold(Mat imOriginal){
	int optThreshold = 125, optThresholdUp = 0, hist[BINARYLENGTH], totalPixels = 0;
	float histProb[BINARYLENGTH], averageBack, averageFore, probSumBack, probSumFore;
	double result;
	
	createHistogram(imOriginal, hist);

	for (int i = 0; i < BINARYLENGTH; i++) totalPixels += hist[i]; // Calcula o total de pixels na imagem

	for (int i = 0; i < BINARYLENGTH; i++) {
		result = hist[i];
		result = result / totalPixels;
		histProb[i] = result; // Calcula a probabilidade de cada pixel
	}

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
}