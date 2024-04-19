#include<opencv2/opencv.hpp>
#include<random>
#include<iostream>
#include"Cellular.h"
using namespace std;
using namespace cv;
int main()
{
	int x, y, iter;
	cout << "Enter Width" << endl;
	cin >> x;
	cout << "Enter Height" << endl;
	cin >> y;
	cout << "Enter Iterations" << endl;
	cin >> iter;
	CellularAutomata(x, y, 0.1,iter);
	waitKey(0);
	return 0;
}