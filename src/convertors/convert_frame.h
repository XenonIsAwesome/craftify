#ifndef CONVERT_FRAME_H
#define CONVERT_FRAME_H

/**
 * @brief Converts an image to a Minecraft block representation.
 *
 * @param img The input image to be converted.
 * @param mode The mode of conversion, default is "normal".
 * @return The converted image as a cv::Mat object.
 */
cv::Mat convert_image_to_minecraft_blocks(const cv::Mat& img, const std::string& mode = "normal");

/**
 * @brief Downscales an image by a given scale factor.
 *
 * @param image_path The path to the image to be downscaled.
 * @param scale_factor The factor by which to downscale the image, default is 16.
 * @return The downscaled image as a cv::Mat object.
 */
cv::Mat downscale_image(const std::string& image_path, size_t scale_factor = 16);

#endif // CONVERT_FRAME_H