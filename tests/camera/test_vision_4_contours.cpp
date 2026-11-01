#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::string pipeline =
        "libcamerasrc ! "
        "video/x-raw,width=640,height=480,framerate=30/1 ! "
        "videoconvert ! "
        "video/x-raw,format=GRAY8 ! "
        "videoflip method=clockwise ! "
        "appsink drop=true max-buffers=1 sync=false";

    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
    if (!cap.isOpened()) {
        std::cerr << "Camera open failed\n";
        return 1;
    }

    cv::Mat cam, blurred, binary, display;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    while (true) {
        cap >> cam;
        if (cam.empty()) {
            std::cerr << "Empty\n";
            break;
        }

        cv::GaussianBlur(cam, blurred, cv::Size(5, 5), 0);

        // for dark object on white: THRESH_BINARY_INV
        cv::threshold(blurred, binary, 0, 255,
                      cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
        cv::morphologyEx(binary, binary, cv::MORPH_OPEN, kernel);
        cv::morphologyEx(binary, binary, cv::MORPH_CLOSE, kernel);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(binary, contours, cv::RETR_EXTERNAL,
                         cv::CHAIN_APPROX_SIMPLE);

        cv::cvtColor(cam, display, cv::COLOR_GRAY2BGR);

        int largestIndex = -1;
        double largestArea = 0.0;
        const double frameArea = cam.cols * cam.rows;

        for (size_t i = 0; i < contours.size(); ++i) {
            double area = cv::contourArea(contours[i]);
            if (area > largestArea && area < 0.9 * frameArea) {
                largestArea = area;
                largestIndex = static_cast<int>(i);
            }
        }

        if (largestIndex >= 0 && largestArea > 500) {
            cv::Rect box = cv::boundingRect(contours[largestIndex]);
            int cx = box.x + box.width / 2;
            int cy = box.y + box.height / 2;

            cv::rectangle(display, box, cv::Scalar(0, 255, 0), 2);
            cv::circle(display, {cx, cy}, 5, cv::Scalar(0, 0, 255), -1);
            cv::putText(display,
                        "x=" + std::to_string(cx) + " y=" + std::to_string(cy),
                        {box.x, std::max(box.y - 10, 15)},
                        cv::FONT_HERSHEY_SIMPLEX, 0.5,
                        cv::Scalar(0, 255, 255), 1);

            std::cout << "center: " << cx << ", " << cy
                      << " | area: " << largestArea << '\n';
        } else {
            cv::putText(display, "No object", {10, 30},
                        cv::FONT_HERSHEY_SIMPLEX, 0.7,
                        cv::Scalar(0, 0, 255), 2);
        }

        cv::imshow("Camera", display);
        cv::imshow("Binary", binary);

        if (cv::waitKey(1) == 'q') break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}