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

    cv::Mat frame;
	cv::Mat blurred;

    while (true) {
        //one shot
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "Empty frame\n";
            break;
        }


        //  contours -> coord
	cv::GaussianBlur(frame,blurred, cv::Size(5,5),0) ;
	

        cv::imshow("Machine vision", frame);
	cv::imshow("Blurred", blurred);


        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
