
/*
This code is intended for academic use only.
You are free to use and modify the code, at your own risk.

This is used for building dataSet.
Some changes must be made if you wanna to make your own dataSet.

if you have any problems, feel free to contect me.

*/

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

#include "EllipseDetectorYaed.h"


void findEllipse(int id)
{
	freopen("./Images/00013/GT-00000.csv","w",stdout);
	printf("Filename;Width;Height;Roi.X1;Roi.Y1;Roi.X2;Roi.Y2;ClassId\n");
	VideoCapture cap("9.avi");
	if(!cap.isOpened()){
		cout<<"cannot open"<<endl;
		return ;
	}

	Mat frame;
	cap>>frame;
	
	//Mat frame;
	//frame=imread(path);
	//Mat frame1=frame.clone();
	Size sz=frame.size();

	

	//paramters settings 
	int iThLength=16;
	float fThObb=3.0f;
	float fThPos=1.0f;
	float fTaoCenters=0.05f;
	int iNs=16;
	float fMaxCenterDistance= sqrt(float(sz.width*sz.width+sz.height*sz.height))*fTaoCenters;
	float fThScoreScore=0.5f;

	//Gaussian filter paramters in pre-processing
	Size szPreProcessingGaussKernelSize=Size(5,5);
	double dPreProcessingGaussSigma=1.0f;
	float fDistanceToEllipseContour=0.1f;
	//const paramters to discard bad ellipses
	float fMinReliability=0.4f;
	CEllipseDetectorYaed * yaed=new CEllipseDetectorYaed();

		//initialize Dector with sel
		yaed->SetParameters(szPreProcessingGaussKernelSize,
			dPreProcessingGaussSigma,
			fThPos,
			fMaxCenterDistance,
			iThLength,
			fThObb,
			fDistanceToEllipseContour,
			fThScoreScore,
			fMinReliability,
			iNs
			);
	
	int count=225;
	char buffer[100];
	while(1){	
		//Detect
		if(id==1){
			cap>>frame;
			int minX,minY,maxX,maxY;

			minX=100;
			minY=200;
			maxX=220;
			maxY=250;

			Mat result;
			result=Mat::zeros(maxX-minX+1,maxY-minY+1, CV_8UC3);

			for(int i=minX;i<=maxX;i++){
				uchar *src = frame.ptr<uchar>(i);  
				uchar *dst = result.ptr<uchar>(i-minX);  
		    	for(int j=minY;j<= maxY;j++){
					dst[3*(j-minY)] = src[3*j];
					dst[3*(j-minY)+1]=src[3*j+1];
					dst[3*(j-minY)+2]=src[3*j+2];
		    		//result.at<uchar>(i- minX,j -minY)=image.at<uchar>(i,j);
		    	}
		    }

		    resize(result,result,Size(100,100));
			imshow("roi",result);
			sprintf(buffer,"./Images/00013/%d.jpg",count);
			imwrite(buffer,result);
			printf("%d.jpg;100;100;0;0;99;99;11\n",count);
			count++;
			//if(count>=1000) break;
			if(cvWaitKey(30)==27){
				continue;
			}
			continue;
		}
		std::vector<Ellipse> ellsYaed;

		//convert ro grayscale
		//cap>>frame;
		Mat gray;
		//Mat3b resultImage;
		cap>>frame;

		gray=Mat::zeros(frame.rows,frame.cols,CV_8UC1);
		cvtColor(frame,gray,CV_RGB2GRAY);

		Mat1b gray2;
		gray2=gray.clone();
		yaed->Detect(gray2,ellsYaed);

		//Mat3b resultImage=frame.clone();

		//yaed->DrawDetectedEllipses(resultImage,ellsYaed);
		//imshow("result",resultImage);
		//frame=imread(path);
		Mat roi= yaed->CutROI(frame,ellsYaed);
		
		if(!roi.empty()) {
			resize(roi,roi,Size(100,100));
			imshow("roi",roi);
			sprintf(buffer,"./Images/00013/%d.jpg",count);
			imwrite(buffer,roi);
			printf("%d.jpg;100;100;0;0;99;99;1\n",count);
			count++;
		}

		
		if(cvWaitKey(30)==27){
			continue;
		}
		
	}
	//<< neuralNetwork->predictSign(roi)<<endl;

//	imshow("result",resultImage);

	
	//waitKey(1000000);
}






int main(void){
	findEllipse(0);


}