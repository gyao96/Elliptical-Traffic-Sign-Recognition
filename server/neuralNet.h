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

class neuralNet
{
public:
	neuralNet(){};
	~neuralNet(){};
	int loadPCA(const string &file_name,cv::PCA& pca_);
	int predict(Mat & sample,CvANN_MLP& nnetwork);
	void read_dataset(char *filename, cv::Mat &data, cv::Mat &labels, cv::Mat &labs);
	void MatShuffle(Mat & dataset);
	void convertToVect(cv::Mat &img, Mat &  img_vect);
	void convert(cv::Mat &img,double pixelArray[]);
	void initNET();
	int predictSign(Mat &sign);
};