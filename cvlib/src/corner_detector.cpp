/* FAST corner detector algorithm implementation.
 * @file
 * @date 2018-10-16
 * @author Anonymous
 */

#include "cvlib.hpp"

#include <ctime>

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
}

void corner_detector_fast::compute(cv::InputArray image, std::vector<cv::KeyPoint>& keypoints, cv::OutputArray descriptors)
{
    // clang-format off
    static const cv::Point2i test_points[49] = {
        {-3, -3}, {-3, -2}, {-3, -1}, {-3, 0}, {-3, 1}, {-3, 2}, {-3, 3}, 
        {-2, -3}, {-2, -2}, {-2, -1}, {-2, 0}, {-2, 1}, {-2, 2}, {-2, 3},
        {-1, -3}, {-1, -2}, {-1, -1}, {-1, 0}, {-1, 1}, {-1, 2}, {-1, 3},
        { 0, -3}, { 0, -2}, { 0, -1}, { 0, 0}, { 0, 1}, { 0, 2}, { 0, 3},
        { 1, -3}, { 1, -2}, { 1, -1}, { 1, 0}, { 1, 1}, { 1, 2}, { 1, 3},
        { 2, -3}, { 2, -2}, { 2, -1}, { 2, 0}, { 2, 1}, { 2, 2}, { 2, 3},
        { 3, -3}, { 3, -2}, { 3, -1}, { 3, 0}, { 3, 1}, { 3, 2}, { 3, 3},
    }; // clang-format on

    const int desc_length = 32;
    descriptors.create(static_cast<int>(keypoints.size()), desc_length, CV_8U);
    auto desc_mat = descriptors.getMat();
    desc_mat.setTo(0);

    cv::Mat blurred;
    cv::GaussianBlur(image, blurred, cv::Size(3, 3), 5.0, 5.0);

    cv::RNG rng;

    uint8_t* ptr = reinterpret_cast<uint8_t*>(desc_mat.ptr());
    for (const auto& pt : keypoints)
    {
        for (int i = 0; i < desc_length; ++i)
        {
            int r1 = rng.uniform(0, 50);
            int r2 = rng.uniform(0, 50);
            auto keypoint = cv::Point2i(pt.pt);
            cv::Point2i test_point1 = keypoint + test_points[r1];
            cv::Point2i test_point2 = keypoint + test_points[r2];
            *ptr = blurred.at<uint8_t>(test_point1) < blurred.at<uint8_t>(test_point2);
            ++ptr;
        }
    }
}

void corner_detector_fast::detectAndCompute(cv::InputArray image, cv::InputArray, std::vector<cv::KeyPoint>& keypoints, cv::OutputArray descriptors,
                                            bool /*= false*/)
{
    detect(image, keypoints);
    compute(image, keypoints, descriptors);
}
} // namespace cvlib
