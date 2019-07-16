#include <tclap/CmdLine.h>
#include <iostream>

#include "white_balancer.hpp"

int main(int argc, char** argv) {
  std::string camera_device;
  std::string calib_image;

  try {
    TCLAP::CmdLine cmd("White balancer for live video stream", ' ', "1.0.0");
    TCLAP::ValueArg<std::string> dev_arg("d", "device",
                                         "Path to camera device or video file",
                                         false, "", "string");
    TCLAP::ValueArg<std::string> img_arg(
        "i", "image", "Input image for calibrating white balance gains", false,
        "", "string");
    cmd.add(dev_arg);
    cmd.add(img_arg);

    // Parse the argv array.
    cmd.parse(argc, argv);

    // Get the value parsed by each arg.
    camera_device = dev_arg.getValue();
    calib_image = img_arg.getValue();
  } catch (TCLAP::ArgException& e)  // Catch any exceptions
  {
    std::cerr << "Error: " << e.error() << " for arg " << e.argId()
              << std::endl;
  }

  // Open camera device
  cv::VideoCapture cap;
  if (camera_device.empty()) {
    camera_device = "/dev/video0";
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
  std::cout << "\033[1;33mPress c to calibrate gains." << std::endl;
  std::cout << "Press q to exit.\033[0m" << std::endl;

  params gains({1.0, 1.0, 1.0});
  // Calibrate gains if given calib image.
  if (!calib_image.empty()) {
    cv::Mat img;
    img = cv::imread(calib_image);
    if (!img.empty())
      gains = calibrate_white_balance(img);
    else
      std::cerr << "Fail to open calibrating image \"" << calib_image << "\""
                << std::endl;
  }

  // Feed captured frames into the processor
  while (cv::getWindowProperty("Original", 0) >= 0) {
    cv::Mat frame;
    // Get a new frame from camera
    cap >> frame;

    if (!frame.empty()) {
      // Show the original frame
      cv::imshow("Original", frame);

      // Process frame
      // TODO: Complete a white balancer function, and use it to get better
      // images. Show your result here.
      cv::Mat processed = apply_white_balance(frame, gains);
      cv::imshow("Result", processed);
    } else {
      std::cerr << "Skip invalid frame" << std::endl;
    }

    // Wait for key event to keep fps
    auto key = (char)cv::waitKey(30);
    if (key == 'c') {
      gains = calibrate_white_balance(frame);
    } else if (key == 'q') {
      break;
    }
  }

  // The camera will be de-initialized automatically in VideoCapture destructor
  cv::destroyAllWindows();
  return 0;
}
