#include <opencv2/opencv.hpp>
#include <iostream>
#include "ros/ros.h"
#include "std_msgs/String.h"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");
    ros::init(argc, argv, "demo");
    ros::NodeHandle n;
    ros::Publisher pos_pub = n.advertise<std_msgs::String>("pos", 1);

    VideoCapture capture(2);
    if (!capture.isOpened())
    {
        cout << "Cannot open camera" << endl;
        return -1;
    }

    Mat frame, hsv, mask;
    namedWindow("Camera", WINDOW_AUTOSIZE);
    ros::Rate rate(5);

    while (ros::ok())
    {
        capture >> frame;
        if (frame.empty())
        {
            cout << "Can't receive frame (stream end?). Exiting ..." << endl;
            break;
        }

        // 转换到HSV颜色空间
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        // 设定蓝色的HSV范围
        Scalar lower_blue(100, 150, 0);
        Scalar upper_blue(140, 255, 255);
        inRange(hsv, lower_blue, upper_blue, mask);

        // 应用高斯模糊
        GaussianBlur(mask, mask, Size(7, 7), 1.5);
        // 形态学操作去噪声
        erode(mask, mask, Mat());
        dilate(mask, mask, Mat());

        // 查找轮廓
        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

        std_msgs::String msg;
        std::stringstream Pos;
        int Pos_flag = 0;

        for (size_t i = 0; i < contours.size(); i++)
        {
            double area = contourArea(contours[i]);
            if (area > 2000) // 设定面积阈值
            {
                Moments m = moments(contours[i]);
                int cx = int(m.m10 / m.m00);
                int cy = int(m.m01 / m.m00);

                // 检查坐标是否在指定范围内
                if (abs(cx - 450) < 50 && abs(cy - 220) < 50)
                {
                    Pos_flag = 1;
                }

                // 绘制轮廓和中心
                drawContours(frame, contours, i, Scalar(0, 255, 0), 2);
                circle(frame, Point(cx, cy), 5, Scalar(255, 0, 0), -1);

                cout << "Center Coordinates: (" << cx << ", " << cy << ") - Pos_flag: " << Pos_flag << endl;
            }
        }

        if (Pos_flag)
        {
            Pos << "yes" << endl;
        }
        else
        {
            Pos << "no" << endl;
        }

        msg.data = Pos.str();
        ROS_INFO("%s", msg.data.c_str());
        pos_pub.publish(msg);

        imshow("Camera", frame);
        if (waitKey(30) == 'q')
            break;

        rate.sleep();
        ros::spinOnce();
    }

    capture.release();
    destroyAllWindows();
    return 0;
}
