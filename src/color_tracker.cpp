#include "color_tracker.hpp"


colorDetector::colorDetector()
{
    erodeSize = 1;
    dilateSize = 1;
    ENABLE_IMSHOW = 0;
    DETECT_MAX_OBJ = 0;
    H_MIN = 0; H_MAX = 256;
    S_MIN = 0; S_MAX = 256;
    V_MIN = 0; V_MAX = 256;
    MAX_NUM_OBJECTS=50;
    MIN_OBJECT_AREA = 640*480/2;
    MAX_OBJECT_AREA = 640*480;
    trackbarWindowName = "Trackbar";
    createTrackbars();
}

colorDetector::colorDetector(string config_file)
{
    //load params from config_file
    load_config(config_file);
    trackbarWindowName = config_file;
    createTrackbars();
}


string colorDetector::getFilenameBrowse()
{
    string filename;
    char fn[1024];
    FILE *f = popen("zenity --file-selection", "r");
    fgets(fn, 1024, f);
    filename = fn;
    filename =  filename.substr(0, filename.size()-1);
    return filename;
}


void colorDetector::reload_config(string filename /* = "" */)
{
    destroyWindow(trackbarWindowName);
    
    if (filename.empty())
        filename = getFilenameBrowse();
    
    load_config(filename);
    trackbarWindowName = filename;
    createTrackbars();
}


void colorDetector::save_config(string filename /* = "" */)
{
    if (filename.empty())
        filename = getFilenameBrowse();

    if (filename.empty())
        return;

    FileStorage fs(filename, FileStorage::WRITE);

    fs << "H_MIN" << H_MIN; fs << "H_MAX" << H_MAX;
    fs << "S_MIN" << S_MIN; fs << "S_MAX" << S_MAX;
    fs << "V_MIN" << V_MIN; fs << "V_MAX" << V_MAX;
    fs << "DETECT_MAX_OBJ" << DETECT_MAX_OBJ;
    fs << "ENABLE_IMSHOW" << ENABLE_IMSHOW;
    fs << "dilateSize" << dilateSize;
    fs << "erodeSize" << erodeSize;
    fs << "MAX_NUM_OBJECTS" << MAX_NUM_OBJECTS;
    fs << "MIN_OBJECT_AREA" << MIN_OBJECT_AREA;
    fs << "MAX_OBJECT_AREA" << MAX_OBJECT_AREA;

    fs.release();
    cout << "saved config to " << filename << endl;
}



void colorDetector::load_config(string filename /* = "" */)
{
    if (filename.empty())
        filename = getFilenameBrowse();
    
    if (filename.empty())
        return;

    FileStorage fs;
    fs.open(filename, FileStorage::READ);
    
    fs["H_MIN"] >> H_MIN; fs["H_MAX"] >> H_MAX;
    fs["S_MIN"] >> S_MIN; fs["S_MAX"] >> S_MAX;
    fs["S_MIN"] >> V_MIN; fs["V_MAX"] >> V_MAX;
    fs["DETECT_MAX_OBJ"] >> DETECT_MAX_OBJ;
    fs["ENABLE_IMSHOW"] >> ENABLE_IMSHOW;
    fs["dilateSize"] >> dilateSize;
    fs["erodeSize"] >> erodeSize;
    fs["MAX_NUM_OBJECTS"] >> MAX_NUM_OBJECTS;
    fs["MIN_OBJECT_AREA"] >> MIN_OBJECT_AREA;
    fs["MAX_OBJECT_AREA"] >> MAX_OBJECT_AREA;

    fs.release();
    cout << "finished loading config file " << filename << endl;
}



vector<Point> colorDetector::detect(Mat img)
{
    Mat HSV, threshold;
    //convert frame from BGR to HSV colorspace
    cvtColor(img, HSV, COLOR_BGR2HSV);
    //filter HSV image between values and store filtered image to
    //threshold matrix
    inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
    //perform morphological operations on thresholded image to eliminate noise
    //and emphasize the filtered object(s)
    morphOps(threshold);
    //pass in thresholded frame to our object tracking function
    //this function will return the x and y coordinates of the
    //filtered object
    vector<Point> center = trackFilteredObject(threshold);
    //save vector
    object_center = center;

    if (ENABLE_IMSHOW)
    {
        char c;
        Mat thresholdRGB, final_imgs;
        cvtColor(threshold, thresholdRGB, COLOR_GRAY2RGB);
        Mat imgs(img.rows, img.cols*2, CV_8UC3);
        img.copyTo(imgs(Rect(0, 0, img.cols, img.rows)));
        //HSV.copyTo(imgs(Rect(img.cols, 0, img.cols, img.rows)));
        thresholdRGB.copyTo(imgs(Rect(img.cols, 0, img.cols, img.rows)));
        resize(imgs, final_imgs, Size(), 0.5, 0.5);
        imshow("images: input  -->  threshold", final_imgs);
        cvWaitKey(1);
        /*if ((c = cvWaitKey(1)) == 'q')
            terminate();*/
    }
    else
        destroyWindow("images: input  -->  threshold");

    return object_center;
}


void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed

}


void colorDetector::createTrackbars()
{
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



void colorDetector::morphOps(Mat &thresh)
{
	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle
    
    erodeSize = (erodeSize > 0) ? erodeSize : 1;
    dilateSize = (dilateSize > 0) ? dilateSize : 1;

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(erodeSize,erodeSize));
    //dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(dilateSize,dilateSize));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);

	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);

}



vector<Point> colorDetector::trackFilteredObject(Mat threshold)
{
    int x,y;
    vector<Point> center;
	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	if (hierarchy.size() > 0)
    {
		int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS)
        {
			for (int index = 0; index >= 0; index = hierarchy[index][0])
            {
				Moments moment = moments((Mat)contours[index]);
				double area = moment.m00;

                if (DETECT_MAX_OBJ)
                {
                    if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea)
                    {
                        x = moment.m10/area;
                        y = moment.m01/area;
                        refArea = area;
                    }
                }
                else
                    center.push_back(Point(moment.m10/area, moment.m01/area));
			}
            if (DETECT_MAX_OBJ)
                center.push_back(Point(x,y));
		}
	}

    return center;
}