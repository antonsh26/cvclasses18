/* FAST corner detector algorithm implementation.
 * @file
 * @date 2018-10-16
 * @author Anonymous
 */

#include "cvlib.hpp"

namespace cvlib
{
// static
cv::Ptr<corner_detector_fast> corner_detector_fast::create()
{
    return cv::makePtr<corner_detector_fast>();
}

bool corner_detector_fast::testPixel(cv::Mat& image, cv::Point2i point)
{
    // clang-format off
    static const cv::Point2i test_points[16] = {
        {0, -3}, {0, 3}, {3, 0}, {-3, 0},
        {1, -3}, {2, -2}, {3, -1},
        {3, 1}, {2, 2}, {1, 3},
        {-1, 3}, {-2, 2}, {-3, 1},
        {-3, -1}, {-2, -2}, {-1, -3}
    }; // clang-format on

    size_t count = 0;
    for (size_t i = 0; count < points_, i < 16; ++i)
    {
        cv::Point2i test_point = point + test_points[i];

        if (std::abs(image.at<uint8_t>(test_point) - image.at<uint8_t>(point)) >= threshhold_)
            ++count;

        if (i == 3 && count < 3)
            return false;
    }

    return count >= points_;
}

void corner_detector_fast::detect(cv::InputArray image, CV_OUT std::vector<cv::KeyPoint>& keypoints, cv::InputArray /*mask = cv::noArray()*/)
{
    keypoints.clear();
    cv::Mat frame = image.getMat();
    for (int i = 3; i < frame.rows - 3; ++i)
        for (int j = 3; j < frame.cols - 3; ++j)
            if (testPixel(frame, cv::Point2i{j, i}))
                keypoints.emplace_back(cv::KeyPoint(j, i, 1));

    /*
    auto mat = cv::Mat_<uint8_t>(frame(cv::Range(3, frame.rows - 3), cv::Range(3, frame.cols - 3)));
    for (int i = 0; i < mat.rows; ++i)
        for (int j = 0; j < mat.cols; ++j)
            if ( testPixel(frame, cv::Point2i{j + 3, i + 3}) )
                keypoints.emplace_back(cv::KeyPoint(j + 3, i + 3, 1));
    */

    /*
    auto mat = cv::Mat_<uint8_t>(frame(cv::Range(3, frame.rows - 3), cv::Range(3, frame.cols - 3)));
    mat.forEach([&](uint8_t &p, const int* pos) ->
        void{ if (testPixel(frame, cv::Point2i{pos[1] + 3, pos[0] + 3}))
                keypoints.emplace_back(cv::KeyPoint(pos[1] + 3, pos[0] + 3, 1));
            });
    */
}
} // namespace cvlib
