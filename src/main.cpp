#include <iostream>

#include "white_balancer.hpp"

int main(int argc, char** argv) {
  std::string camera_device;

  // Open camera device
  cv::VideoCapture cap;
  if (camera_device.empty()) {
    camera_device = "0";
    cap.open(0);
  } else {
    cap.open(camera_device);
  }
  if (!cap.isOpened()) {
    std::cerr << "Fail to open VideoCapture!" << std::endl;
    return -1;
  }
  std::cout << "Camera device: " << camera_device << std::endl;

  // Get properties of input video frame
  int camera_width(640), camera_height(480), camera_fps(30);
#if (CV_VERSION_MAJOR >= 4)
  camera_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  camera_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
  camera_fps = cap.get(cv::CAP_PROP_FPS);
#else
  camera_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  camera_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
  camera_fps = cap.get(CV_CAP_PROP_FPS);
#endif
  std::cout << "Frame property: " << camera_width << "x" << camera_height << "@"
            << camera_fps << "fps" << std::endl;

  // Open the windows for display.
  std::cout << "\033[1;34mStart capturing\033[0m" << std::endl;
#if (CV_VERSION_MAJOR >= 4)
  cv::namedWindow("Original", cv::WINDOW_NORMAL);
  cv::namedWindow("Result", cv::WINDOW_NORMAL);
#else
  cv::namedWindow("Original", CV_WINDOW_NORMAL);
  cv::namedWindow("Result", CV_WINDOW_NORMAL);
#endif
  std::cout << "Press any key to exit" << std::endl;

  params gains({1.0, 1.0, 1.0});
  // Calibrate gains if given calib image.

  // Feed captured frames into the processor
  while (cv::getWindowProperty("Original", 0) >= 0) {
    cv::Mat frame;
    // Get a new frame from camera
    cap >> frame;

    if (!frame.empty()) {
      // Show the original frame
      cv::imshow("Original", frame);

      // Process frame
      // TODO: Complete a white balancer function, and use it to get better images.
      // Show your result here.
      cv::Mat processed = apply_white_balance(frame, gains);
      cv::imshow("Result", processed);
    } else {
      std::cerr << "Skip invalid frame" << std::endl;
    }

    // Wait for key event to keep fps
    auto key = (char)cv::waitKey(30);
    if (key == 'c') {
        calibrate_white_balance(frame);
    } else if (key >= 0) {
      break;
    }
  }

  // The camera will be de-initialized automatically in VideoCapture destructor
  cv::destroyAllWindows();
  return 0;
}
