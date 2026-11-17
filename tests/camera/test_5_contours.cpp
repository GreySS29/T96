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

    // ---- параметры для подбора ----
    const double ROI_TOP_FRAC   = 0.4;   // верхние 40% кадра игнорируем
    const double MIN_CONTRAST   = 40.0;  // мин. разница яркости фон/преграда
    const double MIN_AREA       = 800.0; // мин. площадь преграды, px
    const double NEAR_FRAC      = 0.85;  // нижний край преграды ниже этой доли ROI = "близко"
    // --------------------------------

    cv::Mat cam, blurred, binary, inv, display;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));

    while (true) {
        cap >> cam;
        if (cam.empty()) { std::cerr << "Empty\n"; break; }

        const int W = cam.cols, H = cam.rows;
        const int roiY = static_cast<int>(H * ROI_TOP_FRAC);
        const cv::Rect roiRect(0, roiY, W, H - roiY);

        cv::GaussianBlur(cam, blurred, cv::Size(5, 5), 0);
        cv::Mat roi = blurred(roiRect);

        // тёмное -> белое
        cv::threshold(roi, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

        // защита от ложных срабатываний на пустом полу
        cv::bitwise_not(binary, inv);
        double darkMean  = cv::mean(roi, binary)[0];
        double lightMean = cv::mean(roi, inv)[0];
        if (lightMean - darkMean < MIN_CONTRAST) {
            binary.setTo(0);
        }

        cv::morphologyEx(binary, binary, cv::MORPH_OPEN,  kernel);
        cv::morphologyEx(binary, binary, cv::MORPH_CLOSE, kernel);

        std::vector<std::vector<cv::Point>> contours;
        // смещение, чтобы координаты были в системе полного кадра
        cv::findContours(binary, contours, cv::RETR_EXTERNAL,
                         cv::CHAIN_APPROX_SIMPLE, cv::Point(0, roiY));

        cv::cvtColor(cam, display, cv::COLOR_GRAY2BGR);
        cv::line(display, {0, roiY}, {W, roiY}, cv::Scalar(255, 0, 0), 1);
        cv::line(display, {W / 3, roiY}, {W / 3, H}, cv::Scalar(255, 0, 0), 1);
        cv::line(display, {2 * W / 3, roiY}, {2 * W / 3, H}, cv::Scalar(255, 0, 0), 1);

        bool zoneBlocked[3] = {false, false, false};  // лево, центр, право
        bool zoneNear[3]    = {false, false, false};

        for (const auto& c : contours) {
            double area = cv::contourArea(c);
            if (area < MIN_AREA) continue;

            cv::Rect box = cv::boundingRect(c);
            int cx = box.x + box.width / 2;
            int zone = std::min(2, cx * 3 / W);

            // насколько низко в ROI находится нижний край (0..1), больше = ближе
            double bottomFrac = double(box.y + box.height - roiY) / (H - roiY);
            bool isNear = bottomFrac > NEAR_FRAC;

            zoneBlocked[zone] = true;
            if (isNear) zoneNear[zone] = true;

            cv::Scalar color = isNear ? cv::Scalar(0, 0, 255) : cv::Scalar(0, 255, 0);
            cv::rectangle(display, box, color, 2);
            cv::putText(display, isNear ? "NEAR" : "obst",
                        {box.x, std::max(box.y - 5, 15)},
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
        }

        const char* names[3] = {"LEFT", "CENTER", "RIGHT"};
        for (int i = 0; i < 3; ++i) {
            if (zoneBlocked[i])
                std::cout << names[i] << (zoneNear[i] ? " NEAR  " : " far  ");
        }
        std::cout << '\n';

        cv::imshow("Camera", display);
        cv::imshow("Binary", binary);
        if (cv::waitKey(1) == 'q') break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
