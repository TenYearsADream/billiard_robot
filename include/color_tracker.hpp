#ifndef __COLOR_TRACKER__
#define __COLOR_TRACKER__

#include <sstream>
#include <string>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio.hpp"

using namespace std;
using namespace cv;




class colorDetector
{
    public:
        colorDetector();
        colorDetector(string config_filenames);
        vector<Point> detect(Mat img);
        void save_config(string filename  = "" );
        void reload_config(string filename  = "" );

    private:
        Mat image;
        vector<Point> object_center;
        int H_MIN, H_MAX, S_MIN, S_MAX, V_MIN, V_MAX;
        int MAX_NUM_OBJECTS, MIN_OBJECT_AREA, MAX_OBJECT_AREA;
        int DETECT_MAX_OBJ, ENABLE_IMSHOW;
        int erodeSize, dilateSize;
        string trackbarWindowName;

        //void on_trackbar( int, void* );
        void createTrackbars();
        void morphOps(Mat &thresh);
        vector<Point> trackFilteredObject(Mat threshold);
        void load_config(string filename  = "" );
        string getFilenameBrowse();
};



#endif