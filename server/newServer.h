#include <sys/types.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/utsname.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

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
#include "hsiModel.h"
#include "neuralNet.h"

using namespace std;
using namespace cv;

class newServer
{
public:
	newServer(){};
	~newServer(){};
	int runServer(void);
	
};