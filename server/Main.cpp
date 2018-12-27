/*
This code is intended for academic use only.
You are free to use and modify the code, at your own risk.

*/

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

#include "newServer.h"

using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
	newServer *runS = new newServer();
	runS->runServer();
	return 0;
}