/* Computer Vision Functions.
 * @file
 * @date 2018-09-05
 * @author Anonymous
 */

#ifndef __CVLIB_HPP__
#define __CVLIB_HPP__

#include <opencv2/opencv.hpp>

namespace cvlib
{
struct region
{
    std::vector<region> childs;
    bool validity;
    cv::Rect roi;
    cv::Mat label;
};

bool mergeTwoRegion(region& parent, const cv::Mat& src, region& r1, region& r2, double stddev);

void merge_image(const cv::Mat& src, region& r, double stddev);

region split_image(const cv::Mat& src, cv::Rect roi, double stddev);

void draw(cv::Mat& img, region& r);
} // namespace cvlib

#endif // __CVLIB_HPP__
