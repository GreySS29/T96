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

    //contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(binary,contours,hierarchy, cv::RETR_EXTERNAL, cv ::CHAIN_APPROX_SIMPLE);

    //cv::cvtColor(cam, display, cv::COLOR_GRAY2BGR); //for a frame

    int largestIndex = -1;
    double largestArea = 0.0;


    for (size_t i = 0; i < contours.size(); ++i) {
        double area = cv::contourArea(contours[i]);

        if (area > largestArea) {
            largestArea = area;
            largestIndex = static_cast<int>(i);
        }
    }

if (largestIndex >= 0 && largestArea > 500) {
        cv::Rect box = cv::boundingRect(contours[largestIndex]);

        int centerX = box.x + box.width / 2;
        int centerY = box.y + box.height / 2;

        cv::rectangle(cam, box, cv::Scalar(0, 255, 0), 2);
        cv::circle(cam, {centerX, centerY}, 5,
                    cv::Scalar(0, 0, 255), -1);

        cv::putText(cam,
            "x=" + std::to_string(centerX) +
            " y=" + std::to_string(centerY),
            {box.x, box.y - 10},
            cv::FONT_HERSHEY_SIMPLEX, 0.5,
            cv::Scalar(0, 255, 255), 1);

        std::cout << "center: " << centerX << ", " << centerY
                    << " | area: " << largestArea << '\n';
    } else {
        cv::putText(cam, "No object",
                    {10, 30},
                    cv::FONT_HERSHEY_SIMPLEX, 0.7,
                    cv::Scalar(0, 0, 255), 2);
    }
	

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
