#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;

int main() {
    VideoCapture cap(0);
    if(!cap.isOpened()){ printf("Cannot open webcam\n"); return -1; }

    CascadeClassifier face_cascade;
    if(!face_cascade.load("haarcascade_frontalface_default.xml")){
        printf("Error loading cascade\n"); return -1;
    }

    Mat frame;
    while(true){
        cap >> frame;
        if(frame.empty()) break;

        std::vector<Rect> faces;
        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        face_cascade.detectMultiScale(gray, faces, 1.1, 4, 0, Size(30,30));

        for(size_t i=0;i<faces.size();i++){
            rectangle(frame, faces[i], Scalar(255,0,0), 2);
        }

        imshow("Face Detection", frame);
        if(waitKey(30) >= 0) break;
    }
    cap.release();
    destroyAllWindows();
    return 0;
}

