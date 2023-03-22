#include <iostream>
#include <string>
#include <chrono>


#include <opencv2/core/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "include/plotTrajectory.h"

using namespace std;

const string left_file = "/home/smile/learn/slambook2/ch5_1/data/left.png";
const string right_file = "/home/smile/learn/slambook2/ch5_1/data/right.png";


int main(int argc, char** argv){
	
	double fx = 718.856, fy = 718.856, cx = 607.1928, cy = 185.2175;
	// baseline
	double b = 0.573;

	//
	cv::Mat left = cv::imread(left_file, 0);
	cv::Mat right = cv::imread(right_file, 0);
	cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(
		0, 96, 9, 8*9*9, 32*9*9, 1, 63, 10, 100, 32);
	cv::Mat disparity_sgbm, disparity;
	sgbm->compute(left, right, disparity_sgbm);
	disparity_sgbm.convertTo(disparity, CV_32F, 1.0/16.0f);

	 // 生成点云
    PointCloudType pointcloud;

    // 如果你的机器慢，请把后面的v++和u++改成v+=2, u+=2
    for (int v = 0; v < left.rows; v++)
        for (int u = 0; u < left.cols; u++) {
            if (disparity.at<float>(v, u) <= 0.0 || disparity.at<float>(v, u) >= 96.0) continue;

            Eigen::Vector4d point(0, 0, 0, left.at<uchar>(v, u) / 255.0); // 前三维为xyz,第四维为颜色

            // 根据双目模型计算 point 的位置
            double x = (u - cx) / fx;
            double y = (v - cy) / fy;
            double depth = fx * b / (disparity.at<float>(v, u));
            point[0] = x * depth;
            point[1] = y * depth;
            point[2] = depth;

            pointcloud.push_back(point);
        }

    cv::imshow("disparity", disparity / 96.0);
    cv::waitKey(0);
    // 画出点云
	SimpleGUI simpleGui;

	while(!simpleGui.simpleGuiShouldQuit()){
		simpleGui.simpleGuiClearBuffer();
		simpleGui.drawPointCloud(pointcloud);
		simpleGui.simpleGuiFinishFrame();
	}


	return 0;
}

