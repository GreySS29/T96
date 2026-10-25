#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::string pipeline =
    "libcamerasrc ! "
    "video/x-raw,width=640,height=480,framerate=30/1 ! "
    "videoconvert ! "
    "video/x-raw,format=GRAY8 ! "
    "videoflip method=clockwise ! "
    "appsink drop=true max-buffers=1";

    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

    if (!cap.isOpened()) {
        std::cerr << "Camera open failed\n";
        return 1;
    }

    cv::Mat cam;
	cv::Mat binary;

    while (true) {
     
        cap >> cam;

        if (cam.empty()) {
            std::cerr << "Empty\n";
            break;
        }


	cv::GaussianBlur(cam, cam, cv::Size(5,5),0) ;
	
	//binary
	double usedThreshold = cv::threshold(cam,binary, 0,255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
	cv::morphologyEx(binary,binary, cv::MORPH_OPEN, kernel); 
	cv::morphologyEx(binary,binary, cv::MORPH_CLOSE, kernel);
	

        cv::imshow("Gray", cam);
	cv::imshow("Vision", binary);


        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
