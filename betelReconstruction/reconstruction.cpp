#include "reconstruction.h"
#include "imgProcess.h"

using namespace std;

int main()
{
	cv::Mat imgSrc = cv::imread("lk-8.bmp", 0);

	cv::Mat imgThreshold = cv::Mat::zeros(imgSrc.rows, imgSrc.cols, CV_8U);
	cv::threshold(imgSrc, imgThreshold, 15, 255, CV_THRESH_BINARY);

	//sobel变换
	cv::Sobel(imgSrc, imgThreshold, imgSrc.depth(), 2, 0, 11);

	//腐蚀
	cv::Mat structElement1 = cv::getStructuringElement(cv::MORPH_RECT,cv:: Size(6, 4), cv::Point(-1, -1));
	cv::erode(imgThreshold, imgThreshold, structElement1);

	//中值滤波
	cv::medianBlur(imgThreshold, imgThreshold, 5);

	//膨胀
	cv::Mat structElement2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 80), cv::Point(-1, -1));
	cv::dilate(imgThreshold, imgThreshold, structElement2);

	//腐蚀
	cv::Mat structElement3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 6), cv::Point(-1, -1));
	cv::erode(imgThreshold, imgThreshold, structElement3);

	//寻找连通域
	vector<vector<cv::Point>> contours;
	vector<cv::Vec4i> hierarchy;
	
	cv::Point2f rect[4];//最小外接矩形四个角点

	cv::Point2f lkCenter;
	cv::Point2f lkCenter1;
	cv::Point2f lkCenter2;

	vector<cv::Point2f> lkPoint; //量块4个外角点

	cv::findContours(imgThreshold, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	cv::Mat dstImg = imgThreshold.clone();
	cv::cvtColor(dstImg, dstImg, cv::COLOR_GRAY2BGR);

	vector<cv::RotatedRect> boundRect(contours.size());//定义外接矩形集合

	for (int i = 0; i < contours.size(); i++)
	{
		double Area = fabs(contourArea(contours[i], true));
		if (Area > 10000)
		{
			cout << "Area" << i << " = " << Area << endl;

			boundRect[i] = cv::minAreaRect(cv::Mat(contours[i]));
			boundRect[i].points(rect);
			

			//画矩形
			//cv::rectangle(imgThreshold, boundRect[i], cv::Scalar(0, 255, 0));
			for (int j = 0; j < 4; j++)
				{
				    cv::line(dstImg, rect[j], rect[(j + 1) % 4], cv::Scalar(0, 255, 0), 2, 8);  //绘制最小外接矩形每条边
					lkPoint.push_back(rect[j]);
					
				}
		}
	}

	for (int k = 0; k < lkPoint.size(); ++k)
	{
		cout << "lkPoint" << k << "=" << lkPoint[k] << endl;
	}

	cv::cvtColor(imgSrc, imgSrc, cv::COLOR_GRAY2BGR);

	if (lkPoint[2].x < lkPoint[7].x)//取2，7，4，1
	{
		cv::line(imgSrc, lkPoint[2], lkPoint[7], cv::Scalar(0, 255, 255), 4, 8);
		cv::line(imgSrc, lkPoint[7], lkPoint[4], cv::Scalar(0, 255, 255), 4, 8);
		cv::line(imgSrc, lkPoint[4], lkPoint[1], cv::Scalar(0, 255, 255), 4, 8);
		cv::line(imgSrc, lkPoint[1], lkPoint[2], cv::Scalar(0, 255, 255), 4, 8);

		//获取量块上表面中心点坐标
		lkCenter1.x = lkPoint[2].x + (lkPoint[4].x - lkPoint[2].x) / 2.0;
		lkCenter1.y = lkPoint[2].y + (lkPoint[4].y - lkPoint[2].y) / 2.0;
		cv::circle(imgSrc, lkCenter1, 200, cv::Scalar(255, 0, 0), 4);

		lkCenter2.x = lkPoint[1].x + (lkPoint[7].x - lkPoint[1].x) / 2.0;
		lkCenter2.y = lkPoint[7].y + (lkPoint[1].y - lkPoint[7].y) / 2.0;
		cv::circle(imgSrc, lkCenter2, 200, cv::Scalar(255, 0, 255), 4);

	}
	else//取6，3，0，5
	{
		cv::line(imgSrc, lkPoint[6], lkPoint[3], cv::Scalar(0, 255, 255), 4, 8);
		cv::line(imgSrc, lkPoint[3], lkPoint[0], cv::Scalar(0, 255, 255), 4, 8);
		cv::line(imgSrc, lkPoint[0], lkPoint[5], cv::Scalar(0, 255, 255), 4, 8);
		cv::line(imgSrc, lkPoint[5], lkPoint[6], cv::Scalar(0, 255, 255), 4, 8);

		//获取量块上表面中心点坐标
		lkCenter1.x = lkPoint[6].x + (lkPoint[0].x - lkPoint[6].x) / 2.0;
		lkCenter1.y = lkPoint[6].y + (lkPoint[0].y - lkPoint[6].y) / 2.0;
		cv::circle(imgSrc, lkCenter1, 200, cv::Scalar(255, 0, 0), 4);

		lkCenter2.x = lkPoint[5].x + (lkPoint[3].x - lkPoint[5].x) / 2.0;
		lkCenter2.y = lkPoint[3].y + (lkPoint[5].y - lkPoint[3].y) / 2.0;
		cv::circle(imgSrc, lkCenter2, 200, cv::Scalar(255, 0, 255), 4);
	}


	//把量块中心点1作为量块中心点
	lkCenter = lkCenter1;



	cv::waitKey(0);
	cv::destroyAllWindows();
}