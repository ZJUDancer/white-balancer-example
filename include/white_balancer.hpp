#pragma once
#include <opencv2/opencv.hpp>

struct params {
    double r, g, b;
};

/**
 * \brief Function used to get white balance gains.
 *
 * \param input_image - Input image to calibrate gains.
 *
 * \return The gains used to apply white balance.
 */
params calibrate_white_balance(const cv::Mat& input_image);

/**
 * \brief Use given gains to correct the white balance of an image.
 *
 * \param input_image - Input image.
 * \param gains - White balance gains.
 *
 * \return The corrected image.
 */
cv::Mat apply_white_balance(const cv::Mat& input_image,
                            const params gains);