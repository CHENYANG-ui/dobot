#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

// 函数用于判断主要颜色
std::string getDominantColor(int b, int g, int r) {
    if (r > g && r > b) return "RED";
    else if (g > r && g > b) return "GREEN";
    else return "BLUE";
}

// 根据主要颜色返回对应的文字颜色
cv::Scalar getTextcolor(const std::string& color) {
    if (color == "RED") return cv::Scalar(0, 0, 255); // RED对应红色
    else if (color == "GREEN") return cv::Scalar(0, 255, 0); // GREEN对应绿色
    else return cv::Scalar(255, 0, 0); // BLUE对应蓝色
}

int main() {
    cv::VideoCapture cap(2); // 0 是默认摄像头的ID

    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera" << std::endl;
        return -1;
    }

    cv::Mat frame;
    std::string dominantColor;
    cv::Scalar textColor;

    while (true) {
        cap >> frame; // 读取新的帧

        if (frame.empty()) {
            break;
        }

        // 取左上角的像素作为主要颜色
        cv::Vec3b pixel = frame.at<cv::Vec3b>(0, 0);
        int b = pixel[0];
        int g = pixel[1];
        int r = pixel[2];

        dominantColor = getDominantColor(b, g, r);
        textColor = getTextcolor(dominantColor);

        // 在左上角显示颜色名称
        cv::putText(frame, dominantColor, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, textColor, 2);

        cv::imshow("Frame", frame); // 显示图像

        char c = (char)cv::waitKey(25);
        if (c == 27) {
            break; // 按 'ESC' 退出
        }
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
