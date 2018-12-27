/*

This code is intended for academic use only.
You are free to use and modify the code, at your own risk.

If you use this code, or find it useful, please refer to the paper

*/


#include "neuralNet.h"

#define IMG_NEW_DIM     40
#define RESIZED_IMG_DIM     4800
#define NUM_OF_CLASSES     13
#define HIDDEN    300
/******************************************************************************/
 
#define DATA_SET_SIZE 5740       //Number of samples in  dataset
#define ATTRIBUTES 388  //Number of pixels per sample.16X16

#define TRAINING_SAMPLES 4500       //Number of samples in test dataset
#define TEST_SAMPLES 1204      //Number of samples in test dataset
#define CLASSES 13                  //Number of distinct labels.

//#define RAW_DATA_SET_4800  "dataset4800.txt"
#define SHUFFLE_DATA_SET_FILE_4800 "dataSetShuffle4800.yml"
#define SHUFFLE_DATA_SET_4800 "dataSetShuffle4800"
#define PCA_FILE "pca480.yml"
#define NEURAL_NET_FILE "neuralNet.xml"
#define NEURAL_NET "neuralNet" 

PCA pca;
cv::Mat layers(4,1,CV_32S);


// global variable to store de probabilities for each class on each sample
cv::Mat classificationResult(1, CLASSES, CV_64F);


// global variable for the raw dataset of  size 4800=40*40*3
cv::Mat dataset4800(DATA_SET_SIZE,ATTRIBUTES,CV_64F);

void neuralNet::initNET(){
	loadPCA(PCA_FILE,pca);

	layers.at<int>(0,0) = ATTRIBUTES;//input layer
	layers.at<int>(1,0)= 300;//hidden layer
	layers.at<int>(2,0)=100;
	//layers.at<int>(3,0)=100;
	layers.at<int>(3,0) =CLASSES;//output layer

    	
}


void neuralNet::convert(cv::Mat &img,double pixelArray[])
{
 

int k=0;



for(int i = 0; i < img.rows; i++)
{
    for(int j = 0; j < img.cols; j++)
    {
        
	
	img.at<Vec3b>(i, j)[0] = (int)pixelArray[k]; 

	k++;

	
    }
	
}

for(int i = 0; i < img.rows; i++)
{
    for(int j = 0; j < img.cols; j++)
    {
        	img.at<Vec3b>(i, j)[1] = (int)pixelArray[k]; 
	k++;

	
    }
	
}

for(int i = 0; i < img.rows; i++)
{
    for(int j = 0; j < img.cols; j++)
    {
        	img.at<Vec3b>(i, j)[2] = (int)pixelArray[k]; 

	k++;

	
    }
	
}


}



void neuralNet::convertToVect(cv::Mat &img, Mat &  img_vect)
{
 

int k=0;



for(int i = 0; i < img.rows; i++)
{
    for(int j = 0; j < img.cols; j++)
    {
        
	
	img_vect.at<double>(0,k) = img.at<Vec3b>(i, j)[0] ; 

	k++;

	
    }
	
}

for(int i = 0; i < img.rows; i++)
{
    for(int j = 0; j < img.cols; j++)
    {
        	img_vect.at<double>(0,k) = img.at<Vec3b>(i, j)[1] ;
	k++;

	
    }
	
}

for(int i = 0; i < img.rows; i++)
{
    for(int j = 0; j < img.cols; j++)
    {
        	img_vect.at<double>(0,k) = img.at<Vec3b>(i, j)[2] ; 

	k++;

	
    }
	
}


}






void neuralNet::MatShuffle(Mat & dataset)
{
	vector< Mat > vv;
	Mat res;

	for( int i=0; i<dataset.rows; i++)
	{
		vv.push_back( dataset.row(i).clone());

	}
	
	random_shuffle(vv.begin(), vv.end());
	
	for(int i=0; i<vv.size();  i++)
	{
		res.push_back( vv[i] ) ;
	}
	dataset = res.clone();
	
	cout << "  shuffle over  " << endl;
	
}

void neuralNet::read_dataset(char *filename, cv::Mat &data, cv::Mat &labels, cv::Mat &labs)
{
 
    Mat dataSet, classes;
    

    FileStorage fs(filename,FileStorage::READ);
    fs[SHUFFLE_DATA_SET_4800] >> dataSet ; fs.release();
	
    // randomly shuffle the matrix rows
    MatShuffle(dataSet);
	


    data = dataSet( Range::all(),Range(0,RESIZED_IMG_DIM) ).clone();
	labs = dataSet.col(RESIZED_IMG_DIM).clone();
	classes= labs;
	
	
	
	for(int i=0 ; i <  classes.rows ; i++)	
	{
		for( int j = 0; j < classes.cols ; j++) 
		{
			switch( (int)classes.at<double>(i,j) )
			{
			case 0 : labels.at<double>(i,0) = 1; break;
			case 1 : labels.at<double>(i,1) = 1; break;
			case 2 : labels.at<double>(i,2) = 1; break;
			case 3 : labels.at<double>(i,3) = 1; break;
			case 4 : labels.at<double>(i,4) = 1; break;
			case 5 : labels.at<double>(i,5) = 1; break;
			case 6 : labels.at<double>(i,6) = 1; break;
			case 7 : labels.at<double>(i,7) = 1; break;
			case 8 : labels.at<double>(i,8) = 1; break;
			case 9 : labels.at<double>(i,9) = 1; break;
			case 10 : labels.at<double>(i,10) = 1; break;
			case 11 : labels.at<double>(i,11) = 1; break;
			case 12 : labels.at<double>(i,12) = 1; break;
           
			default: break;
			}
		}
	}


    
}


int neuralNet::predict(Mat & sample,CvANN_MLP& nnetwork)
{
	
 
    nnetwork.predict(sample, classificationResult);
    /*The classification result matrix holds weightage  of each class. 
    we take the class with the highest weightage as the resultant class */

    // find the class with maximum weightage.
    int maxIndex = 0;
    double value=0.0;
    double maxValue=classificationResult.at<double>(0,0);
    for(int index=1;index<CLASSES;index++)
    {   value = classificationResult.at<double>(0,index);
        if(value>maxValue)
        {   maxValue = value;
            maxIndex=index;

        }
    }

       
	return maxIndex + 1;

} 



int neuralNet::loadPCA(const string &file_name,cv::PCA& pca_)
{
    FileStorage fs(file_name,FileStorage::READ);
    fs["mean"] >> pca_.mean ;
    fs["e_vectors"] >> pca_.eigenvectors ;
    fs["e_values"] >> pca_.eigenvalues ;
    fs.release();

}

int neuralNet::predictSign(Mat &sign){
	Mat test_img_vect(1,RESIZED_IMG_DIM,CV_64FC1);
	Mat resizedImg(IMG_NEW_DIM,IMG_NEW_DIM,CV_8UC3);
	resize(sign,resizedImg,resizedImg.size());
	convertToVect(resizedImg,test_img_vect);
	Mat test_img_388=pca.project(test_img_vect);
	CvANN_MLP nnetwork(layers, CvANN_MLP::SIGMOID_SYM,1,1);
	nnetwork.load(NEURAL_NET_FILE,NEURAL_NET);
	int pred;
	pred=predict(test_img_388,nnetwork);
	return pred;
	
}