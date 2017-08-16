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
	//create window for trackbars
    namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
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
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
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
        balls_vec.push_back(ball);
    }

    return balls_vec;
}


vector<Ball> ballDetector::detect_suit(vector<Ball> balls)
{
    //TODO
}


bool ballDetector::isWhite(Ball ball)
{
    //TODO
}


bool ballDetector::isBlack(Ball ball)
{
    //TODO
}