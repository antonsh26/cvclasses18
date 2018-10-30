/* FAST corner detector algorithm testing.
 * @file
 * @date 2018-09-05
 * @author Anonymous
 */

#include <catch2/catch.hpp>

#include "cvlib.hpp"

using namespace cvlib;

TEST_CASE("simple check", "[corner_detector_fast]")
{
    auto fast = corner_detector_fast::create();
    cv::Mat image(10, 10, CV_8UC1, cv::Scalar(0));
    std::vector<cv::KeyPoint> out;

    SECTION("empty image")
    {
        fast->detect(image, out);
        REQUIRE(out.empty());
    }

    SECTION("one point")
    {
        image.at<uint8_t>(3, 3) = 255;
        fast->detect(image, out);
        REQUIRE(1 == out.size());
    }

    SECTION("two points")
    {
        image.at<uint8_t>(3, 3) = 255;
        image.at<uint8_t>(6, 6) = 255;
        fast->detect(image, out);
        REQUIRE(2 == out.size());
    }

    // clang-format off
    SECTION("no corner")
    {
        cv::Mat image = (cv::Mat_<uint8_t>(7, 7) <<
              0,    0,    0,    0,  255,    0,    0,    0,    0,
              0,    0,    0,    0,  255,    0,    0,    0,    0,
              0,    0,    0,    0,  255,    0,    0,    0,    0,
              0,    0,    0,    0,  255,    0,    0,    0,    0,
              0,    0,    0,    0,  255,    0,    0,    0,    0,
              0,    0,    0,    0,  255,    0,    0,    0,    0,
              0,    0,    0,    0,  255,    0,    0,    0,    0,
              0,    0,    0,    0,  255,    0,    0,    0,    0,
              0,    0,    0,    0,  255,    0,    0,    0,    0);
        fast->detect(image, out);
        REQUIRE(out.empty());
    }

    SECTION("one corner")
    {
        cv::Mat image = (cv::Mat_<uint8_t>(7, 7) <<
              0,    0,    0,    0,    0,    0,    0,    0,    0,
              0,    0,    0,    0,    0,    0,    0,    0,    0,
              0,    0,    0,    0,    0,    0,    0,    0,    0,
              0,    0,    0,    0,    0,    0,    0,    0,    0,
              0,    0,    0,    0,  255,    0,    0,    0,    0,
              0,    0,    0,  255,  255,  255,    0,    0,    0,
            255,  255,  255,  255,  255,  255,  255,  255,  255,
            255,  255,  255,  255,  255,  255,  255,  255,  255,
            255,  255,  255,  255,  255,  255,  255,  255,  255);
        fast->detect(image, out);
        REQUIRE(out.empty());
    }

    SECTION("no corners")
    {
        cv::Mat image = (cv::Mat_<uint8_t>(7, 7) <<
              0,    0,    0,    0,    0,    0,    0,    0,    0,
              0,    0,    0,    0,    0,    0,    0,    0,    0,
              0,    0,    0,    0,    0,    0,    0,    0,    0,
              0,    0,    0,    0,    0,  255,    0,    0,    0,
              0,    0,    0,  255,    0,    0,    0,    0,    0,
            255,  255,  255,  255,  255,  255,  255,  255,  255,
            255,  255,  255,  255,  255,  255,  255,  255,  255,
            255,  255,  255,  255,  255,  255,  255,  255,  255,
            255,  255,  255,  255,  255,  255,  255,  255,  255);

        fast->detect(image, out);
        REQUIRE(0 == out.size());
    }
}
