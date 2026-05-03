#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

// 红色、黄色、蓝色和绿色的HSV颜色范围
Scalar lower_red(0, 127, 128);
Scalar higher_red(10, 255, 255);
Scalar lower_yellow(15, 140, 210);
Scalar higher_yellow(35, 255, 255);
Scalar lower_blue(90, 80, 150);
Scalar higher_blue(140, 240, 255);
Scalar lower_green(40, 90, 135);
Scalar higher_green(80, 220, 190);

int main(int argc, char** argv)
{
    // 打开默认摄像头（ID = 0）
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "错误：无法打开摄像头" << endl;
        return -1;
    }

    Mat frame, img_hsv;
    namedWindow("image", WINDOW_AUTOSIZE);

    while (true) {
        // 捕获每一帧
        cap >> frame;
        if (frame.empty()) {
            cerr << "错误：捕获到空帧" << endl;
            break;
        }

        // 转换为HSV颜色空间
        cvtColor(frame, img_hsv, COLOR_BGR2HSV);

        // 创建红色、黄色、蓝色、绿色的掩码
        Mat mask_red, mask_yellow, mask_blue, mask_green;
        inRange(img_hsv, lower_red, higher_red, mask_red);
        inRange(img_hsv, lower_yellow, higher_yellow, mask_yellow);
        inRange(img_hsv, lower_blue, higher_blue, mask_blue);
        inRange(img_hsv, lower_green, higher_green, mask_green);

        // 中值模糊，减少噪声
        medianBlur(mask_red, mask_red, 7);
        medianBlur(mask_yellow, mask_yellow, 7);
        medianBlur(mask_blue, mask_blue, 7);
        medianBlur(mask_green, mask_green, 7);

        // 查找轮廓
        vector<vector<Point>> cnts_red, cnts_yellow, cnts_blue, cnts_green;
        findContours(mask_red, cnts_red, RETR_EXTERNAL, CHAIN_APPROX_NONE);
        findContours(mask_yellow, cnts_yellow, RETR_EXTERNAL, CHAIN_APPROX_NONE);
        findContours(mask_blue, cnts_blue, RETR_EXTERNAL, CHAIN_APPROX_NONE);
        findContours(mask_green, cnts_green, RETR_EXTERNAL, CHAIN_APPROX_NONE);

        // 定义颜色映射
        vector<vector<Point>> contours_list[4] = {cnts_red, cnts_yellow, cnts_blue, cnts_green};
        Scalar colors[4] = {Scalar(0, 0, 255), Scalar(0, 255, 255), Scalar(255, 0, 0), Scalar(0, 255, 0)};
        string color_labels[4] = {"red", "yellow", "blue", "green"};

        // 为每种颜色的物体绘制轮廓和最小外接矩形
        for (int i = 0; i < 4; i++) {
            for (auto& cnt : contours_list[i]) {
                Rect rect = boundingRect(cnt); // 获取边界矩形
                RotatedRect minRect = minAreaRect(cnt); // 获取旋转矩形
                double area = contourArea(cnt); // 计算轮廓面积
                if (area < 2000) continue; // 忽略小区域

                // 绘制旋转矩形
                Point2f box[4];
                minRect.points(box);
                for (int j = 0; j < 4; j++) {
                    line(frame, box[j], box[(j + 1) % 4], colors[i], 3); // 绘制矩形的每一条边
                }

                // 在中心绘制十字架和点
                line(frame, Point(minRect.center.x + 10, minRect.center.y), Point(minRect.center.x - 10, minRect.center.y), colors[i], 2);
                line(frame, Point(minRect.center.x, minRect.center.y + 10), Point(minRect.center.x, minRect.center.y - 10), colors[i], 2);
                circle(frame, minRect.center, 3, colors[i], -1); // 绘制中心点

                // 显示颜色标签
                putText(frame, color_labels[i], Point(rect.x, rect.y - 5), FONT_HERSHEY_SIMPLEX, 0.7, colors[i], 2);

                // 显示中心坐标和角度
                stringstream ss;
                ss << "core: (" << minRect.center.x << ", " << minRect.center.y << ")"; // 显示核心坐标
                putText(frame, ss.str(), Point(minRect.center.x + 10, minRect.center.y - 20), FONT_HERSHEY_SIMPLEX, 0.5, colors[i], 2);

                stringstream ss_angle;
                ss_angle << "angle: " << minRect.angle << "°"; // 显示旋转角度
                putText(frame, ss_angle.str(), Point(minRect.center.x + 10, minRect.center.y + 20), FONT_HERSHEY_SIMPLEX, 0.5, colors[i], 2);
            }
        }

        // 显示结果帧
        imshow("image", frame);
        if (waitKey(1) == 'q') {
            break; // 按下 'q' 键退出
        }
    }

    cap.release(); // 释放摄像头
    destroyAllWindows(); // 关闭所有窗口
    return 0;
}
