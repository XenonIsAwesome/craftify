#include "convertors/convert_frame.h"

int main(int argc, char** argv) {
    std::string image_path = argv[1];
    size_t scale = argv[2] ? std::stoi(argv[2]) : 16;
    cv::Mat scaled_img = downscale_image(image_path);
    if (!scaled_img.empty()) {
        cv::Mat minecraft_img = convert_image_to_minecraft_blocks(scaled_img);
        cv::imwrite("output_image.png", minecraft_img);
    }
    return 0;
}
