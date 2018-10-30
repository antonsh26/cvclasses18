/* Demo application for Computer Vision Library.
 * @file
 * @date 2018-09-05
 * @author Anonymous
 */

#include <cvlib.hpp>
#include <opencv2/opencv.hpp>

#include "utils.hpp"

void on_threshold_change(int value, void* ptr)
{
    auto& detector = *(cv::Ptr<cvlib::corner_detector_fast>*)(ptr);
    detector->setThreshold((uint8_t)(value));
}
void on_points_change(int value, void* ptr)
{
    auto& detector = *(cv::Ptr<cvlib::corner_detector_fast>*)(ptr);
    detector->setPoints((size_t)(value));
}
void put_points_text(cv::Mat image, size_t points)
{
    const auto fontScale = 0.5;
    const auto thickness = 1;
    const auto color = cv::Scalar(0, 255, 0);
    std::stringstream ss;
    ss << "points: " << points;
    cv::putText(image, ss.str(), cv::Point{2, 35}, CV_FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, 8, false);
}

int demo_corner_detector(int argc, char* argv[])
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;

    const auto main_wnd = "main";
    const auto demo_wnd = "demo";

    cv::namedWindow(main_wnd);
    cv::namedWindow(demo_wnd);

    cv::Mat frame;
    cv::Mat frame_gray;
    // auto detector = cv::GFTTDetector::create(); // \todo use cvlib::corner_detector_fast
    auto detector = cvlib::corner_detector_fast::create();
    std::vector<cv::KeyPoint> corners;

    int threshold = 40;
    cv::createTrackbar("threshold", demo_wnd, &threshold, 255, on_threshold_change, &detector);
    int points = 12;
    cv::createTrackbar("points", demo_wnd, &points, 16, on_points_change, &detector);

    utils::fps_counter fps;
    while (cv::waitKey(30) != 27) // ESC
    {
        cap >> frame;
        cv::imshow(main_wnd, frame);

        cv::cvtColor(frame, frame_gray, CV_BGR2GRAY);
        detector->detect(frame_gray, corners);

        cv::drawKeypoints(frame, corners, frame, cv::Scalar(0, 0, 255));
        utils::put_fps_text(frame, fps);
        put_points_text(frame, corners.size());
        cv::imshow(demo_wnd, frame);
    }

    cv::destroyWindow(main_wnd);
    cv::destroyWindow(demo_wnd);

    return 0;
}
