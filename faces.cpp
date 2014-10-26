#include <vector>
#include <string>

#include "CommonHeaders.hpp"

#include "helpers/ImageCollection.hpp"
#include "helpers/SimpleDisplay.hpp"


void copyImageToCol(cv::Mat& destinationMatrix, int col, cv::Mat image) {

}

int main(int argc, char const *argv[])
{

  ImageCollection Images = ImageCollection();
  cv::Mat tmp_mat;
  Images.nextImage(tmp_mat);

  std::cout << "pixels:" << tmp_mat.cols*tmp_mat.rows << std::endl;
  std::cout << "images:" << Images.imageList().size() << std::endl;
  std::cout << std::endl;

  // cv::Mat matrixWhereEveryColumnIsAnImage(
    
  //   );  

  return 0;
}