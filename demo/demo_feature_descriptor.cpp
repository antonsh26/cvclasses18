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
    cv::putText(image, ss.str(), cv::Point{2, 15}, CV_FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, 8, false);
}

void calculate_histogram(std::string wnd, const cv::Mat& descriptors, size_t width, size_t height)
{
    std::vector<double> distances;
    for (size_t i = 0; i < descriptors.rows; i++)
        for (size_t j = i + 1; j < descriptors.rows; j++)
            distances.push_back(cv::norm(descriptors.row(i), descriptors.row(j), cv::NORM_HAMMING));

    size_t hist_w = 10;
    auto hist = cv::Mat(1, hist_w, CV_32F, cv::Scalar(0));
    double max_d = *std::max_element(distances.begin(), distances.end());
    for (auto c : distances)
        hist.at<float>(c / max_d * hist_w)++;

    size_t bin_w = round((double)width / hist_w);
    cv::Mat histogram(height, width, CV_8U, cv::Scalar(0));
    cv::normalize(hist, hist, 0, height, cv::NORM_MINMAX, -1, cv::Mat());
    for (size_t i = 1; i < hist_w; i++)
        cv::rectangle(histogram, cv::Point2i(bin_w * (i - 1), height), cv::Point2i(bin_w * i, height - round(hist.at<float>(i - 1))), cv::Scalar(255),
                      cv::FILLED);

    cv::imshow(wnd, histogram);
}

int demo_feature_descriptor(int argc, char* argv[])
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;

    const auto main_wnd = "orig";
    const auto demo_wnd = "demo";

    cv::namedWindow(main_wnd);
    cv::namedWindow(demo_wnd);

    cv::Mat frame;
    cv::Mat frame_gray;
    auto detector_a = cvlib::corner_detector_fast::create();
    auto detector_b = cv::ORB::create();
    std::vector<cv::KeyPoint> corners;
    cv::Mat descriptors_a, descriptors_b;

    int threshold = 40;
    cv::createTrackbar("threshold", demo_wnd, &threshold, 255, on_threshold_change, &detector_a);
    int points = 12;
    cv::createTrackbar("points", demo_wnd, &points, 16, on_points_change, &detector_a);

    utils::fps_counter fps;
    int pressed_key = 0;
    while (pressed_key != 27) // ESC
    {
        cap >> frame;
        cv::imshow(main_wnd, frame);

        cv::cvtColor(frame, frame_gray, CV_BGR2GRAY);
        detector_a->detect(frame_gray, corners);

        cv::drawKeypoints(frame, corners, frame, cv::Scalar(0, 0, 255));

        utils::put_fps_text(frame, fps);
        put_points_text(frame, corners.size());
        cv::imshow(demo_wnd, frame);

        pressed_key = cv::waitKey(30);
        if (pressed_key == ' ') // space
        {
            cv::FileStorage file("descriptor.json", cv::FileStorage::WRITE | cv::FileStorage::FORMAT_JSON);

            detector_b->compute(frame, corners, descriptors_b);
            detector_a->compute(frame, corners, descriptors_a);

            file << detector_a->getDefaultName() << descriptors_a;
            file << "ORB" << descriptors_b;

            std::cout << "Dump descriptors complete! \n";

            calculate_histogram("histogram_a", descriptors_a, frame.cols, frame.rows);
            calculate_histogram("histogram_b", descriptors_b, frame.cols, frame.rows);
        }

        std::cout << "Feature points: " << corners.size() << "\r";
    }

    cv::destroyWindow(main_wnd);
    cv::destroyWindow(demo_wnd);

    return 0;
}
