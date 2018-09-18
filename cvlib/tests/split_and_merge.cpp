/* Split and merge segmentation algorithm testing.
 * @file
 * @date 2018-09-05
 * @author Anonymous
 */

#include <catch2/catch.hpp>

#include "cvlib.hpp"

using namespace cvlib;

TEST_CASE("empty image", "[split_and_merge]")
{
    cv::Mat image;
    split_image(image, cv::Rect(0, 0, image.cols, image.rows), 1);
    REQUIRE(image.empty());
}

TEST_CASE("constant image", "[split_and_merge]")
{
    const cv::Mat image(100, 100, CV_8UC1, cv::Scalar{15});

    split_image(image, cv::Rect(0, 0, image.cols, image.rows), 1);
    REQUIRE(cv::Scalar(15) == cv::mean(image));
}

TEST_CASE("simple regions", "[split_and_merge]")
{
    SECTION("2x2")
    {
        // \todo
    }

    SECTION("3x3")
    {
        // \todo
    }
}

TEST_CASE("compex regions", "[split_and_merge]")
{
    SECTION("2x2")
    {
        // \todo
    }

    SECTION("3x3")
    {
        // \todo
    }

    SECTION("4x4")
    {
        // \todo
    }
}
