#ifndef CONVERT_FRAME_H
#define CONVERT_FRAME_H

cv::Mat convert_image_to_minecraft_blocks(const cv::Mat& img);

cv::Mat downscale_image(const std::string& image_path, int scale_factor = 16);

#endif // CONVERT_FRAME_H