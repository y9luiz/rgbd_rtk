/* 
 *  Software License Agreement (BSD License)
 *
 *  Copyright (c) 2016, Natalnet Laboratory for Perceptual Robotics
 *  All rights reserved.
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided
 *  that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and
 *     the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
 *     the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 *  3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or
 *     promote products derived from this software without specific prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 *  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
 
#include <cstdio>
#include <cstdlib>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <rgbd_loader.h>
#include <klttw_tracker.h>
#include <common_types.h>

using namespace std;
using namespace cv;

void draw_window(Mat &img, Point2i pt1,Point2i pt2,float size){
		Point2i pt3=pt2,pt4=pt1;
		pt3.x=pt3.x-size/2;
		pt3.y=pt3.y-size/2;
		pt4.x=pt2.x+size/2;
		pt4.y=pt2.y+size/2;
		//draw_window( img,pt2, 6);
		rectangle (img,pt3,pt4,150,1,8,0);

}

void draw_last_track(Mat& img, const vector<Point2f> prev_pts, const vector<Point2f> curr_pts)
{
	for(size_t k = 0; k < curr_pts.size(); k++)
	{
		Point2i pt1, pt2;
		pt1.x = prev_pts[k].x;
		pt1.y = prev_pts[k].y;
		pt2.x = curr_pts[k].x;
		pt2.y = curr_pts[k].y;

		circle(img, pt1, 1, CV_RGB(0,0,255), 1);
		circle(img, pt2, 3, CV_RGB(0,255,0), 1);
		line(img, pt1, pt2, CV_RGB(0,255,0));
	}
}

void draw_last_track(Mat& img, const vector<Point2f> prev_pts, const vector<Point2f> curr_pts, float size)
{

	for(size_t k = 0; k < curr_pts.size(); k++)
	{	
		
		Point2i pt1, pt2;
		pt1.x = prev_pts[k].x;
		pt1.y = prev_pts[k].y;
		pt2.x = curr_pts[k].x;
		pt2.y = curr_pts[k].y;

		
	
			 draw_window(img,pt1,pt2,size);	
		
		
		
		circle(img, pt1, 1, CV_RGB(0,0,255), 1);
		circle(img, pt2, 3, CV_RGB(0,255,0), 1);
		line(img, pt1, pt2, CV_RGB(0,255,0));
	}
}
int main(int argc, char **argv)
{
	string index_file_name;
	RGBDLoader loader;
	KLTTrackerW tracker;

	Mat frame, depth;

	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s <index file> <size of window>\n", argv[0]);
		exit(0);
	}

	index_file_name = argv[1];
	loader.processFile(index_file_name);
	tracker.size=atof(argv[2]);
	//Track points on each image
	for(int i = 0; i < loader.num_images_; i++)
	{
		loader.getNextImage(frame, depth);

		tracker.track(frame);
		draw_last_track(frame, tracker.prev_pts_, tracker.curr_pts_, tracker.size);	
		
		
		//draw_tracks(frame, tracker.tracklets_);

		imshow("Image view", frame);
		imshow("Depth view", depth);
		char key = waitKey(15);
		if(key == 27 || key == 'q' || key == 'Q')
		{
			printf("Exiting.\n");
			break;
		}
	}

	return 0;
}
