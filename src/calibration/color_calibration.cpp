#include "calibration/color_calibration.hpp"

ColorCalibration::ColorCalibration(){
    //initial parameters
    hsv_min_h_ = 0;
    hsv_max_h_ = 0;
    hsv_min_s_ = 0;
    hsv_max_s_ = 0;
    hsv_min_v_ = 0;
    hsv_max_v_ = 0;

    is_blue_saved_ = false;
    is_yellow_saved_ = false;
    is_orange_saved_ = false;
    is_red_saved_ = false;
    is_pink_saved_ = false;
    is_green_saved_ = false;
    window_name_ = "Color Calibration";
    

    //decide if open or not file to save color calibration
    ros::param::get("/vision/calibration/calibrate_team_color", calibrate_);
    auto sourceDir = ros::package::getPath("vision").append("/data/");
    auto filename = "color_calibration.yaml";
    colorHandler_ = cv::FileStorage(sourceDir+filename, cv::FileStorage::READ);

    

    if (calibrate_)
    {

        //create UI
        cv::namedWindow("HSV Trackbar");
        cv::createTrackbar("HMIN", "HSV Trackbar", &hsv_min_h_, 360);
        cv::createTrackbar("HMAX", "HSV Trackbar", &hsv_max_h_, 360);
        cv::createTrackbar("SMIN", "HSV Trackbar", &hsv_min_s_, 256);
        cv::createTrackbar("SMAX", "HSV Trackbar", &hsv_max_s_, 256);
        cv::createTrackbar("VMIN", "HSV Trackbar", &hsv_min_v_, 256);
        cv::createTrackbar("VMAX", "HSV Trackbar", &hsv_max_v_, 256);
        
        
        //save previous parameters
        colorHandler_["Blue"] >> old_blue;
        colorHandler_["Yellow"] >> old_yellow;
        colorHandler_["Orange"] >> old_orange;
        colorHandler_["Red"] >> old_red;
        colorHandler_["Pink"] >> old_pink;
        colorHandler_["Green"] >> old_green;
        //set file calibration to save HSV values
        colorManager_ = cv::FileStorage(sourceDir+filename, cv::FileStorage::WRITE);
    }
    else
        ROS_INFO("Color already calibrated, to recalibrate change config!");

}

ColorCalibration::~ColorCalibration(){
    if (calibrate_){
        if (not is_blue_saved_)
            colorManager_ << "Blue" << old_blue;
        if (not is_yellow_saved_)
            colorManager_ << "Yellow" << old_yellow;
        if(not is_orange_saved_)
            colorManager_ << "Orange" << old_orange;
        if(not is_red_saved_)
            colorManager_ << "Red" << old_red;
        if(not is_pink_saved_)
            colorManager_ << "Pink" << old_pink;
        if(not is_green_saved_)
            colorManager_ << "Green" << old_green;                  
        
        colorManager_.release();
    }
    colorHandler_.release();
}

void ColorCalibration::calibrate(cv::Mat& rgb_input){
    //equalizeIntensity(rgb_input);

    if (calibrate_)
    {
        
        cv::waitKey(1);
        if (is_blue_saved_ && is_yellow_saved_ && is_orange_saved_ && is_red_saved_ && is_pink_saved_ && is_green_saved_)
            cv::destroyWindow(window_name_);
        else{
            cv::Mat hsv_converted;
            cv::cvtColor(rgb_input, hsv_converted, CV_BGR2HSV);

            cv::inRange(hsv_converted, cv::Scalar(hsv_min_h_, hsv_min_s_, hsv_min_v_),
                         cv::Scalar(hsv_max_h_, hsv_max_s_, hsv_max_v_), segmented_image_);

            cv::imshow(window_name_, segmented_image_);
        }

        if (cv::waitKey(10) == 'b' && not is_blue_saved_)
        {
            ROS_INFO("Blue Calibrated!");
            save("Blue");
        }
        if (cv::waitKey(10) == 'y' && not is_yellow_saved_)
        {
            ROS_INFO("Yellow Calibrated!");
            save("Yellow");
        }
        if (cv::waitKey(10) == 'o' && not is_orange_saved_)
        {
            ROS_INFO("Orange Calibrated!");
            save("Orange");
        }
        if (cv::waitKey(10) == 'r' && not is_red_saved_)
        {
            ROS_INFO("Red Calibrated!");
            save("Red");
        }
        if (cv::waitKey(10) == 'p' && not is_pink_saved_)
        {
            ROS_INFO("Pink Calibrated!");
            save("Pink");
        }
        if (cv::waitKey(10) == 'g' && not is_green_saved_)
        {
            ROS_INFO("Green Calibrated!");
            save("Green");
        }
    }
}

void ColorCalibration::save(std::string color){
    cv::Mat colorMat =  (cv::Mat_<int>(2, 3) << hsv_min_h_, hsv_min_s_, hsv_min_v_, hsv_max_h_, hsv_max_s_, hsv_max_v_);
    colorManager_ << color << colorMat;
    if (color == "Blue")
        is_blue_saved_ = true;
    if (color == "Yellow")
        is_yellow_saved_ = true;
    if (color == "Orange")
        is_orange_saved_ = true;
    if (color == "Red")
        is_red_saved_ = true;
    if (color == "Pink")
        is_pink_saved_ = true;
    if (color == "Green")
        is_green_saved_ = true;

}

bool ColorCalibration::isCalibrated(){
    return calibrate_;
}

cv::Mat ColorCalibration::getRGBCalibrated(){
    return rgb_calibrated_;
}

void ColorCalibration::equalizeIntensity(cv::Mat& rgb_input)
{
    if(rgb_input.channels() >= 3)
    {
        cv::Mat ycrcb;

        cv::cvtColor(rgb_input,ycrcb,CV_BGR2YCrCb);
        std::vector<cv::Mat> channels;
        cv::split(ycrcb,channels);

        cv::equalizeHist(channels[0], channels[0]);

        cv::merge(channels,ycrcb);

        cv::cvtColor(ycrcb,rgb_input,CV_YCrCb2BGR);

    }else
        ROS_ERROR("[COLOR CALIBRATION] 3 channels are needed to >>color<< calibration!");
}