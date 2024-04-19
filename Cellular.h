#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<random>
#include<Windows.h>
using namespace std;
using namespace cv;

//Converts a grid of points into an image which can be visualized
void genimagefromvec(Mat m, vector<vector<int>>grid) 
{
	for (int i = 0; i < grid.size(); i++) 
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			Vec3b col = m.at<Vec3b>(i, j);
			if (grid[i][j] == 1) 
			{
				col[0] = 255; col[1] = 255; col[2] = 255;
			}
			else 
			{
				col[0] = 0; col[1] = 0; col[2] = 0;
			}
			
			m.at<Vec3b>(i, j)=col;
		}
	}
}
//Converts a grid of points into an image which can be visualized in colour
void genimagefromvec(Mat m, vector<vector<float>>grid)
{
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			Vec3b col = m.at<Vec3b>(j, i);
			int val = grid[i][j] * 255;
			col[0] = val / 3;
			col[1] = val;
			col[2] = val / 2;
			m.at<Vec3b>(j, i) = col;
		}
	}
}
//Takes a grid to the next iteration of cellular automata
vector<vector<int>> celliter(vector<vector<int>> prevgrid, int min, int max)
{
	//Iterates one loop of the cell iterator
	vector<vector<int>>grid;
	grid.resize(prevgrid.size());
	for (int i = 0; i < grid.size(); i++) 
	{
		grid[i].resize(prevgrid[i].size());
		fill(grid[i].begin(), grid[i].end(), 0);
	}
	for (int i = 1; i < prevgrid.size() - 1; i++)
	{
		for (int j = 1; j < prevgrid[i].size() - 1; j++)
		{
			int neighbours = prevgrid[i - 1][j] + prevgrid[i + 1][j] + prevgrid[i][j - 1] + prevgrid[i][j + 1];// +prevgrid[j - 1][i - 1] + prevgrid[j + 1][i - 1] + prevgrid[j + 1][i - 1] + prevgrid[j + 1][i + 1];
			if (neighbours >= min && neighbours <= max)
			{
				grid[i][j] = 1;
			}
			else
			{
				grid[i][j] = 0;
			}
		}
	}
	return grid;
}
//returns sum of 2d vectors
vector<vector<float>> Sumveccell(vector<vector<int>>a, vector<vector<float>>b,int iter) 
{
	vector<vector<float>>c;
	for (int i = 0; i < a.size(); i++) 
	{
		vector<float>row;
		for (int j = 0; j < a[i].size(); j++) 
		{
			float s = b[i][j] + ((float)a[i][j])/iter;
			row.push_back(s);
		}
		c.push_back(row);
	}
	return c;
}
//Changes the backgroun to a more "muddy" texture
void Real_Mud(Mat image) 
{
	Mat ground = Mat(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));
	mt19937 eng(time(NULL));
	uniform_int_distribution<int> dis1(0, 255);
	for (int i = 0; i < ground.cols; i++) 
	{
		for (int j = 0; j < ground.rows; j++) 
		{
			Vec3b col = ground.at<Vec3b>(j, i);
			int val = dis1(eng);
			col[0] = val;
			col[1] = val;
			col[2] = val;
			ground.at<Vec3b>(j, i) = col;
		}
	}
	
	blur(ground, ground,Size(5,5));
	
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			Vec3b col = image.at<Vec3b>(i, j);
			if (col[0] == 0 && col[1] == 0 && col[2] == 0) 
			{
				float val = ground.at<Vec3b>(i, j)[0];
				col[0] = 62 * (float)(val / 255);
				col[1] = 84 * (float)(val/255);
				col[2] = 112 * (float)(val/255);
			}
			image.at<Vec3b>(i, j) = col;
		}
	}

}

//Generates a complete image using cellulat automata
Mat CellularAutomata(int x, int y, float density,int iter)
{
	//Initial Setup
	//Small hack Since the relation of rows and columns and x and y was not known, i switched their values here
	int c;
	c = x;
	x = y;
	y = c;

	///Grids
	vector<vector<int>>grid;
	vector<vector<float>>finalgrid;
	///Random engine distribution and engine
	mt19937 eng(time(NULL));
	uniform_real_distribution<double> dis1(0, 1);
	//Image
	Mat image = Mat(x, y, CV_8UC3,Scalar(0,0,0));
	//Video Can be used to see the progress of cellular automata iterations
	vector<Mat>video;
	//Putting random points on the grid based on density.
	for (int i = 0; i < y; i++) 
	{
		vector<int>row;
		for (int j = 0; j < x; j++) 
		{
			int a;
			float prob = dis1(eng);
			if (prob <= density)
			{
				a = 1;
			}
			else
			{
				a = 0; 
			}
			row.push_back(a);
			cout << i <<":"<<j<< endl;
		}
		grid.push_back(row);
	}
	//Initializing final grid
	for (int i = 0; i < y; i++)
	{
		vector<float>row;
		for (int j = 0; j < x; j++)
		{
			row.push_back(0);
		}
		finalgrid.push_back(row);
	}
	//Creating a loop to iterate cellular automata
	for (int i = 0; i < iter; i++)
	{
		cout << i << endl;
		finalgrid = Sumveccell (grid, finalgrid,iter);
		Mat a = Mat::zeros(Size(x, y), CV_8UC3);
		genimagefromvec(a, grid);
		grid = celliter(grid, 1, 2);
		video.push_back(a);
		

	}
	//Display the images
	//For testing
	cout << finalgrid[20][40] << endl;
	genimagefromvec(image, finalgrid);
	Real_Mud(image);
	imshow("Window", image);
	waitKey(0);
	return image;
}

