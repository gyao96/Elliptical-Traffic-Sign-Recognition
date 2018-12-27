#include "hsiModel.h"


#define PI 3.1416  
#define min(a,b) (a<b?a:b)  
#define HMIN 0.027
#define HMAX 0.833
#define SMIN 0.12
#define IMIN 0.2


Mat hsiModel::rgb2hsi(Mat &image,Mat &hsi){ 
	Mat result;
	hsi=Mat::zeros(image.rows,image.cols,CV_8UC1);
    if(!image.data){  
        cout<<"Miss Data"<<endl;  
		return result;  
    }  
    int xMax=-1,xMin=1e7,yMax=-1,yMin=1e7;
    int oldXMax,oldYMax,oldYMin,oldXMin;
    int nl = image.rows;  
    int nc = image.cols;  
    if(image.isContinuous()){  
        nc = nc*nl;  
        nl = 1;  
    }  
    for(int i = 0;i < nl;i++){  
        uchar *src = image.ptr<uchar>(i);  
		cout<<i<<endl;
       // uchar *dst = hsiModel.ptr<uchar>(i);  
        for(int j = 0;j < nc;j++){  
            float b = src[j*3]/255.0;  
            float g = src[j*3+1]/255.0;  
            float r = src[j*3+2]/255.0;  
            float num = (float)(0.5*((r-g)+(r-b)));  
            float den = (float)sqrt((r+g)*(r+g)+(r-b)*(g-b));  
            float H,S,I;  
            if(den == 0){  
                H = 0;  
            }  
            else{  
                float theta = acos(num/den);  
                if(b <= g)  
                    H = theta/(PI*2);  
                else  
                    H = (2*PI - theta)/(2*PI);  
            }
            float minRGB = min(min(r,g),b);  
            den = r+g+b;  
            if(den == 0)   
                S = 0;  
            else  
                S = 1 - 3*minRGB/den;  
            I = den/3.0;  
            
          //  dst[3*j] = H*255;  
         //   dst[3*j+1] = S*255;  
          //  dst[3*j+2] = I*255;  

			//if((H<=HMIN||H>=HMAX)&&S>SMIN&&I>IMIN)
            if(H<=HMIN||H>=HMAX)
			   //cout<<i<<endl;
				hsi.at<uchar>(i,j)=255;				
            else
            	hsi.at<uchar>(i,j)=0;
        }  
    } 
    //medianBlur(hsiModel,hsiModel,3);
   // morphologyEx(hsiModel,hsiModel,MORPH_OPEN,Mat());
  
	//imshow("hsiModel",hsiModel);
	//waitKey(100000);

	int matchNum=0;
	for(int i=0;i<image.rows;i++){
		for(int j=0;j<image.cols;j++){
			if(hsi.at<uchar>(i,j)==255){
				if(i<xMin&&i>0) xMin=i;
            	if(i>xMax) xMax=i;
            	if(j<yMin&&j>0) yMin=j;
            	if(j>yMax) yMax=j;
				matchNum++;
			}
		}
	}

	if(matchNum<30){
		return result;
	}

	if(1.0*(xMax-xMin)/(yMax-yMin)>1.2||1.0*(xMax-xMin)/(yMax-yMin)<0.9){
		medianBlur(hsi,hsi,3);
		//imshow("hsiModel1",hsiModel);
		//waitKey(100000);
		matchNum=0;
		xMax=-1;
		xMin=1e7;
		yMax=-1;
		yMin=1e7;
		for(int i=0;i<image.rows;i++){
			for(int j=0;j<image.cols;j++){
				if(hsi.at<uchar>(i,j)==255){
					if(i<xMin&&i>0) xMin=i;
	            	if(i>xMax) xMax=i;
	            	if(j<yMin&&j>0) yMin=j;
	            	if(j>yMax) yMax=j;
					matchNum++;
				}
			}
		}
		if(matchNum<30)
			return result;
	}

	cout<<"xMax"<<xMax<<" xMin"<<xMin<<" yMax"<<yMax<<" yMin"<<yMin<<endl;


    result=Mat::zeros(xMax- xMin+1,yMax- yMin+1,CV_8UC3);

    for(int i=xMin;i<=xMax;i++){
		uchar *src = image.ptr<uchar>(i);  
		uchar *dst = result.ptr<uchar>(i-xMin);  
    	for(int j=yMin;j<= yMax;j++){
			dst[3*(j-yMin)] = src[3*j];
			dst[3*(j-yMin)+1]=src[3*j+1];
			dst[3*(j-yMin)+2]=src[3*j+2];
    		//result.at<uchar>(i- xMin,j -yMin)=image.at<uchar>(i,j);
    	}
    }
	return result;  
}  