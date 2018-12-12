/* Demo application for Computer Vision Library.
 * @file
 * @date 2018-11-25
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

static void ratio_change(int value, void* ptr)
{
    auto& matcher = *(cv::Ptr<cvlib::descriptor_matcher>*)(ptr);
    matcher->setRatio(value / 10.0f);
}

int demo_feature_matching(int argc, char* argv[])
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;

    const auto main_wnd = "orig";
    const auto demo_wnd = "demo";

    cv::namedWindow(main_wnd);
    cv::namedWindow(demo_wnd);

    //auto detector = cvlib::corner_detector_fast::create();
    auto detector = cv::ORB::create();
    //auto detector = cv::FastFeatureDetector::create();
    auto matcher = cv::BFMatcher();
    //auto matcher = cv::FlannBasedMatcher();
    //auto matcher = cvlib::descriptor_matcher();

    int ratio = 15;
    int max_distance = 100;
    int threshold = 40;
    cv::createTrackbar("max distance", main_wnd, &max_distance, 500);
    cv::createTrackbar("ratio", main_wnd, &ratio, 30, ratio_change, &matcher);
    cv::createTrackbar("threshold", main_wnd, &threshold, 255, threshold_change, &detector);

    /// \brief helper struct for tidy code
    struct img_features
    {
        cv::Mat img;
        std::vector<cv::KeyPoint> corners;
        cv::Mat descriptors;
    };

    img_features ref;
    img_features test;
    std::vector<std::vector<cv::DMatch>> pairs;

    cv::Mat frame_gray;
    cv::Mat main_frame;
    cv::Mat demo_frame;
    utils::fps_counter fps;
    int pressed_key = 0;
    while (pressed_key != 27) // ESC
    {
        cap >> test.img;

        cv::cvtColor(test.img, frame_gray, CV_BGR2GRAY);
        //cv::GaussianBlur(frame_gray, frame_gray, cv::Size(5, 5), 0);

        detector->detect(frame_gray, test.corners);
        cv::drawKeypoints(test.img, test.corners, main_frame);
        cv::imshow(main_wnd, main_frame);

        pressed_key = cv::waitKey(30);
        if (pressed_key == ' ') // space
        {
            ref.img = test.img.clone();
            detector->detectAndCompute(ref.img, cv::Mat(), ref.corners, ref.descriptors);
        }

        if (ref.corners.empty())
        {
            continue;
        }

        detector->compute(test.img, test.corners, test.descriptors);
        matcher.radiusMatch(test.descriptors, ref.descriptors, pairs, max_distance);
        cv::drawMatches(test.img, test.corners, ref.img, ref.corners, pairs, demo_frame);

        utils::put_fps_text(demo_frame, fps);
        cv::imshow(demo_wnd, demo_frame);
    }

    cv::destroyWindow(main_wnd);
    cv::destroyWindow(demo_wnd);

    return 0;
}