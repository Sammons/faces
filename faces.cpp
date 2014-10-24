#include <vector>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "helpers/ImageCollection.hpp"
#include "helpers/SimpleDisplay.hpp"


int main(int argc, char const *argv[])
{

  ImageCollection Images = ImageCollection();
  std::vector< cv::Mat > images = Images.readImages();
  SimpleDisplay::renderImage(images[1]);
  /* code */
  return 0;
}