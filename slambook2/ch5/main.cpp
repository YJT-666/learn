#include <iostream>
#include <string>
#include <chrono>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;

const string imgPath = "/home/smile/learn/slambook2/ch5/data/ubuntu.png";
const string image_file = "/home/smile/learn/slambook2/ch5/data/distorted.png";

void test(const string& imgPath){
	cv::Mat image;
	image = cv::imread(imgPath);

	if(image.data == nullptr) {
		cerr << " file [ " << imgPath << " ] is not exist" << endl;
		return;
	}
	cout << " image width : " << image.cols << " , image height : " << image.rows
		 << " channnel : " << image.channels() << endl;
	cv::imshow("image", image);
	cv::waitKey(0);

	//
	if(image.type() != CV_8UC1 && image.type() != CV_8UC3) {
		cerr << " please input a color image or a intensity picture !" << endl;
		return;
	}

	//
	chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
	for(size_t y =0; y < image.rows; y++) {
		unsigned char *row_ptr = image.ptr<unsigned char>(y);
		for(size_t x = 0; x < image.cols; x++) {
			unsigned char *data_ptr = &row_ptr[x * image.channels()];
			//
			for(int c=0; c != image.channels(); c++){
				unsigned char data = data_ptr[c];
			}
		}
	}
	chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
	chrono::duration<double> time_used = chrono::duration_cast< chrono::duration<double> >(t2-t1);
	cout << "ergodic time: " << time_used.count() << "s" << endl;

	//
	cv::Mat image_another = image;
	image_another(cv::Rect(0, 0, 100, 100)).setTo(255);
	cv::imshow("image", image);
	cv::imshow("image_clone", image_another);
	cv::waitKey();

	//
	cv::Mat image_clone = image.clone();
	image_another(cv::Rect(0, 0, 100, 100)).setTo(255);
	cv::imshow("image", image);
	cv::imshow("image_clone", image_clone);
	cv::waitKey();
	
	//
	cv::destroyAllWindows();
}

void test2(){
 // 本程序实现去畸变部分的代码。尽管我们可以调用OpenCV的去畸变，但自己实现一遍有助于理解。
  // 畸变参数
  double k1 = -0.28340811, k2 = 0.07395907, p1 = 0.00019359, p2 = 1.76187114e-05;
  // 内参
  double fx = 458.654, fy = 457.296, cx = 367.215, cy = 248.375;

  cv::Mat image = cv::imread(image_file, 0);   // 图像是灰度图，CV_8UC1
  int rows = image.rows, cols = image.cols;
  cv::Mat image_undistort = cv::Mat(rows, cols, CV_8UC1);   // 去畸变以后的图

  // 计算去畸变后图像的内容
  for (int v = 0; v < rows; v++) {
    for (int u = 0; u < cols; u++) {
      // 按照公式，计算点(u,v)对应到畸变图像中的坐标(u_distorted, v_distorted)
      double x = (u - cx) / fx, y = (v - cy) / fy;
      double r = sqrt(x * x + y * y);
      double x_distorted = x * (1 + k1 * r * r + k2 * r * r * r * r) + 2 * p1 * x * y + p2 * (r * r + 2 * x * x);
      double y_distorted = y * (1 + k1 * r * r + k2 * r * r * r * r) + p1 * (r * r + 2 * y * y) + 2 * p2 * x * y;
      double u_distorted = fx * x_distorted + cx;
      double v_distorted = fy * y_distorted + cy;

      // 赋值 (最近邻插值)
      if (u_distorted >= 0 && v_distorted >= 0 && u_distorted < cols && v_distorted < rows) {
        image_undistort.at<uchar>(v, u) = image.at<uchar>((int) v_distorted, (int) u_distorted);
      } else {
        image_undistort.at<uchar>(v, u) = 0;
      }
    }
  }

  // 画图去畸变后图像
  cv::imshow("distorted", image);
  cv::imshow("undistorted", image_undistort);
  cv::waitKey();
}


int main(int argc, char** argv){
	
	test2();
	return 0;
}

