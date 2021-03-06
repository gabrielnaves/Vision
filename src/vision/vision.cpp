#include <vision/vision.hpp>

Vision& Vision::getInstance()
{
    static Vision vision;
    return vision;
}

/**
 * Starts the vision system.
 */
Vision::Vision()
{
    has_received_first_image_ = false;
    ConfigParser::getInstance().parseConfigFile();
    Segmenter::getInstance().init();
    Identifier::getInstance().init();
}

/**
 * Execute vision processing. Nothing will be done until the first image has been
 * received.
 */
void Vision::run()
{
    if (has_received_first_image_)
    {
        Segmenter::getInstance().runSegmentationAlgorithms();
        Identifier::getInstance().runIdentificationAlgorithms();
        Tracker::getInstance().runTracking();

        VisionGUI::getInstance().showOutputImages();
    }
}

/**
 * Receives the raw rgb image, and passes it on to the RawImage class and the VisionGUI.
 */
void Vision::setRawImage(const cv::Mat &image)
{
    if (isValidSize(image))
    {
        has_received_first_image_ = true;
        RawImage::getInstance().setRawImage(image);
        VisionGUI::getInstance().setInitialImage(image);
    }
}

/**
 * Check whether an image has a valid size, i.e., neither the width nor the height can be zero.
 * @return true if the image has proper size, false otherwise.
 */
bool Vision::isValidSize(const cv::Mat &img)
{
    return not (img.rows == 0 or img.cols == 0);
}

std::unordered_map<std::string, TrackingOutput> Vision::getVisionOutput()
{
    return Tracker::getInstance().getTrackerOutput();
}
