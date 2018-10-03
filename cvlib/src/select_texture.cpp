/* Split and merge segmentation algorithm implementation.
 * @file
 * @date 2018-09-18
 * @author Anonymous
 */

#include "cvlib.hpp"

namespace
{
struct descriptor : public std::vector<double>
{
    using std::vector<double>::vector;
    descriptor operator-(const descriptor& right) const
    {
        descriptor temp = *this;
        for (int i = 0; i < temp.size(); ++i)
        {
            temp[i] -= right[i];
        }
        return temp;
    }

    double norm_l1() const
    {
        double res = 0.0;
        for (auto v : *this)
        {
            res += std::abs(v);
        }
        return res;
    }

    double norm_l2() const
    {
        double res = 0.0;
        for (auto v : *this)
        {
            res += v * v;
        }
        return sqrt(res);
    }
};

const int num = 3;

void calculateResponse(const cv::Mat& image, int kernel_size, std::vector<cv::Mat>& sum, std::vector<cv::Mat>& sumsqr)
{
    const double th = CV_PI / 4;
    const double lm = 10.0;
    const double gm = 0.5;
    const std::vector<double> sig = {5, 10, 15};
    cv::Mat temp;

    for (int i = 0; i < num; i++)
    {
        cv::Mat kernel = cv::getGaborKernel(cv::Size(kernel_size, kernel_size), sig[i], th, lm, gm);
        cv::filter2D(image, temp, CV_32F, kernel);
        cv::integral(temp, sum[i], sumsqr[i]);
    }
}

void calculateDescriptor(const std::vector<cv::Mat>& sum, const std::vector<cv::Mat>& sumsqr, const cv::Rect& roi, int kernel_size, descriptor& descr)
{
    descr.clear();
    cv::Point lt(roi.x, roi.y);
    cv::Point rt(roi.x + roi.width, roi.y);
    cv::Point rb(roi.x + roi.width, roi.y + roi.height);
    cv::Point lb(roi.x, roi.y + roi.height);

    for (int i = 0; i < num; i++)
    {
        const cv::Mat& sum_image = sum[i];
        const cv::Mat& sumsqr_image = sumsqr[i];

        double mean = (sum_image.at<double>(lt) + sum_image.at<double>(rb) - sum_image.at<double>(rt) - sum_image.at<double>(lb));
        mean /= roi.area();
        double dev = sumsqr_image.at<double>(lt) + sumsqr_image.at<double>(rb) - sumsqr_image.at<double>(rt) - sumsqr_image.at<double>(lb);
        dev = std::sqrt(dev / roi.area() - mean * mean); 

        descr.emplace_back(mean);
        descr.emplace_back(dev);

    }
    
}
} // namespace

namespace cvlib
{
cv::Mat select_texture(const cv::Mat& image, const cv::Rect& roi, double eps)
{
    const int kernel_size = (std::min(roi.height, roi.width) + 2) / 2; // round to nearest odd

    //cv::Mat sum, sumsqr;
    std::vector<cv::Mat> sum(num);
    std::vector<cv::Mat> sumsqr(num);
    calculateResponse(image, kernel_size, sum, sumsqr);

    descriptor reference;
    calculateDescriptor(sum, sumsqr, roi, kernel_size, reference);

    cv::Mat res = cv::Mat::zeros(image.size(), CV_8UC1);

    descriptor test(reference.size());
    cv::Rect baseROI = roi - roi.tl();

    for (int i = 0; i < image.size().width - roi.width; ++i)
    {
        for (int j = 0; j < image.size().height - roi.height; ++j)
        {
            auto curROI = baseROI + cv::Point(i, j);
            calculateDescriptor(sum, sumsqr, curROI, kernel_size, test);

            res(curROI) = 255 * ((test - reference).norm_l2() <= eps);
        }
    }

    return res;
}
} // namespace cvlib
