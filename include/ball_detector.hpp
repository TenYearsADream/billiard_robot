#ifndef __BALL_DETECTOR__
#define __BALL_DETECTOR__

#include <sstream>
#include <string>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "color_tracker.hpp"

using namespace std;
using namespace cv;

enum class Suit { WHITE, BLACK, STRIPES, SOLIDS };

typedef struct Ball_struct
{
    Point center;
    float radius;
    Suit suit;
    Mat img;
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
        void WhiteBlackPercentage(Ball ball, float& per_w, float& per_b);

};



#endif