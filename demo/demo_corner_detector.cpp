/* Demo application for Computer Vision Library.
 * @file
 * @date 2018-09-05
 * @author Anonymous
 */

#include <cvlib.hpp>
#include <opencv2/opencv.hpp>

#include "utils.hpp"

static void put_points_text(cv::Mat image, size_t points)
{
    const auto fontScale = 0.5;
    const auto thickness = 1;
    const auto color = cv::Scalar(0, 255, 0);
    std::stringstream ss;
    ss << "points: " << points;
    cv::putText(image, ss.str(), cv::Point{2, 15}, CV_FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, 8, false);
}

static void threshold_change(int value, void* ptr)
{
    auto& detector = *(cv::Ptr<cvlib::corner_detector_fast>*)(ptr);
    detector->setThreshold(value);
}

int demo_corner_detector(int argc, char* argv[])
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;

    const auto main_wnd = "orig";
    const auto demo_wnd = "demo";

    cv::namedWindow(main_wnd);
    cv::namedWindow(demo_wnd);

    cv::Mat frame;
    auto detector = cvlib::corner_detector_fast::create();
    // auto detector = cv::FastFeatureDetector::create();
    std::vector<cv::KeyPoint> corners;

    int threshold = 40;
    cv::createTrackbar("threshold", main_wnd, &threshold, 255, threshold_change, &detector);

    utils::fps_counter fps;
    while (cv::waitKey(30) != 27) // ESC
    {
        cap >> frame;
        cv::imshow(main_wnd, frame);

        detector->detect(frame, corners);
        cv::drawKeypoints(frame, corners, frame, cv::Scalar(0, 0, 255));
        utils::put_fps_text(frame, fps);

        cv::imshow(demo_wnd, frame);
    }

    cv::destroyWindow(main_wnd);
    cv::destroyWindow(demo_wnd);

    return 0;
}
