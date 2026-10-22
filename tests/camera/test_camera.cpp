#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::string pipeline =
        "libcamerasrc ! "
        "video/x-raw,width=640,height=480,framerate=30/1 ! "
        "videoconvert ! videoscale ! "
        "video/x-raw,format=BGR,width=640,height=480 ! "
        "appsink drop=true max-buffers=1";

    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

    if (!cap.isOpened()) {
        std::cerr << "Camera open failed\n";
        return 1;
    }

    cv::Mat frame;

    for (int i=0; i<1; ++i){
        cap >> frame;
	}

        if (frame.empty()) {
            std::cerr << "Empty frame\n";
            return 1;
        }

        cv::imwrite("frame.jpg", frame);
	std::cout << "save pic\n";

       

    cap.release();
    return 0;
}
