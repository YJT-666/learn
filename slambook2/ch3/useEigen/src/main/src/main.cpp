#include <iostream>
using namespace std;
#include <ctime>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

using namespace Eigen;

#define MATRIX_SIZE 50

void eigenMatrix(){
	Matrix<float, 2, 3> matrix_23;

	Vector3d v_3d;

	Matrix<float, 3, 1> vd_3d;

	Matrix3d matrix_33 = Matrix3d::Zero();

	Matrix<double, Dynamic, Dynamic> matrix_dynamic;

	MatrixXd matrix_x;

	// init
	matrix_23 << 1, 2, 3, 4, 5, 6;
	cout << "matrix 2x3 from 1 to 6: \n" << matrix_23 << endl;
	
	cout << "matrix_23[0,1]=" << matrix_23(0, 1) << endl;

	v_3d << 3, 2, 1;
	vd_3d << 4, 5, 5;

	Matrix<double, 2, 1> result = matrix_23.cast<double>() * v_3d;
	cout << matrix_23 << endl << "   *   " << endl << v_3d << endl << "   =   " << endl << result << endl;

	matrix_33 = Matrix3d::Random();
	cout << " random matrix: \n" << matrix_33 << endl;
	cout << " transpose:\n" << matrix_33.transpose() << endl;
	cout << " sum : " << matrix_33.sum() << endl;
	cout << " trace: " << matrix_33.trace() << endl;
	cout << " times 10:\n" << 10 * matrix_33 << endl;
	cout << " inverse:\n" << matrix_33.inverse() << endl;
	cout << " det: " << matrix_33.determinant() << endl;

}

void useGeometry(){
	Matrix3d rotation_matrix = Matrix3d::Identity();

	AngleAxisd rotation_vector(M_PI / 4, Vector3d(0, 0, 1));
	cout.precision(3);

	cout << "rotation matrix =\n" << rotation_vector.matrix() << endl;
	rotation_matrix = rotation_vector.toRotationMatrix();

	Vector3d v(1, 0, 0);
	Vector3d v_rotated = rotation_vector * v;
	cout << "(1, 0, 0) after rotation (by angle axis) = " << v_rotated.transpose() << endl;

	v_rotated = rotation_matrix * v;
	cout << "(1, 0, 0) after rotation (by rotation matrix) = " << v_rotated.transpose() << endl;

	Vector3d euler_angles = rotation_matrix.eulerAngles(2, 1, 0);
	cout << " yaw pitch roll " << euler_angles.transpose() << endl;

	Isometry3d T = Isometry3d::Identity();
	T.rotate(rotation_vector);
	T.pretranslate(Vector3d(1, 3, 4));
	cout << "Transform matrix = \n" << T.matrix() << endl;

	Vector3d v_transformed = T * v;
	cout << "v transformed = " << v_transformed.transpose() << endl;

	Quaterniond q = Quaterniond(rotation_vector);
	cout << "quaternion from rotation vector = " << q.coeffs().transpose() << endl;

	q = Quaterniond(rotation_matrix);
	cout << "quaternion from rotation matrix = " << q.coeffs().transpose() << endl;

	v_rotated = q * v;
	cout << "(1, 0, 0) after rotation = " << v_rotated.transpose() << endl;
	cout << "should be equal to " << (q * Quaterniond(0, 1, 0, 0) * q.inverse()).coeffs().transpose() << endl;
}

void coordinateTransform(){
	Quaterniond q1(0.35, 0.2, 0.3, 0.1), q2(-0.5, 0.4, -0.1, 0.2);

	q1.normalize();
	q2.normalize();
	Vector3d t1(0.3, 0.1, 0.1), t2(-0.1, 0.5, 0.3);
	Vector3d p1(0.5, 0, 0.2);

	Isometry3d T1w(q1), T2w(q2);
	T1w.pretranslate(t1);
	T2w.pretranslate(t2);

	Vector3d p2 = T2w * T1w.inverse() * p1;
	cout << endl << p2.transpose() << endl;

}

#include <pangolin/pangolin.h>
#include <eigen3/Eigen/Core>
#include <unistd.h>
#include <vector>
#include <fstream>

using namespace std;
using namespace Eigen;

string trajectory_file = "./trajectory.txt";


using Vector_Isometry3d = vector<Isometry3d, Eigen::aligned_allocator<Isometry3d> >;
void DrawTrajectory(Vector_Isometry3d poses){
	// create pangolin window and plot the trajectory
	pangolin::CreateWindowAndBind("Trajectory Viewer", 1024, 768);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	pangolin::OpenGlRenderState s_cam(
	pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 389, 0.1, 1000),
	pangolin::ModelViewLookAt(0, -0.1, -1.8, 0, 0, 0, 0.0, -1.0, 0.0)
	);

	pangolin::View &d_cam = pangolin::CreateDisplay()
		.SetBounds(0.0, 1.0, 0.0, 1.0, -1024.0f/768.0f)
		.SetHandler(new pangolin::Handler3D(s_cam));
	
	while (pangolin::ShouldQuit() == false){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		d_cam.Activate(s_cam);
		dlclearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glLineWidth(2);
		for ( size_t i = 0; i < poses.size(); i++){
			  Vector3d Ow = poses[i].translation();
			  Vector3d Xw = poses[i] * (0.1 * Vector3d(1, 0, 0));
			  Vector3d Yw = poses[i] * (0.1 * Vector3d(0, 1, 0));
			  Vector3d Zw = poses[i] * (0.1 * Vector3d(0, 0, 1));
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

			for (size_t i = 0; i < poses.size(); i++) {
			  glColor3f(0.0, 0.0, 0.0);
			  glBegin(GL_LINES);
			  auto p1 = poses[i], p2 = poses[i + 1];
			  glVertex3d(p1.translation()[0], p1.translation()[1], p1.translation()[2]);
			  glVertex3d(p2.translation()[0], p2.translation()[1], p2.translation()[2]);
			  glEnd();
			}
    	pangolin::FinishFrame();
    	usleep(5000);   // sleep 5 ms
	}
}

int main(){
	Vector_Isometry3d  poses;

	ifstream fin(trajectory_file);
	if(!fin){
		cout << "cannot find trajectory file at" << trajectory_file << endl;
		return 1;
	}

	while(!fin.eof()){
		double time, tx, ty, tz, qx, qy, qz, qw;
		fin >> time >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
		Isometry3d Twr(Quaterniond(qw, qx, qy, qz));
		Twr.pretranslate(Vector3d(tx, ty, tz));
		poses.push_back(Twr);
	}
	
	cout << "read total " << poses.size() << " pose entries" << endl;

	// draw trajectory in pangolin
	DrawTrajectory(poses);

	return 0;
}
