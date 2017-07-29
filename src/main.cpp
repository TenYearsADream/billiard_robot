#include "color_tracker.hpp"


int main(int argc, char* argv[])
{
    char c;
    string config_file;
    vector<Point> center;
    
    if (argc < 2)
    {
        cout << "plese enter image file path !!" << endl;
        return -1;
    }
    else if (argc > 2)
        config_file = argv[2];
    else
    {
        cout << "Do you want to load configuration from file? type 'y':YES or 'n':NO" << endl;
        cin >> c;        
    }

    Mat keyMat(300, 300, CV_8UC3, CV_RGB(0,0,0));
    putText(keyMat,"Keyboard input",Point(10,30),2,1,Scalar(255,255,255),2);
    putText(keyMat,"exit program: 'q'",Point(10,100),2,0.7,Scalar(255,0,0),1);
    putText(keyMat,"load config file: 'l'",Point(10,150),2,0.7,Scalar(255,0,0),1);
    putText(keyMat,"save config file: 's'",Point(10,200),2,0.7,Scalar(255,0,0),1);
    
    Mat img = imread(argv[1], CV_LOAD_IMAGE_COLOR);

    colorDetector detector;
    if (!config_file.empty() || c == 'y')
        detector.reload_config(config_file);


    while (true)
    {
        center = detector.detect(img.clone());
        imshow("Keyboard input", keyMat);
        c = cvWaitKey(30);
        switch (c)
        {
            case 'q':
                return 0;
            case 'l':
                detector.reload_config();
                break;
            case 's':
                detector.save_config();
                break;
        }
    }

    return 0;
}