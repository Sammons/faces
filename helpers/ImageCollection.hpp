#ifndef IMAGES_HEADER
#define IMAGES_HEADER value

#include "../CommonHeaders.hpp"

class ImageCollection
{
public:
  ImageCollection(){};
  ~ImageCollection(){};
  std::vector<cv::Mat>readImages( );
private:
  std::vector<std::string>allImages( );
  std::vector<cv::Mat>readImages( std::vector<std::string> imagePathListing ); 
};
#endif