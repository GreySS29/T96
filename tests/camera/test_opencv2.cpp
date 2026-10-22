#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::cout << "OpenCV version: "
              << CV_VERSION << std::endl;

    cv::Mat image(240, 320, CV_8UC3, cv::Scalar(0, 255, 0));
    cv::imwrite("green.jpg", image);

    std::cout << "Saved green.jpg\n";
    return 0;
}
