#ifndef IMAGES_HEADER
#define IMAGES_HEADER value

#include "../CommonHeaders.hpp"

class ImageCollection
{
public:
  ImageCollection(){};
  ~ImageCollection(){};
  cv::Mat nextImage ( cv::Size );
  cv::Mat readImageAtIndex(int index, cv::Size );
  std::vector< std::string > imageList();
private:
  int currentImageIndex = 0;
};
#endif