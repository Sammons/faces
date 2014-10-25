#include <vector>
#include <string>

#include "CommonHeaders.hpp"

#include "helpers/ImageCollection.hpp"
#include "helpers/SimpleDisplay.hpp"


int main(int argc, char const *argv[])
{

  ImageCollection        Images = ImageCollection();
  std::vector< cv::Mat > images = Images.readImages();

  std::cout << "cols:" << images[0].cols << std::endl;
  std::cout << "rows:" << images[0].rows << std::endl;
  /* code */
  return 0;
}