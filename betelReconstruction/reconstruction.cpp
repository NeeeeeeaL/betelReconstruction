#include "reconstruction.h"
#include "imgProcess.h"

int main()
{
	cv::Mat imgSrc = cv::imread("betel-wx.bmp", 0);

	cv::Mat imgThreshold = cv::Mat::zeros(imgSrc.rows, imgSrc.cols, CV_8U);
	cv::threshold(imgSrc, imgThreshold, 15, 255, CV_THRESH_BINARY);


	cv::waitKey(0);
	cv::destroyAllWindows();
}