#include "reconstruction.h"
#include "imgProcess.h"

using namespace std;

int main()
{
	cv::Mat imgSrc = cv::imread("betel-wx2.bmp", 0);

	//��ȡ����λ�ã���λ�����ĵ�
	//cv::Point2f blockCenter;

	//blockCenter = blockExtraction(imgSrc);

	//cout << "center:" << blockCenter << endl;


	betelExtraction(imgSrc);

	cv::waitKey(0);
	cv::destroyAllWindows();
}