#include "color_tracker.hpp"


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "plese enter image file path !!" << endl;
        return -1;
    }
    
    Mat img = imread(argv[1], CV_LOAD_IMAGE_COLOR);

    colorDetector detector;
    vector<Point> center;

    while (true)
    {
        center = detector.detect(img.clone());
        cvWaitKey(3);
    }

    return 0;
}