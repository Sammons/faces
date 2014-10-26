#include <vector>
#include <string>

#include "CommonHeaders.hpp"

#include "helpers/ImageCollection.hpp"
#include "helpers/SimpleDisplay.hpp"

/* assumes destination has space */
/* rocks everything into being a float */
void copyImageToCol(cv::Mat& destinationMatrix, int col, cv::Mat image) {
  int rowOfDestCol = 0;
  image = image.reshape(0,1); // convert to vector
  for (int i = 0; i < image.cols; i ++) {
      destinationMatrix.at<float>(col, rowOfDestCol) 
          = (float)image.at<unsigned char>(i);
      rowOfDestCol++;
  }
}

void subtractAverageVectorFromEveryColumnOfMatrix(cv::Mat& Matrix, cv::Mat avg ) {
  for (int row = 0; row < Matrix.rows; ++row) {
    for (int col = 0; col < Matrix.cols; ++col) {
      Matrix.at<float>(col, row) -= avg.at<float>(1, row);
    }
  }
}


void averageMatrixWhereColumsAreImagesInto1xNVector
  (cv::Mat& matrixWhereEveryColumnIsAnImage, cv::Mat& avg ) {
    for (int row = 0; row < matrixWhereEveryColumnIsAnImage.rows; ++row) {
      for (int column = 0; column < matrixWhereEveryColumnIsAnImage.cols; ++column) {
        avg.at<float>(0, row) += 
          matrixWhereEveryColumnIsAnImage.at<float>(column, row);
      }
      avg.at<float>(0, row) /= matrixWhereEveryColumnIsAnImage.cols;
    }
  }

int main(int argc, char const *argv[])
{

  ImageCollection Images = ImageCollection();
  cv::Mat tmp_mat;
  cv::Size imageSize(32,32); 
  tmp_mat = Images.nextImage(imageSize);
  std::cout << "pixels:" << tmp_mat.cols*tmp_mat.rows << std::endl;
  std::cout << "images:" << Images.imageList().size() << std::endl;
  std::cout << std::endl;

  int numImagesToUse = 100; // there are like a redic amount of images
  cv::Mat matrixWhereEveryColumnIsAnImage = cv::Mat(
    tmp_mat.cols*tmp_mat.rows,
    numImagesToUse,
    cv::DataType<float>::type,
    (float)0
    );

  cv::Mat avg = cv::Mat(
    tmp_mat.cols*tmp_mat.rows,
    1,
    cv::DataType<float>::type,
    (float)0
    );
  /* reading an arbitrary number of images into a matrix, where 
  each column is an image */
  for (int i = 0; i < numImagesToUse; i++) {
      tmp_mat = Images.readImageAtIndex( i , imageSize );// skip around, get better sampling
      copyImageToCol(matrixWhereEveryColumnIsAnImage,
        i, 
        tmp_mat);
  }


  /* calculate average */
  averageMatrixWhereColumsAreImagesInto1xNVector( 
    matrixWhereEveryColumnIsAnImage,
    avg);

  /* subtract off average */
  subtractAverageVectorFromEveryColumnOfMatrix(
    matrixWhereEveryColumnIsAnImage,
    avg);

  cv::Mat transpose = matrixWhereEveryColumnIsAnImage.clone().t();
  std::cout << transpose.cols << "," << transpose.rows << std::endl;
  std::cout << matrixWhereEveryColumnIsAnImage.cols << "," << matrixWhereEveryColumnIsAnImage.rows << std::endl;
  
  /* calculate covariance matrix */
  /* note this is fat */
  cv::Mat covarianceMat = 
    matrixWhereEveryColumnIsAnImage * transpose;

  std::cout << "calculating svd" << std::endl;
  cv::SVD svd(covarianceMat);// u (use u?)vt w

  std::cout << "u cols:" << svd.u.cols << ", rows" << svd.u.rows << std::endl;
  double min = 0.0f, max =0.0f;
  cv::minMaxLoc(svd.vt, &min, &max);
  double difference = max - min;

  std::cout << "u min:" << min << ", max:" << max << std::endl;

  /* normalize to 0 - 255 */
  for (int i = 0; i < svd.vt.cols; i++ ) {
    for( int j = 0; j < svd.vt.rows; ++j) {
      svd.vt.at<float>(i,j) = (255.0)*(svd.vt.at<float>(i,j) - min)/(max-min);
    }
  }
   SimpleDisplay::renderImage(svd.vt);
  /* calculate weights for a face */


  /* print logic */
  // for (int j = 0; j < matrixWhereEveryColumnIsAnImage.rows; j++) {
  //     std::cout << 
  //       avg.at<float>(1,j);
  //   std::cout << ";" << std::endl;
  // }


  return 0;
}