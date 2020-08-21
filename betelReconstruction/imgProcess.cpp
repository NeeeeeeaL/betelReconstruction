#include "imgProcess.h"

using namespace std;

void showImg(int width, int height, const char * windowName, const cv::Mat & src)
{
	cv::namedWindow(windowName, cv::WINDOW_NORMAL);
	cv::resizeWindow(windowName, width, height);
	cv::imshow(windowName, src);
}


cv::Point2f blockExtraction( cv::Mat& imgSrc)
{
	cv::Mat imgThreshold = cv::Mat::zeros(imgSrc.rows, imgSrc.cols, CV_8U);
	//cv::threshold(imgSrc, imgThreshold, 15, 255, CV_THRESH_BINARY);

	//sobel�任
	cv::Sobel(imgSrc, imgThreshold, imgSrc.depth(), 2, 0, 11);

	//��ʴ
	cv::Mat structElement1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(6, 2), cv::Point(-1, -1));
	cv::erode(imgThreshold, imgThreshold, structElement1);

	//��ֵ�˲�
	cv::medianBlur(imgThreshold, imgThreshold, 7);

	//����
	cv::Mat structElement2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 90), cv::Point(-1, -1));
	cv::dilate(imgThreshold, imgThreshold, structElement2);

	//��ʴ
	cv::Mat structElement3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 6), cv::Point(-1, -1));
	cv::erode(imgThreshold, imgThreshold, structElement3);

	//Ѱ����ͨ��
	vector<vector<cv::Point>> contours;
	vector<cv::Vec4i> hierarchy;

	cv::Point2f rect[4];//��С��Ӿ����ĸ��ǵ�

	cv::Point2f lkCenter;
	cv::Point2f lkCenter1;
	cv::Point2f lkCenter2;
	cv::Point2f lkCenterup;

	vector<cv::Point2f> lkPoint; //����4����ǵ�

	cv::findContours(imgThreshold, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	cv::Mat dstImg = imgThreshold.clone();
	cv::cvtColor(dstImg, dstImg, cv::COLOR_GRAY2BGR);

	vector<cv::RotatedRect> boundRect(contours.size());//������Ӿ��μ���

	for (int i = 0; i < contours.size(); i++)
	{
		double Area = fabs(contourArea(contours[i], true));
		if (Area > 2000)
		{
			cout << "Area" << i << " = " << Area << endl;

			boundRect[i] = cv::minAreaRect(cv::Mat(contours[i]));
			boundRect[i].points(rect);


			//������
			//cv::rectangle(imgThreshold, boundRect[i], cv::Scalar(0, 255, 0));
			for (int j = 0; j < 4; j++)
			{
				cv::line(dstImg, rect[j], rect[(j + 1) % 4], cv::Scalar(0, 255, 0), 6, 8);  //������С��Ӿ���ÿ����
				lkPoint.push_back(rect[j]);

			}
		}
	}

	for (int k = 0; k < lkPoint.size(); ++k)
	{
		cout << "lkPoint" << k << "=" << lkPoint[k] << endl;
	}

	cv::cvtColor(imgSrc, imgSrc, cv::COLOR_GRAY2BGR);

	if (lkPoint[2].x < lkPoint[7].x)//ȡ2��7��4��1
	{
		cv::line(imgSrc, lkPoint[2], lkPoint[7], cv::Scalar(0, 255, 255), 6, 8);
		cv::line(imgSrc, lkPoint[7], lkPoint[4], cv::Scalar(0, 255, 255), 6, 8);
		cv::line(imgSrc, lkPoint[4], lkPoint[1], cv::Scalar(0, 255, 255), 6, 8);
		cv::line(imgSrc, lkPoint[1], lkPoint[2], cv::Scalar(0, 255, 255), 6, 8);

		//��ȡ�����ϱ������ĵ�����
		lkCenter1.x = lkPoint[2].x + (lkPoint[4].x - lkPoint[2].x) / 2.0;
		lkCenter1.y = lkPoint[2].y + (lkPoint[4].y - lkPoint[2].y) / 2.0;
		cv::circle(imgSrc, lkCenter1, 200, cv::Scalar(255, 0, 0), 4);

		lkCenter2.x = lkPoint[1].x + (lkPoint[7].x - lkPoint[1].x) / 2.0;
		lkCenter2.y = lkPoint[7].y + (lkPoint[1].y - lkPoint[7].y) / 2.0;
		cv::circle(imgSrc, lkCenter2, 200, cv::Scalar(255, 0, 255), 4);

	}
	else//ȡ6��3��0��5
	{
		cv::line(imgSrc, lkPoint[6], lkPoint[3], cv::Scalar(0, 255, 255), 6, 8);
		cv::line(imgSrc, lkPoint[3], lkPoint[0], cv::Scalar(0, 255, 255), 6, 8);
		cv::line(imgSrc, lkPoint[0], lkPoint[5], cv::Scalar(0, 255, 255), 6, 8);
		cv::line(imgSrc, lkPoint[5], lkPoint[6], cv::Scalar(0, 255, 255), 6, 8);

		//��ȡ�����ϱ������ĵ�����
		lkCenter1.x = lkPoint[6].x + (lkPoint[0].x - lkPoint[6].x) / 2.0;
		lkCenter1.y = lkPoint[6].y + (lkPoint[0].y - lkPoint[6].y) / 2.0;
		cv::circle(imgSrc, lkCenter1, 200, cv::Scalar(255, 0, 0), 4);

		lkCenterup.x = lkCenter1.x;
		lkCenterup.y = lkCenter1.y + 500;

		cv::circle(imgSrc, lkCenterup, 200, cv::Scalar(255, 0, 0), 4);

		lkCenter2.x = lkPoint[5].x + (lkPoint[3].x - lkPoint[5].x) / 2.0;
		lkCenter2.y = lkPoint[3].y + (lkPoint[5].y - lkPoint[3].y) / 2.0;
		cv::circle(imgSrc, lkCenter2, 200, cv::Scalar(255, 0, 255), 4);
	}


	//���������ĵ�1��Ϊ�������ĵ�
	lkCenter = lkCenter1;

	return lkCenter;
}


//��ȡ������������
void betelExtraction(cv::Mat& imgSrc)
{
	cv::equalizeHist(imgSrc, imgSrc);

	cv::Mat imgThreshold;
	cv::threshold(imgSrc, imgThreshold, 14, 255, CV_THRESH_BINARY);

	//��ֵ�˲�
	cv::medianBlur(imgThreshold, imgThreshold, 19);

	//cv::Mat imgSobel;
	//cv::Sobel(imgThreshold, imgSobel, imgSrc.depth(), 1, 0, 15);

	////��ʴ
	//cv::Mat structElement1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4), cv::Point(-1, -1));
	//cv::erode(imgSobel, imgSobel, structElement1);


	////��ֵ�˲�
	//cv::medianBlur(imgSobel, imgSobel, 9);

	////����
	//cv::Mat structElement2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(25, 80), cv::Point(-1, -1));
	//cv::dilate(imgSobel, imgSobel, structElement2);

	////����
	//cv::Mat structElement3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(60, 80), cv::Point(-1, -1));
	//cv::dilate(imgSobel, imgSobel, structElement3);

	//������ʱ������
	cv::Mat structElement2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(20, 60), cv::Point(-1, -1));
	cv::erode(imgThreshold, imgThreshold, structElement2);

	//Ѱ����ͨ��
	vector<vector<cv::Point>> contours;
	vector<cv::Vec4i> hierarchy;

	cv::Point2f rect[4];//��С��Ӿ����ĸ��ǵ�

	cv::Point2f lkCenter;

	cv::findContours(imgThreshold, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	cv::Mat dstImg = imgSrc.clone();
	cv::cvtColor(dstImg, dstImg, cv::COLOR_GRAY2BGR);

	vector<cv::RotatedRect> boundRect(contours.size());//������Ӿ��μ���

	for (int i = 0; i < contours.size(); i++)
	{
		double Area = fabs(contourArea(contours[i], true));
		if (Area > 150000 && Area < 800000)
		{
			cout << "Area" << i << " = " << Area << endl;

			boundRect[i] = cv::minAreaRect(cv::Mat(contours[i]));
			boundRect[i].points(rect);

			cv::circle(dstImg, boundRect[i].center, 40, cv::Scalar(255, 0, 255), 6, 8);

			cv::drawContours(dstImg, contours, i, cv::Scalar(255, 255, 0), 6, 8, hierarchy);

			//������
			//cv::rectangle(imgThreshold, boundRect[i], cv::Scalar(0, 255, 0));
			for (int j = 0; j < 4; j++)
			{
				cv::line(dstImg, rect[j], rect[(j + 1) % 4], cv::Scalar(0, 255, 0), 6, 8);  //������С��Ӿ���ÿ����
			}
		}
	}

	cv::waitKey(0);

}
