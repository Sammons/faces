#ifndef IMAGES_HEADER
#define IMAGES_HEADER value

#include "../CommonHeaders.hpp"

class ImageCollection
{
public:
  ImageCollection(){};
  ~ImageCollection(){};
  bool nextImage(cv::Mat& imageResult);
  bool readImageAtIndex(cv::Mat& imageResult, int index);
  std::vector< std::string > imageList();
private:
  int currentImageIndex = 0;
};
#endif