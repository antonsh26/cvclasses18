/* Split and merge segmentation algorithm implementation.
 * @file
 * @date 2018-09-18
 * @author Anonymous
 */

#include "cvlib.hpp"

#include <iostream>

namespace cvlib
{
void motion_segmentation::apply(cv::InputArray _image, cv::OutputArray _fgmask, double learningRate)
{
    if (learningRate < 0 || learningRate > 1)
        learningRate = 0.05;

    cv::Mat frame = _image.getMat();
    cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

    if (init == false)
    {
        frame.copyTo(ut_);
        ut_.convertTo(ut_, CV_32F);
        bt_ = cv::Mat::zeros(frame.size(), CV_32F);
        _fgmask.assign(ut_);
        init = true;
        return;
    }

    cv::accumulateWeighted(frame, ut_, learningRate);

    cv::Mat diff;
    frame.convertTo(frame, CV_32F);
    cv::absdiff(frame, ut_, diff);

    cv::multiply(diff, diff, diff);
    diff.convertTo(diff, CV_8U);
    cv::absdiff(ut_, frame, diff);

    cv::accumulateWeighted(diff, bt_, learningRate);
    cv::sqrt(bt_, bt_);
    cv::divide(diff, bt_, diff);
    cv::threshold(diff, diff, T, 255, cv::THRESH_BINARY);

    _fgmask.assign(diff);
}

cv::Ptr<motion_segmentation> createBackgroundSubtractor1G()
{
    return cv::makePtr<motion_segmentation>();
}

} // namespace cvlib
