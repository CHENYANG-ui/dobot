2024.10.18 TIAN 编写

    cv_pkg 此功能包用于做视觉 

version_node.cpp 为最初版本 
对应dobot_ws的 test1.cpp （仅固定点识别抓取 用于测试）

look.cpp为后续版本，能够识别物块并发布颜色、中心坐标、旋转角度三个消息 
reseive.cpp为消息接受的测试
对应dobot_ws的 test12.cpp 和 test14.cpp

 - `cd cv_ws`（进入项目目录）
 - `catkin_make`（编译项目）



 连接摄像头
 - `roscore`（启动 ROS 主节点）
 - `rosrun cv_pkg look`（运行相机进行识别）


