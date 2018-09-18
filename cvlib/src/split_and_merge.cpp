/* Split and merge segmentation algorithm implementation.
 * @file
 * @date 2018-09-05
 * @author Anonymous
 */

#include "cvlib.hpp"

namespace cvlib
{
bool mergeTwoRegion(region& parent, const cv::Mat& src, region& r1, region& r2, double stddev)
{
    if (r1.childs.size() == 0 && r2.childs.size() == 0)
    {
        cv::Rect roi1 = r1.roi;
        cv::Rect roi2 = r2.roi;
        cv::Rect roi12 = roi1 | roi2;

        cv::Mat mean;
        cv::Mat dev;
        cv::meanStdDev(src(roi12), mean, dev);

        if (dev.at<double>(0) <= stddev)
        {
            r1.roi = roi12;
            r2.validity = false;
            return true;
        }
    }
    return false;
}

void merge_image(const cv::Mat& src, region& r, double stddev)
{
    if (r.childs.size() == 0)
        return;

    if (!r.childs[0].validity && !r.childs[1].validity && !r.childs[2].validity && !r.childs[3].validity)
        return;

    bool row1 = mergeTwoRegion(r, src, r.childs[0], r.childs[1], stddev);
    bool row2 = mergeTwoRegion(r, src, r.childs[2], r.childs[3], stddev);

    if (!row1 && !row2)
    {
        bool col1 = mergeTwoRegion(r, src, r.childs[0], r.childs[2], stddev);
        bool col2 = mergeTwoRegion(r, src, r.childs[1], r.childs[3], stddev);

        if (col1 && col2)
        {
            bool row = mergeTwoRegion(r, src, r.childs[0], r.childs[1], stddev);
            if (row)
            {
                r.childs[0].validity = false;
                r.roi = r.childs[0].roi;
            }
        }
    }
    else if (row1 && row2)
    {
        bool col = mergeTwoRegion(r, src, r.childs[0], r.childs[2], stddev);
        if (col)
        {
            r.childs[0].validity = false;
            r.roi = r.childs[0].roi;
        }
    }

    for (auto& x : r.childs)
    {
        if (!x.validity)
            merge_image(src, x, stddev);
    }
}

region split_image(const cv::Mat& src, cv::Rect roi, double stddev)
{
    region r;
    r.roi = roi;
    r.validity = false;

    cv::Mat mean;
    cv::Mat dev;
    cv::meanStdDev(src, mean, dev);

    if (dev.at<double>(0) <= stddev)
    {
        r.validity = true;
        r.label = mean;
        return r;
    }

    const auto w = src.cols / 2;
    const auto h = src.rows / 2;
    region r1 = split_image(src(cv::Rect(0, 0, w, h)), cv::Rect(roi.x, roi.y, w, h), stddev);
    region r2 = split_image(src(cv::Rect(w, 0, w, h)), cv::Rect(roi.x + w, roi.y, w, h), stddev);
    region r3 = split_image(src(cv::Rect(0, h, w, h)), cv::Rect(roi.x, roi.y + h, w, h), stddev);
    region r4 = split_image(src(cv::Rect(w, h, w, h)), cv::Rect(roi.x + w, roi.y + h, w, h), stddev);
    r.childs.push_back(r1);
    r.childs.push_back(r2);
    r.childs.push_back(r3);
    r.childs.push_back(r4);

    return r;
}

void draw(cv::Mat& img, region& r)
{
    if (r.validity == true && r.childs.size() == 0)
    {
        img(r.roi).setTo(r.label);
        cv::rectangle(img, r.roi, 50, .1);
        return;
    }

    for (auto& x : r.childs)
        draw(img, x);
}

} // namespace cvlib
