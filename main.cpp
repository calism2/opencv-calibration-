#include <iostream>
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
int CHECKERBOARD[2]{7,9}; 

int main(int, char**) {
    cv::VideoCapture cap(3);
    int samples = 0;
    if(!cap.isOpened()){ return -1; }
    // Creating vector to store vectors of 3D points for each checkerboard image
    std::vector<std::vector<cv::Point3f> > objpoints;
    
    // Creating vector to store vectors of 2D points for each checkerboard image
    std::vector<std::vector<cv::Point2f> > imgpoints;

    std::vector<cv::Point3f> objp;
    for(int i{0}; i<CHECKERBOARD[1]; i++)
    {
        for(int j{0}; j<CHECKERBOARD[0]; j++)
        objp.push_back(cv::Point3f(j,i,0));
    }

    cv::Mat frame, gray, draw;
    while( samples < 30 ){
        cap.read(frame);
        std::vector<cv::Point2f> corner_pts;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        char key = cv::waitKey(1);
        if( key == 'k'){
            bool success = cv::findChessboardCorners(gray, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, cv::CALIB_CB_ADAPTIVE_THRESH  || cv::CALIB_CB_NORMALIZE_IMAGE);
            if(success){
                cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);
                cv::cornerSubPix(gray,corner_pts,cv::Size(11,11), cv::Size(-1,-1),criteria);

                objpoints.push_back(objp);
                imgpoints.push_back(corner_pts);

                cv::drawChessboardCorners(frame, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, success);
                samples++;
            }
            
        }
        cv::imshow(std::string{"winname"}, frame);
        
     }

    cv::destroyAllWindows();
 
    cv::Mat cameraMatrix,distCoeffs, R, T;
    
    /*
    * Performing camera calibration by 
    * passing the value of known 3D points (objpoints)
    * and corresponding pixel coordinates of the 
    * detected corners (imgpoints)
    */
    cv::calibrateCamera(objpoints, imgpoints, cv::Size(gray.rows,gray.cols), cameraMatrix, distCoeffs, R, T);
    
    std::cout << "cameraMatrix : " << cameraMatrix << std::endl;
    std::cout << "distCoeffs : " << distCoeffs << std::endl;
    std::cout << "Rotation vector : " << R << std::endl;
    std::cout << "Translation vector : " << T << std::endl;

    while (true)
    {
        cap.read(frame);
        cv::Mat result;
        
        cv::undistort(frame, result, cameraMatrix, distCoeffs);
        cv::imshow(std::string{"winnam1e"}, result);
        char key = cv::waitKey(1);
    }
    

}
