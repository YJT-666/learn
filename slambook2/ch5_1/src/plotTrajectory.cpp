#include "../include/plotTrajectory.h"
#include <unistd.h>
#include <sophus/se3.hpp>
#include <cmath>



// read trajectory file
TrajectoryType* read_trajectory(const std::string& trajectoryPath){
	std::ifstream fin;
	fin.open(trajectoryPath);
	if(!fin){
		std::cout << "cannot find trajectory file at " << trajectoryPath << std::endl;
		return NULL;
	}
	TrajectoryType* poses = new TrajectoryType;
	double time, tx, ty, tz, qx, qy, qz, qw;
	while(!fin.eof()){
		fin >> time >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
		Eigen::Isometry3d Twr(Eigen::Quaterniond(qw, qx, qy, qz));
		Twr.pretranslate(Eigen::Vector3d(tx, ty, tz));
		poses->push_back(Twr);
	}
	std::cout << "read total " << poses->size() << " pose entries" << std::endl;
	fin.close();
	return poses;
}

double calculate_ATE_RMSE(TrajectoryType &groundtruth, TrajectoryType &estimated){
	double rmse = 0;
	int numOfPoses = groundtruth.size();
	if (numOfPoses != estimated.size()) {
		throw "the num of poses between groundtruth and estimated mismatching !";
	}
	for (int i = 0; i < numOfPoses; i++) {
		Sophus::SE3d p1(estimated[i].matrix()), p2(groundtruth[i].matrix());
		double error = (p2.inverse() * p1).log().norm();
		rmse += error * error;
	}
	rmse = rmse / static_cast<double>(estimated.size());
	rmse = sqrt(rmse);
	return rmse;
}


//  a simple gui init
SimpleGUI::SimpleGUI(){
  	// create pangolin window and plot the trajectory

	// Create a GUI window called "Trajectory Viewer" for display, with a window size of 1024x768 pixels
	pangolin::CreateWindowAndBind("Trajectory Viewer", 1024, 768);
	// enable depth test
    glEnable(GL_DEPTH_TEST);
	// enable color mix
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Create a camera observation view
    ps_cam = new pangolin::OpenGlRenderState(
	  // camera projection (Camera's image height, width, 4 internal parameters, and closest and farthest viewing distances)
      pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 389, 0.1, 1000),
	  // camera lookAt matrix
      pangolin::ModelViewLookAt(0, -0.1, -1.8, 0, 0, 0, 0.0, -1.0, 0.0)
    );

    pd_cam = &(pangolin::CreateDisplay());
	pd_cam->SetBounds(0.0, 1.0, 0.0, 1.0, -1024.0f / 768.0f);
	pd_cam->SetHandler(new pangolin::Handler3D(*ps_cam));
}
// 
void SimpleGUI::simpleGuiClearBuffer(){	
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pd_cam->Activate(*ps_cam);
}

// some opeartion of gui flash display buffer
void SimpleGUI::simpleGuiFinishFrame(){
	pangolin::FinishFrame();	
}

// draw a tarjectory in while loop
void SimpleGUI::drawTrajectory(TrajectoryType& tarjectory, Eigen::Vector4d &color, bool isDrawPose, int lienWidth){
        glLineWidth(lienWidth);
		
		if(isDrawPose){
			// Draw three coordinate axes for each pose
			for (size_t i = 0; i < tarjectory.size(); i++) {
				Eigen::Vector3d Ow = tarjectory[i].translation();
				Eigen::Vector3d Xw = tarjectory[i] * (0.1 * Eigen::Vector3d(1, 0, 0));
				Eigen::Vector3d Yw = tarjectory[i] * (0.1 * Eigen::Vector3d(0, 1, 0));
				Eigen::Vector3d Zw = tarjectory[i] * (0.1 * Eigen::Vector3d(0, 0, 1));
				glBegin(GL_LINES);
				glColor3f(1.0, 0.0, 0.0);
				glVertex3d(Ow[0], Ow[1], Ow[2]);
				glVertex3d(Xw[0], Xw[1], Xw[2]);
				glColor3f(0.0, 1.0, 0.0);
				glVertex3d(Ow[0], Ow[1], Ow[2]);
				glVertex3d(Yw[0], Yw[1], Yw[2]);
				glColor3f(0.0, 0.0, 1.0);
				glVertex3d(Ow[0], Ow[1], Ow[2]);
				glVertex3d(Zw[0], Zw[1], Zw[2]);
				glEnd();
			}
		}
    	// dram line
    	for (size_t i = 0; i < tarjectory.size(); i++) {
      		glColor4f(color(0), color(1), color(2), color(3));
      		glBegin(GL_LINES);
      		auto p1 = tarjectory[i], p2 = tarjectory[i + 1];
      		glVertex3d(p1.translation()[0], p1.translation()[1], p1.translation()[2]);
      		glVertex3d(p2.translation()[0], p2.translation()[1], p2.translation()[2]);
      		glEnd();
    	}

}

void SimpleGUI::drawPointCloud(PointCloudType& point_cloud, int point_size){
	glPointSize(2);
	glBegin(GL_POINTS);
	for(Eigen::Vector4d &p : point_cloud){
		glColor3f(p[3], p[3], p[3]);
		glVertex3d(p[0], p[1], p[2]);
	}
	glEnd();
}

