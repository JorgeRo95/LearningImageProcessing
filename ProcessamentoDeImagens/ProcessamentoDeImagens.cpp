// ProcessamentoDeImagens.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//
#include "Transformations.h"
#include "Thresholding.h"

using namespace std;
using namespace cv;


int main()
{
	int maskMatrizH[3][3] = { {0,0,-1}, {0,1,0}, {0,0,0} };
	int maskMatrizV[3][3] = { {-1,0,0}, {0,1,0}, {0,0,0} };
	int hist[256];
	string picture[] = { "teste.jpg", "teste2.jpg", "teste3.jpg" };
	string aux;
	Mat im, imLinear, imNonLinear, imAutoScale, imOptThreshold, imOtsu;

	im = imread("photos/" + picture[0], IMREAD_GRAYSCALE);
	im.copyTo(imLinear);
	im.copyTo(imNonLinear);
	im.copyTo(imAutoScale);
	im.copyTo(imOptThreshold);
	im.copyTo(imOtsu);

	if (!im.data) {
		std::cout << "Nao foi possivel carregar a imagem : " << picture[1];
	}

	//fillWithZerosOnMargins(im);
	//convolMask(im, maskMatrizH);
	//convolMask(im, maskMatrizV);
	namedWindow(picture[1], cv::WINDOW_AUTOSIZE);
	imOptThreshold = optimalThresholding(im, imOptThreshold);
	imOtsu = otsuThresholding(im, imOtsu);
	createHistogram(im, hist);
	imLinear = linearTransformation(im, imLinear);
	imNonLinear = nonLinearTransformation(im, imNonLinear);
	imAutoScale = autoScaleTransformation(im, imAutoScale);
	imshow(picture[1].c_str(), im);
	imshow("Tranformacao Linear", imLinear);
	imshow("Transformacao Nao Linear", imNonLinear);
	imshow("Auto Escalado", imAutoScale);
	imshow("Optimal Threshold", imOptThreshold);
	imshow("Otsu Threshold", imOtsu);
	waitKey(0);
	cv::destroyAllWindows();
	return 0;
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