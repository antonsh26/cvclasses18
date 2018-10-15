/* Demo application for Computer Vision Library.
 * @file
 * @date 2018-09-05
 * @author Anonymous
 */

#include <opencv2/opencv.hpp>

int demo_split_and_merge(int argc, char* argv[]); // lab 1
int demo_select_texture(int argc, char* argv[]); // lab 2
int demo_motion_segmentation(int argc, char* argv[]); // lab 3

int main(int argc, char* argv[])
{
    cv::namedWindow("main");
    cv::Mat help = cv::Mat::zeros(300, 500, CV_8UC3);

    // clang-format off
    auto putText = [help, x = 30](const std::string& text) mutable {
        cv::putText(help, text, cv::Point(30, x), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0), 1, CV_8U);
        x += 20;
    };
    // clang-format on

    putText("Press ESC to exit");
    putText("Press 1 for Lab 1 Demo (split and merge)");
    putText("Press 2 for Lab 2 Demo (texture segmentation)");
    putText("Press 3 for Lab 3 Demo (motion segmentation)");

    cv::imshow("main", help);

    while (true)
    {
        int key = cv::waitKey(0);

        switch (key)
        {
            case '1':
                demo_split_and_merge(argc, argv);
                break;
            case '2':
                demo_select_texture(argc, argv);
                break;
            case '3':
                demo_motion_segmentation(argc, argv);
                break;
            case 27: // ESC
                return 0;
        }
    }
}
