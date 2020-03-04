#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main( int argc, char** argv ){

    VideoCapture cam(0);
    Mat frame;

    if ( !cam.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    while(true){
        cam >> frame;
        if( !frame.empty() ) imshow( "cam", frame );
        else cout << "no image" << endl;
        if( waitKey(10) == 27 ) break;
    }


    return 0;
}