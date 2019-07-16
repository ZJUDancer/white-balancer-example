#include "white_balancer.hpp"

params calibrate_white_balance(const cv::Mat& input_image) {
  std::cout << "\033[1;36mCalibrating...\033[0m" << std::endl;
  params gains({1.0, 1.0, 1.0});
  /* TODO: Use the input image to calibrate and get the gains.
     The number of gains can be 2 or 3.
     You can modify the struct definition in hpp file.*/
  std::cout << "\033[1;36mGains: " << gains.r << ", " << gains.g << ", "
            << gains.b << "\033[0m" << std::endl;
  return gains;
}

cv::Mat apply_white_balance(const cv::Mat& input_image, const params gains) {
  return input_image;
}