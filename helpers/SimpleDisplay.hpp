#ifndef SIMPLE_DISPLAY_HEADER
#define SIMPLE_DISPLAY_HEADER value

#include "../CommonHeaders.hpp"

class SimpleDisplay
{
public:

  static void renderImage(cv::Mat imageToRender) {
    static int displayCount = 0;
    displayCount++;
    std::string windowName = "Display window";
    cv::namedWindow( windowName + std::to_string( displayCount )
        , cv::WINDOW_AUTOSIZE );    // Create a window for display.
    cv::imshow( windowName + std::to_string( displayCount )
        , imageToRender );                   // Show our image inside it.
    cv::waitKey(0);    
  }

};

#endif


