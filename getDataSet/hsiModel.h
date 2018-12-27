
#pragma once

#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <cv.h>

using namespace std;
using namespace cv;



class hsiModel
{
public:
	Mat rgb2hsi(Mat &image,Mat &HsiModel);
};