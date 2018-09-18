/* Demo application for Computer Vision Library.
 * @file
 * @date 2018-09-05
 * @author Anonymous
 */

#include <cvlib.hpp>
#include <opencv2/opencv.hpp>

int main(int /*argc*/, char** /*argv*/)
{
    cv::VideoCapture cap(0);

    if (!cap.isOpened())
        return -1;

    cv::Mat frame;
    cv::Mat frame_gray;

    const auto origin_wnd = "origin";
    const auto split_wnd = "split";
    const auto merge_wnd = "merge";

    int stddev = 50;
    cv::namedWindow(split_wnd, CV_GUI_EXPANDED);
    cv::createTrackbar("stdev", split_wnd, &stddev, 50);

    while (cv::waitKey(30) != 27) // ESC
    {
        cap >> frame;

        cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
        cv::imshow(origin_wnd, frame);

        cvlib::region r = cvlib::split_image(frame_gray, cv::Rect(0, 0, frame_gray.cols, frame_gray.rows), stddev);

        cv::Mat imgSplit = frame_gray.clone();
        cvlib::draw(imgSplit, r);

        for (int i = 0; i < 10; i++)
            cvlib::merge_image(frame_gray, r, stddev);

        cv::Mat imgMerge = frame_gray.clone();
        cvlib::draw(imgMerge, r);

        cv::imshow(split_wnd, imgSplit);
        cv::imshow(merge_wnd, imgMerge);
    }

    return 0;
}
