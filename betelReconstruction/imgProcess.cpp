#include "imgProcess.h"

void showImg(int width, int height, const char * windowName, const cv::Mat & src)
{
	cv::namedWindow(windowName, cv::WINDOW_NORMAL);
	cv::resizeWindow(windowName, width, height);
	cv::imshow(windowName, src);
}
