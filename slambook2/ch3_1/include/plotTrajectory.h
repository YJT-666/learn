#ifndef PLOTRAJECTORY_H_
#define PLOTRAJECTORY_H_

#include <pangolin/pangolin.h>
#include <Eigen/Core>
#include <vector>
#include <string>


// type of trajectory
typedef std::vector<Eigen::Isometry3d, Eigen::aligned_allocator<Eigen::Isometry3d> > TrajectoryType;


// read trajectory file
// trajectory file format : time, tx,ty,tz, qx, qy, qz, qw (time translation rotate, world coord -> Robot coord)
// return a vector of Euclidean transformation matrix(Isometry3d)
TrajectoryType* read_trajectory(const std::string& trajectoryPath);

class SimpleGUI{
public:
	SimpleGUI();
	~SimpleGUI(){
		delete ps_cam;
	}

	bool simpleGuiShouldQuit(){
		return pangolin::ShouldQuit();
	}
	
	void simpleGuiClearBuffer();
	// some opeartion of gui flash display buffer
	void simpleGuiFinishFrame();
		

	// draw a tarjectory in while loop
	// tarjectory : a tarjectory
	// color : the color of tarjectory (RGBA)
	// isDrawPose : weather draw pose(axis of the point)
	// lienWidth : just lineWidth
	void drawTrajectory(TrajectoryType& tarjectory, Eigen::Vector4d &color, bool isDrawPose=true, int lienWidth=2);
	
	// attribute
	pangolin::OpenGlRenderState *ps_cam;
	pangolin::View *pd_cam;
};


 





#endif
