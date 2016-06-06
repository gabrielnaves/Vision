#include <vision/dummy_segmentation_algorithm.hpp>

REGISTER_ALGORITHM_DEF(DummySegmentationAlgorithm);

DummySegmentationAlgorithm::DummySegmentationAlgorithm()
{
    name_ = "DummySegmentationAlgorithm";
}

void DummySegmentationAlgorithm::run()
{
    ROS_INFO("Running dummy segmentation algorithm");
    RawImage::getInstance().getRawImage().copyTo(output_image_);
}
