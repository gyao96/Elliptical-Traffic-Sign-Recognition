/*
This code is intended for academic use only.
You are free to use and modify the code, at your own risk.

The raw image data will be converted to DataSet(.yml)
With dim from 40*40 resized to 388.

Also data shuffling is operated.

if you have any problem, feel free to contect me.

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
using namespace std;
using namespace cv; 

#define IMG_NEW_DIM     40
#define RESIZED_IMG_DIM     4800
#define NUM_OF_CLASSES     13


#define ATTRIBUTES 388  //input



#define RAW_DATA_SET_4800  "dataset4800.txt"
#define SHUFFLE_DATA_SET_FILE_4800 "dataSetShuffle4800.yml"
#define SHUFFLE_DATA_SET_4800 "dataSetShuffle4800"
#define PCA_FILE "pca480.yml"


int trainingSample[NUM_OF_CLASSES] = {0,1,2,3,4,5,6,7,8,9,10,11,12}; 


void savePCA(const string &file_name,cv::PCA& pca_)
{
    FileStorage fs(file_name,FileStorage::WRITE);
    fs << "mean" << pca_.mean;
    fs << "e_vectors" << pca_.eigenvectors;
    fs << "e_values" << pca_.eigenvalues;
    fs.release();
}

// convert image Vector to image 3D matrix
void convert(cv::Mat &img,int pixelArray[])
{
 

	Mat_<cv::Vec3b>::iterator it = img.begin<cv::Vec3b>() ;
	Mat_<Vec3b>::iterator itend = img.end<Vec3b>() ;

	int k=0;



	for(int i = 0; i < img.rows; i++)
	{
	    for(int j = 0; j < img.cols; j++)
	    {
	        

		img.at<Vec3b>(i, j)[0] = pixelArray[k]; 

		k++;

		
	    }
		
	}

	for(int i = 0; i < img.rows; i++)
	{
	    for(int j = 0; j < img.cols; j++)
	    {
	        	img.at<Vec3b>(i, j)[1] = pixelArray[k]; 
		k++;

		
	    }
		
	}

	for(int i = 0; i < img.rows; i++)
	{
	    for(int j = 0; j < img.cols; j++)
	    {
	        	img.at<Vec3b>(i, j)[2] = pixelArray[k]; 

		k++;

		
	    }
		
	}


}

//  Function to convert 3D image matrix into a vector40*40*3->4800*1
void convertToPixelValueArray(cv::Mat &img,int pixelArray[])
{
 

	Mat_<cv::Vec3b>::iterator it = img.begin<cv::Vec3b>() ;
	Mat_<Vec3b>::iterator itend = img.end<Vec3b>() ;

	int k=0;



	for(int i = 0; i < img.rows; i++)
	{
	    for(int j = 0; j < img.cols; j++)
	    {
	        Vec3b bgr = img.at<Vec3b>(i, j);

		pixelArray[k]= (int)bgr[0]; 

		k++;

		
	    }
		
	}

	for(int i = 0; i < img.rows; i++)
	{
	    for(int j = 0; j < img.cols; j++)
	    {
	        Vec3b bgr = img.at<Vec3b>(i, j);

		pixelArray[k]= (int)bgr[1]; 

		k++;

		
	    }
		
	}

	for(int i = 0; i < img.rows; i++)
	{
	    for(int j = 0; j < img.cols; j++)
	    {
	        Vec3b bgr = img.at<Vec3b>(i, j);

		pixelArray[k]= (int)bgr[2]; 

		k++;

		
	    }
		
	}

}

// auxiliary function to build DataSet text file
string convertInt(int number, char * prefix = "" , char * suffix = "")
{
    stringstream ss;//create a stringstream
    ss << prefix << number << suffix;//add number to the stream
    return ss.str();//return a string with the contents of the stream
}


void loadDataSet(){
	string rowPath = "./Images";
	// the name of text file containing the final dataSet
	string outputfile = RAW_DATA_SET_4800;

	fstream dataSet(outputfile.c_str(),ios::out);
	int pixelVector[RESIZED_IMG_DIM];

	int countnum=0;
	// iterate through the different folder to fetch its images 
	for (int i=0 ; i< NUM_OF_CLASSES ; i++)
	{
		/* the next 4 are used to construct the path to .cvs file 
		   where are registered paths to road sign's images and 
		   other metadata
		*/
		//cout<<1<<endl;
		string  numFolder;
		if(trainingSample[i]>=10){
			numFolder  = convertInt(trainingSample[i], "000");
		}
		else{
			numFolder  = convertInt(trainingSample[i], "0000");
		}

		
		string folder = rowPath + "/" + numFolder;
		string csvFile = folder + "/" + "GT-" + numFolder + ".csv" ;
		std::ifstream file(csvFile.c_str());


		// string to fetch each line of the previous file  
		std::string line;
		int numeroLigne = 0;
		
		// iterate through the file and fetch each image's metadata
		while (std::getline(file, line))
		{
			numeroLigne ++;
			
			if(numeroLigne == 1) continue;
			std::replace(line.begin(), line.end(), ';', ' ');

  			std::istringstream iss(line);
  			string rawInfo[8];
			string cell;
			int k=0;
			
			// string to fetch the road sign path
			string imagePath;
			
			// integers to fetch other metadata ( exact location coordinates and image's classId )
			int RoiX1 ,  RoiY1, RoiX2, RoiY2, ClassId; 

  			while (iss >> cell)
  			{
				rawInfo[k] = cell;
				k++;				
 			}
			
			imagePath =  folder + "/"  +  rawInfo[0] ;
			RoiX1 = atoi(rawInfo[3].c_str());
			RoiY1 = atoi(rawInfo[4].c_str());
			RoiX2 = atoi(rawInfo[5].c_str());
			RoiY2 = atoi(rawInfo[6].c_str());
			ClassId = atoi(rawInfo[7].c_str());
		
			cout <<  imagePath <<  "   " <<  RoiX1 << "  " << RoiY1<<  "   " <<  RoiX2 << "  " << RoiY2 << "  "  << ClassId << "  "<< ++countnum<<endl;
				
			// loading img
			Mat img = imread( imagePath.c_str() , CV_LOAD_IMAGE_COLOR );
			
			//imshow("img",img);
			
			// cropping img to get the exact image
			Rect ROI(RoiX1, RoiY1, RoiX2 - RoiX1, RoiY2 - RoiY1);
						
			Mat croppedImg = img(ROI).clone();

			
			// resizing img to  standardize their sizes to 40*40*3
			Mat resizedImg(IMG_NEW_DIM,IMG_NEW_DIM,CV_8UC3) ;
			resize(croppedImg , resizedImg ,  resizedImg.size() );

			//  free memory 
			img.release();
			croppedImg.release();

			
			// matrix img  to vector img  ( 40*40*3  ----> 4800*1 )
			
			convertToPixelValueArray( resizedImg , pixelVector );

			for( int l=0 ; l < RESIZED_IMG_DIM ; l++)
			{	
				dataSet << pixelVector[l] << " ";
			}

			// save the dataSet in a file.
			dataSet << ClassId << "\n";
				
		}		
	}

	
	// close file pointer to free memory
	dataSet.close();
}

void shuffleDataSet(){
	// raw dataset file  8729(rows) * 4800(cols)  not yet shuffle  
	std::ifstream file(RAW_DATA_SET_4800);
	std::string line;
	
	Mat dataSet;
	int ligne =0;

	// vector of vector containing each line of the dataset file = each image pixels (1*4800)
	vector< vector<double> > vv;


	// iterates through the file to construct the vector vv
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		double n;
		int k = 0;
		double tab[ RESIZED_IMG_DIM +1 ];
		vector<double> v;



		while (iss >> n)
		{ 	
			if( k == RESIZED_IMG_DIM +1) break;
			//tab[k] = n;  
			v.push_back(n);
			k++;
		}


		//Mat img(1,RESIZED_IMG_DIM +1,CV_64F,tab);
		//dataSet.push_back(img);
		vv.push_back(v);
		ligne ++ ;
		
	}


	// finaly we can randomly shuffle the dataSet
	random_shuffle(vv.begin(), vv.end());

	cout<<vv.size()<<endl;
	
	
	// save the randomized dataSet back to a file
	for( int i=0; i < vv.size(); i++)
	{ 

		double* tab = &vv[i][0];
		Mat img(1,RESIZED_IMG_DIM +1,CV_64F,tab);
		dataSet.push_back(img);
	}
	FileStorage fs(SHUFFLE_DATA_SET_FILE_4800,FileStorage::WRITE);   
	fs<< SHUFFLE_DATA_SET_4800 << dataSet;
	fs.release(); 
}

void createPCACovMatrix(){
	Mat dataset;

    // load the shuffled dataSet  ( 8729(rows)  *  4800(cols) )  the last column for the image ClassId	
    FileStorage fs(SHUFFLE_DATA_SET_FILE_4800,FileStorage::READ);
    fs[SHUFFLE_DATA_SET_4800] >> dataset ;
    cout<<"cols: "<<dataset.cols<<" rows: "<<dataset.rows<<endl;
    //cout<<dataset(Range::all(),Range(4800,4801))<<endl;
    // exclude the ClassId before performing PCA
    Mat data = dataset(Range::all(), Range(0,4800));
    cout<<"cols: "<<data.cols<<" rows: "<<data.rows<<endl;

    PCA pca(data, Mat(), CV_PCA_DATA_AS_ROW , ATTRIBUTES);
  //  cout<<1<<endl;
   // save the model generated for  future uses.
    savePCA(PCA_FILE, pca); 
}

int main(int argc, char const *argv[])
{
	loadDataSet();
	cout<<"loadDataSet is OK!"<<endl;
	shuffleDataSet();
	cout<<"shuffleDataSet is OK!"<<endl;
	createPCACovMatrix();
	cout<<"createPCACovMatrix is OK!"<<endl;
	return 0;
}
