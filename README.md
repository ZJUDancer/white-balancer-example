# white-balancer-example
An example for calibrating and correcting white balance of images.

## Build
0. You need to install essential libraries before building:
OpenCV
libtclap-dev
  - `OpenCV`
  - `libtclap-dev`

1. Build

  ```bash
  $ cd /path/to/this/repo
  $ mkdir build && cd build
  $ cmake .. & make -j
  ```
## Arguments
```bash
USAGE:

   ./white_balancer  [-i <string>] [-d <string>] [--] [--version] [-h]


Where:

   -i <string>,  --image <string>
     Input image for calibrating white balance gains

   -d <string>,  --device <string>
     Path to camera device or video file

   -h,  --help
     Displays usage information and exits.


   White balancer for live video stream
```