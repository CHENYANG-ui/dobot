#include <ros/ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/Float32.h>

float camera_x = 0.0;
float camera_y = 0.0;
float camera_z = 0.0;
float grab_x = 0.0;
float grab_y = 0.0;

// 接收颜色信息的回调函数
void colorCallback(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO("接收到颜色: %s", msg->data.c_str());
}

// 接收中心坐标的回调函数
void centerCallback(const geometry_msgs::Point::ConstPtr& msg)
{
    // 将接收到的坐标值赋值给 camera_x, camera_y, camera_z
    camera_x = msg->x;
    camera_y = msg->y;
    camera_z = msg->z;

    // 计算抓取的 x 和 y
    grab_x = 255 + (camera_y - 350) / 5;
    grab_y = (camera_x - 650) / 5;

    ROS_INFO("接收到中心坐标: [x: %f, y: %f, z: %f]", camera_x, camera_y, camera_z);
    ROS_INFO("计算的抓取坐标: [grab_x: %f, grab_y: %f]", grab_x, grab_y);
}


// 接收旋转角度的回调函数
void angleCallback(const std_msgs::Float32::ConstPtr& msg)
{
    ROS_INFO("接收到旋转角度: %f", msg->data);
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "") ;
    // 初始化ROS节点
    ros::init(argc, argv, "color_info_receiver");
    ros::NodeHandle nh;

    // 订阅颜色、中心坐标和角度话题
    ros::Subscriber color_sub = nh.subscribe("detected_color", 1000, colorCallback);
    ros::Subscriber center_sub = nh.subscribe("detected_center", 1000, centerCallback);
    ros::Subscriber angle_sub = nh.subscribe("detected_angle", 1000, angleCallback);

    // 循环等待消息
    ros::spin();

    return 0;
}
