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