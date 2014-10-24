#ifndef IMAGES_DEFINITION
#define IMAGES_DEFINITION
#include "ImageCollection.hpp"

std::vector< cv::Mat > ImageCollection::readImages() {
  return this->readImages( this->allImages() );
}

std::vector< cv::Mat > ImageCollection::readImages( std::vector< std::string > imagePathListing ) {
  std::vector< cv::Mat > returnImages;
  for ( auto path : imagePathListing ) {
    returnImages.push_back( cv::imread( path ) );
  }
  return returnImages;
}

std::vector< std::string > ImageCollection::allImages( ) {
  const std::string BASEPATH = "/home/catalyst/Desktop/faces/";
  std::vector< std::string > images = { 
    BASEPATH + "BenFace1.png",
    BASEPATH + "BenFace2.png",
    BASEPATH + "BenFace3.png" 
  };
  return images;
}
#endif
