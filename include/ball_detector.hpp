#ifndef __BALL_DETECTOR__
#define __BALL_DETECTOR__

#include <sstream>
#include <string>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"

using namespace std;
using namespace cv;


typedef struct Ball_struct
{
    Point center;
    float radius;
    int suit;   //0:no one's ball (white/black) 1:stripes 2:solids
} Ball;


class ballDetector
{
    public:
        ballDetector();
        ballDetector(string config_filenames);
        vector<Ball> detect(Mat img);
        void save_config(string filename  = "" );
        void reload_config(string filename  = "" );

    private:
        Mat image;
        vector<Ball> balls;
        string trackbarWindowName;
        int ksize, kdev;    //GaussianBlur kernel size and std
        int min_dist, param_1, param_2;   //Hough params

        void createTrackbars();
        void load_config(string filename  = "" );
        string getFilenameBrowse();
        vector<Ball> detect_circles(Mat img);
        vector<Ball> detect_suit(vector<Ball>);
        bool isWhite(Ball ball);
        bool isBlack(Ball ball);
};



#endif