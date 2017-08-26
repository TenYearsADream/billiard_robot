#include "ball_detector.hpp"


ballDetector::ballDetector()
{
    ksize = 1;
    kdev = 2;
    min_dist = 5;
    param_1 = 200;
    param_2 = 100;
    ENABLE_IMSHOW = 0;

    trackbarWindowName = "ball detector Trackbar";
    createTrackbars();
}

ballDetector::ballDetector(string config_file)
{
    //load params from config_file
    load_config(config_file);
    trackbarWindowName = config_file;
    createTrackbars();
}


string ballDetector::getFilenameBrowse()
{
    string filename;
    char fn[1024];
    FILE *f = popen("zenity --file-selection", "r");
    fgets(fn, 1024, f);
    filename = fn;
    filename =  filename.substr(0, filename.size()-1);
    return filename;
}


void ballDetector::reload_config(string filename /* = "" */)
{
    destroyWindow(trackbarWindowName);
    
    if (filename.empty())
        filename = getFilenameBrowse();
    
    load_config(filename);
    trackbarWindowName = filename;
    createTrackbars();
}


void ballDetector::save_config(string filename /* = "" */)
{
    if (filename.empty())
        filename = getFilenameBrowse();

    if (filename.empty())
        return;

    FileStorage fs(filename, FileStorage::WRITE);

    fs << "kernel_size" << ksize;
    fs << "kernel_dev" << kdev;
    fs << "hough_min_dist" << min_dist;
    fs << "hough_param1" << param_1;
    fs << "hough_param2" << param_2;
    fs << "ENABLE_IMSHOW" << ENABLE_IMSHOW;

    fs.release();
    cout << "saved config to " << filename << endl;
}



void ballDetector::load_config(string filename /* = "" */)
{
    if (filename.empty())
        filename = getFilenameBrowse();
    
    if (filename.empty())
        return;

    FileStorage fs;
    fs.open(filename, FileStorage::READ);
    
    fs["kernel_size"] >> ksize;
    fs["kernel_dev"] >> kdev;
    fs["hough_min_dist"] >> min_dist;
    fs["hough_param1"] >> param_1;
    fs["hough_param2"] >> param_2;
    fs["ENABLE_IMSHOW"] >> ENABLE_IMSHOW;

    fs.release();
    cout << "finished loading config file " << filename << endl;
}



vector<Ball> ballDetector::detect(Mat img)
{
    vector<Ball> circles = detect_circles(img);
    balls = detect_suit(circles);

    if (ENABLE_IMSHOW)
    {
        Scalar suit_color;
        for (vector<Ball>::iterator it = balls.begin(); it != balls.end(); it++)
        {
            switch((*it).suit)
            {
                case s_BLACK:
                    suit_color = Scalar(0,0,0);
                    break;
                case s_WHITE:
                    suit_color = Scalar(255,255,255);
                    break;
                case s_SOLIDS:
                    suit_color = Scalar(255,0,0);
                    break;
                case s_STRIPES:
                    suit_color = Scalar(0,255,0);
                    break;
            }
            circle(img, (*it).center, (*it).radius, suit_color, 3, 8, 0 );
        }

        imshow("balls", img);
        cvWaitKey(1);
    }
    else
        destroyWindow("balls");

    return balls;
}


void on_trackbar2( int, void* )
{//This function gets called whenever a
	// trackbar position is changed

}


void ballDetector::createTrackbars()
{
    namedWindow(trackbarWindowName,0);
    char TrackbarName[50];
    sprintf( TrackbarName, "kernel_size", ksize);
    sprintf( TrackbarName, "kernel_dev", kdev);
    sprintf( TrackbarName, "hough_min_dist", min_dist);
    sprintf( TrackbarName, "hough_param1", param_1);
    sprintf( TrackbarName, "hough_param2", param_2);
    sprintf( TrackbarName, "ENABLE_IMSHOW", ENABLE_IMSHOW);
 
    createTrackbar( "kernel_size", trackbarWindowName, &ksize, 50, on_trackbar2 );
    createTrackbar( "kernel_dev", trackbarWindowName, &kdev, 10, on_trackbar2 );
    createTrackbar( "hough_min_dist", trackbarWindowName, &min_dist, 50, on_trackbar2 );
    createTrackbar( "hough_param1", trackbarWindowName, &param_1, 500, on_trackbar2 );
    createTrackbar( "hough_param2", trackbarWindowName, &param_2, 500, on_trackbar2 );
    createTrackbar( "ENABLE_IMSHOW", trackbarWindowName, &ENABLE_IMSHOW, 1, on_trackbar2 );
}



vector<Ball> ballDetector::detect_circles(Mat img)
{
    Mat gray;
    vector<Ball> balls_vec;
    
    if (ksize%2 != 1)
        ksize ++;

    cvtColor(img, gray, CV_BGR2GRAY);
    GaussianBlur(gray, gray, Size(ksize, ksize), kdev, kdev);
    vector<Vec3f> circles;
    HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1, min_dist, param_1, param_2, 0, 0 );
    
    for (int i=0; i<circles.size(); i++)
    {
        Ball ball;
        ball.center = Point(cvRound(circles[i][0]), cvRound(circles[i][1]));
        ball.radius = circles[i][2];
        ball.img = Mat(img, Rect(ball.center.x-ball.radius, ball.center.y-ball.radius,
                                    ball.radius*2, ball.radius*2));
        balls_vec.push_back(ball);
    }

    return balls_vec;
}


vector<Ball> ballDetector::detect_suit(vector<Ball> balls)
{
    float per_w, per_b;
    for (vector<Ball>::iterator it = balls.begin(); it != balls.end(); it++)
    {
        WhiteBlackPercentage(*it, per_w, per_b);
        if (per_b > 0.5)
            (*it).suit = s_BLACK;
        else if (per_w > 0.8)
            (*it).suit = s_WHITE;
        else if (per_w > 0.3)
            (*it).suit = s_STRIPES;
        else
            (*it).suit = s_SOLIDS;
    }

    return balls;
}


void ballDetector::WhiteBlackPercentage(Ball ball, float& per_w, float& per_b)
{
    int count_w = 0;
    int count_b = 0;

    for (int i=0; i<ball.img.rows; i++)
    {
        for (int j=0; j<ball.img.cols; j++)
        {
            Vec3b intensity = ball.img.at<Vec3b>(j, i);
            uchar blue = intensity.val[0];
            uchar green = intensity.val[1];
            uchar red = intensity.val[2];

            if (blue < 20 && red < 20 && red < 20)
                count_b ++;

            else if (blue > 200 && red > 200 && red > 200)
                count_w ++;
        }
    }
    
    per_w =  (float)count_w/(ball.img.rows * ball.img.cols);
    per_b =  (float)count_b/(ball.img.rows * ball.img.cols);
}