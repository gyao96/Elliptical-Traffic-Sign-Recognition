/*
This code is intended for academic use only.
You are free to use and modify the code, at your own risk.
*/

#include "newServer.h"

neuralNet *neuralNetwork = new neuralNet();

int newServer::runServer(void){

	//create socket
	int server_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	struct sockaddr_in server_addr;
	pthread_t pid;
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr("192.168.1.102");
	server_addr.sin_port=htons(4806);

	if(bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
		printf("Bind Error!\n");
		return 0;
	}

	if(listen(server_socket,20)<0){
		printf("Listen Error!\n");
		return 0;
	}
	printf("ok!\n");

	
	struct sockaddr_in client_addr;
	socklen_t client_addr_size=sizeof(client_addr);
	char addr_ip[20];
	


	int client_sock=accept(server_socket,(struct sockaddr*)&client_addr,&client_addr_size);
	printf("one connected\n");
	
	char msg[1024];
	char rev[1024];
	//welcome!
	memset(rev,0,1024);
	memset(msg,0,1024);
	
	neuralNetwork->initNET();

	VideoCapture cap(0);
	if(!cap.isOpened()){
		cout<<"cannot open"<<endl;
		
	}

	Mat frame;
	cap>>frame;
	Size sz=frame.size();

	int predictNum;

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

	cout<<"pass"<<endl;

	while(1){

		std::vector<Ellipse> ellsYaed;
		Mat gray;
		cap>>frame;

		gray=Mat::zeros(frame.rows,frame.cols,CV_8UC1);
		cvtColor(frame,gray,CV_RGB2GRAY);

		Mat1b gray2;
		gray2=gray.clone();
		yaed->Detect(gray2,ellsYaed);
		Mat roi= yaed->CutROI(frame,ellsYaed);	
		if(!roi.empty()) {
			imshow("roi",roi);
			cvWaitKey(30);
			predictNum = neuralNetwork->predictSign(roi);
			cout<< predictNum <<endl;

			sprintf(msg,"%d",predictNum);
			write(client_sock,msg,sizeof(msg));
			memset(msg,0,1024);
			if(cvWaitKey(20)==27){
				break;
			}
			
		}				
	}

}

