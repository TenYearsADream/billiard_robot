#include "ball_detector.hpp"


ballDetector::ballDetector()
{
    //TODO
    ksize = 9;
    kdev = 2;
    min_dist = 5;
    param_1 = 200;
    param_2 = 100;

    trackbarWindowName = "Trackbar";
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

    //TODO

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
    
    //TODO

    fs.release();
    cout << "finished loading config file " << filename << endl;
}



vector<Ball> ballDetector::detect(Mat img)
{
    vector<Ball> circles = detect_circles(img);
    balls = detect_suit(circles);

    if (ENABLE_IMSHOW)
    {
        //TODO
    }
    else
        destroyWindow("images: balls");

    return balls;
}


void on_trackbar2( int, void* )
{//This function gets called whenever a
	// trackbar position is changed

}


void ballDetector::createTrackbars()
{//TODO (note to use trackbar2)
    namedWindow(trackbarWindowName,0);
    char TrackbarName[50];
    sprintf( TrackbarName, "H_MIN", H_MIN);
    sprintf( TrackbarName, "H_MAX", H_MAX);
    sprintf( TrackbarName, "S_MIN", S_MIN);
    sprintf( TrackbarName, "S_MAX", S_MAX);
    sprintf( TrackbarName, "V_MIN", V_MIN);
    sprintf( TrackbarName, "V_MAX", V_MAX);
    sprintf( TrackbarName, "DETECT_MAX_OBJ", DETECT_MAX_OBJ);
    sprintf( TrackbarName, "ENABLE_IMSHOW", ENABLE_IMSHOW);
    sprintf( TrackbarName, "dilateSize", dilateSize);
    sprintf( TrackbarName, "erodeSize", erodeSize);
 
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, 256, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, 256, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, 256, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, 256, on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, 256, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, 256, on_trackbar );
    createTrackbar( "DETECT_MAX_OBJ", trackbarWindowName, &DETECT_MAX_OBJ, 1, on_trackbar );
    createTrackbar( "ENABLE_IMSHOW", trackbarWindowName, &ENABLE_IMSHOW, 1, on_trackbar );
    createTrackbar( "dilateSize", trackbarWindowName, &dilateSize, 50, on_trackbar );
    createTrackbar( "erodeSize", trackbarWindowName, &erodeSize, 50, on_trackbar );
}



vector<Ball> ballDetector::detect_circles(Mat img)
{
    vector<Ball> balls_vec;
    
    cvtColor(img, gray, CV_BGR2GRAY);
    GaussianBlur(gray, gray, Size(ksize, ksize), kdev, kdev);
    vector<Vec3f> circles;
    HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1, min_dist, param_1, param_2, 0, 0 );
    
    for (int i=0; i<circles.size(); i++)
    {
        Ball ball;
        ball.center = Point(cvRound(circles[i][0]), cvRound(circles[i][1]));
        ball.radius = circles[i][2];
        ball.image = Mat(img, Rect(ball.center.x-ball.radius, ball.center.y-ball.radius,
                                    ball.radius*2, ball.radius*2));
        balls_vec.push_back(ball);
    }

    return balls_vec;
}


vector<Ball> ballDetector::detect_suit(vector<Ball> balls)
{
    //TODO
    float per_w, per_b;
    for (vector<Ball>::iterator it = balls.begin(); it != balls.end(); it++)
    {
        WhiteBlackPercentage(*it, per_w, per_b);
        if (per_b > 0.8)
            (*it).suit = Suit::BLACK;
        else if (per_w > 0.85)
            (*it).suit = Suit::WHITE;
        else if (per_w > 0.3)
            (*it).suit = Suit::STRIPES;
        else
            (*it).suit = Suit::SOLIDS;
    }

    return balls;
}


void ballDetector::WhiteBlackPercentage(Ball ball, float& per_w, float& per_b)
{
    int count_w = 0;
    int count_b = 0;

    for (int i=0; i<ball.image.rows; i++)
    {
        for (int j=0; j<ball.image.cols; j++)
        {
            Vec3b intensity = ball.image.at<Vec3b>(j, i);
            uchar blue = intensity.val[0];
            uchar green = intensity.val[1];
            uchar red = intensity.val[2];

            if (wb)
            {
                if (blue < 20 && red < 20 && red < 20)
                    count_b ++;
            }
            else
            {
                if (blue > 230 && red > 230 && red > 230)
                    count_w ++;
            }
        }
    }

    per_w =  count_w/(i*j);
    per_b =  count_b/(i*j);
}