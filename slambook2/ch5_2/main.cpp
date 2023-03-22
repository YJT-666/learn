#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <boost/format.hpp>

#include <opencv2/opencv.hpp>

#include <sophus/se3.hpp>

#include "include/plotTrajectory.h"

using namespace std;

const string poses_file = "/home/smile/learn/slambook2/ch5_2/data/pose.txt";
const char* fmt_file_string = "/home/smile/learn/slambook2/ch5_2/data/%s/%d.%s";

int main(int argc, char** argv){

	vector<cv::Mat> colorImgs, depthImgs;    // 彩色图和深度图
	TrajectoryTypeSE3d poses;         // 相机位姿

	ifstream fin(poses_file);
	if(!fin) {
		cerr << "can't find pose.txt in the \"./data/pose.txt\" " << endl;
		return 1;
	}

	boost::format fmt(fmt_file_string);
	for (int i = 0; i <5; i++) {
		colorImgs.push_back(cv::imread( (fmt % "color" % (i+1) % "png").str() ) );
		depthImgs.push_back(cv::imread((fmt % "depth" % (i + 1) % "pgm").str(), cv::IMREAD_UNCHANGED));

		double data[7] = {0};
		for(auto &d : data) {
			fin >> d;  // tx ty tz qx qy qz qs
		}
		Sophus::SE3d pose(Eigen::Quaterniond(data[6], data[3], data[4], data[5]),
						  Eigen::Vector3d(data[0], data[1], data[2]));
		poses.push_back(pose);

	}

	// 计算点云并拼接
    // 相机内参
    double cx = 325.5;
    double cy = 253.5;
    double fx = 518.0;
    double fy = 519.0;
    double depthScale = 1000.0;

	PointCloudTypeRGB *pointCloud = new PointCloudTypeRGB;
	pointCloud->reserve(1000000);

	for(int i=0; i<5; i++){
		cout << "convert image: " << i + 1 << endl;
		cv::Mat color = colorImgs[i];
		cv::Mat depth = depthImgs[i];
		Sophus::SE3d T = poses[i];
		for(int v=0; v < color.rows; v++){
			for(int u=0; u <color.cols; u++){
				unsigned short d = depth.ptr<unsigned short>(v)[u]; // depth type is unsigned int
				if(d==0) continue; // d==0 represent no depth;
				Eigen:: Vector3d point;
				point[2] = double(d) / depthScale; //z
				point[1] = (v-cy) * point[2] /fy; // y
				point[0] = (u-cx) * point[2] /fx; // x
				Eigen::Vector3d pointWorld = T * point;

				Eigen::Vector6d p;
				p.head<3>() = pointWorld;
				p[5] = color.data[v * color.step + u * color.channels()]; //B
				p[4] = color.data[v * color.step + u * color.channels() + 1]; //G
				p[3] = color.data[v * color.step + u * color.channels() + 2]; //R
				// cout << p[3] << " " << p[4] << " " << p[5] << endl;
				pointCloud->push_back(p);
			}
		}
	}

	cout << "ponit cloud : " << pointCloud->size() << endl;

	SimpleGUI simpleGui;

	while(!simpleGui.simpleGuiShouldQuit()){
		simpleGui.simpleGuiClearBuffer();
		simpleGui.drawPointCloud(*pointCloud);
		simpleGui.simpleGuiFinishFrame();
	}


	return 0;
}

