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

int main(void){

	VideoCapture cap(1);

	if(!cap.isOpened()){
		cout<<"cannot open"<<endl;
		return 0;
	}
	Mat frame;
	cap>>frame;
	


	VideoWriter writer("9.avi",CV_FOURCC('P','I','M','1'),30,frame.size());

	while(1){
		cap>>frame;
		writer<<frame;
		imshow("video",frame);
		if(cvWaitKey(20)==27){
			break;
		}
	}

	return 0;
}