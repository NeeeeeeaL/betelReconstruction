#pragma once
#include "reconstruction.h"

void showImg(int width, int height, const char* windowName, const cv::Mat &src);

cv::Point2f blockExtraction(cv::Mat &imgSrc);

void betelExtraction(cv::Mat &imgSrc);