#include "reconstruction.h"
#include "imgProcess.h"

using namespace std;

int main()
{
	cv::Mat imgSrc = cv::imread("betel-2.bmp", 0);


	//��ȡ����λ�ã���λ�����ĵ�
	//cv::Point2f blockCenter;

	//blockCenter = blockExtraction(imgSrc);

	//cout << "center:" << blockCenter << endl;

	//��ȡ����
	//betelExtraction(imgSrc);

	cv::equalizeHist(imgSrc, imgSrc);

	cv::Mat imgThreshold;
	cv::threshold(imgSrc, imgThreshold, 10, 255, CV_THRESH_BINARY);

	//��ֵ�˲�
	cv::medianBlur(imgThreshold, imgThreshold, 19);

	cv::Mat imgSobel;
	cv::Sobel(imgThreshold, imgSobel, imgSrc.depth(), 1, 0, 15);

	//��ʴ
	cv::Mat structElement1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4), cv::Point(-1, -1));
	cv::erode(imgSobel, imgSobel, structElement1);


	//��ֵ�˲�
	cv::medianBlur(imgSobel, imgSobel, 5);

	//����
	cv::Mat structElement2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 80), cv::Point(-1, -1));
	cv::dilate(imgSobel, imgSobel, structElement2);

	//����
	cv::Mat structElement3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(80, 80), cv::Point(-1, -1));
	cv::dilate(imgSobel, imgSobel, structElement3);

	//Ѱ����ͨ��
	vector<vector<cv::Point>> contours;
	vector<cv::Vec4i> hierarchy;

	cv::Point2f rect[4];//��С��Ӿ����ĸ��ǵ�

	cv::Point2f lkCenter;

	cv::findContours(imgSobel, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	cv::Mat dstImg = imgSrc.clone();
	cv::cvtColor(dstImg, dstImg, cv::COLOR_GRAY2BGR);

	vector<cv::RotatedRect> boundRect(contours.size());//������Ӿ��μ���

	for (int i = 0; i < contours.size(); i++)
	{
		double Area = fabs(contourArea(contours[i], true));
		if (Area > 100000)
		{
			cout << "Area" << i << " = " << Area << endl;

			boundRect[i] = cv::minAreaRect(cv::Mat(contours[i]));
			boundRect[i].points(rect);

			cv::drawContours(dstImg, contours, i, cv::Scalar(255, 255, 0), 4, 8, hierarchy);

			//������
			//cv::rectangle(imgThreshold, boundRect[i], cv::Scalar(0, 255, 0));
			for (int j = 0; j < 4; j++)
			{
				cv::line(dstImg, rect[j], rect[(j + 1) % 4], cv::Scalar(0, 255, 0), 6, 8);  //������С��Ӿ���ÿ����
			}
		}
	}

	cv::waitKey(0);
	cv::destroyAllWindows();
}