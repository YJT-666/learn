#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "include/plotTrajectory.h"

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

const string trajectory_file = "/home/smile/learn/slambook2/ch3_1/data/trajectory.txt";


int main(){
		
	TrajectoryType* trajectory = read_trajectory(trajectory_file);
	SimpleGUI simpleGui;

	while(!simpleGui.simpleGuiShouldQuit()){
		simpleGui.simpleGuiClearBuffer();
		Eigen::Vector4d color(1.0, 1.0, 1.0, 1.0);
		simpleGui.drawTrajectory(*trajectory, color, false);
		simpleGui.simpleGuiFinishFrame();
	}
	
	return 0;
}

