#include <iostream>

using namespace std;
#include <Eigen/Core>
#include <Eigen/Dense>

int main(){
	using namespace Eigen;

	Matrix3d matrix_33 = Matrix3d::Random();
	auto res = matrix_33.transpose() * matrix_33;
	cout << "matrix_33:\n" << matrix_33 << endl;
	cout << "res\n" << res << endl;
}
