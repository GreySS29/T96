#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::string pipeline =
    "libcamerasrc ! "
    "video/x-raw,width=640,height=480,framerate=30/1 ! "
    "videoconvert ! videoscale ! "
    "videoflip method=clockwise ! "
    "video/x-raw,format=BGR ! "
    "appsink drop=true max-buffers=1";

    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

    if (!cap.isOpened()) {
        std::cerr << "Camera open failed\n";
        return 1;
    }

    cv::Mat frame;

    while (true) {
        //one shot
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "Empty frame\n";
            break;
        }

        
        // grayscale -> threshold -> contours -> coord

        cv::imshow("Machine vision", frame);

      
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
