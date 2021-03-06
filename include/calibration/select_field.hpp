#ifndef SELECT_FIELD_H
#define SELECT_FIELD_H

#include <string>
#include <opencv2/opencv.hpp>

#include <calibration/capture_points.hpp>
#include "calibration/file_manager.hpp"

class SelectField
{
  public:
    SelectField(std::string rbg_window, bool is_rgb);
    SelectField(){};
    void run();
    void start();
    void showFrame(cv::Mat rgb_frame);
    bool isDone();
    cv::Mat warp(cv::Mat rgb_frame);

  private:
    void close();
    bool is_rgb_;
    std::string rgb_window_;
    CapturePoints rgb_cap_;
    std::vector<cv::Point2f> dst_points_;
    cv::Mat field_matrix_;
    bool is_done_;
    bool calibrate_;
};

#endif
