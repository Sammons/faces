#include "CommonHeaders.hpp"

#include "face.hpp"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

cv::Mat norm_0_255(cv::Mat _src) {
    cv::Mat src = _src;
    // Create and return normalized image:
    cv::Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}

void face::serializeAndWriteEigenvectorsToFile( std::string filepath ) 
{

}

void face::readEigenvectorsFromFile( std::string filepath ) 
{

}

std::vector< std::string > face::readFileNamesFromDirectory( std::string dirpath )
{
return std::vector<std::string>();
}
void dumpMatProps(cv::Mat in) {
  std::cout << "type: " << in.type() << std::endl;
  std::cout << "dims: " << in.size() << std::endl;
}
/* assumes input is nx1 and of type float */
void copy1DMatInto2DFloat( cv::Mat& src, cv::Mat& dest, int rows, int cols ) {
  dest.create(rows, cols, src.type());
  for (int i = 0; i < src.rows; i++) {
    int row = i / cols;
    int col = i % cols;
    dest.at<float>(col, row) = src.at<float>(0, i);
  }

}
/* make sure the face is grey, dot products
each eigenvector against the image */
cv::Mat face::applyToImage( cv::Mat inputImage ) 
{

  //printf(" cols : %d\nrows: %d\n", this->eigenvectors.cols, this->eigenvectors.rows);
  cv::Mat results(
    1,this->numberOfComponentsToUse, CV_64FC1),
   image,
   scaledInputImage,
   mean;
  std::vector<float> weights;
  // std::cout << inputImage.rows << std::endl;
  cv::resize( inputImage, image, this->enforcedSize );
  mean = this->sampleMean.reshape( 1, image.rows );
  image.convertTo( image, CV_64FC1 );

  image -= mean;

  for (int i=0; i < this->eigenvectors.cols; i++) 
  {
    cv::Mat curCol;
    this->eigenvectors.col(i).copyTo(curCol);
    curCol = curCol.reshape(1, image.rows);
    results.col(i) = curCol.dot(image);
  }
  return results;
}

/* note - assumes greyscale, will resize to match so
can be whatever size */
void face::constructEigenFaceFromGreyFaceImagesInDir( 
  std::string collectionDir ) 
{
  std::vector< cv::Mat > inputImages;
  std::vector< int     > labels; /* will all be set to be 1 */

  fs::path collectionFullPath = fs::system_complete( fs::path( collectionDir ) );

  if ( !fs::is_directory( collectionFullPath ) ) {
    throw "The directory path for the images is invalid";
  }

  /* iterate over the files at the first level in the folder */
  fs::directory_iterator dirEndIter;/* I guess these are initialized to the end */
  for ( fs::directory_iterator directoryIter( collectionFullPath );
        directoryIter != dirEndIter && inputImages.size() < 100;
        ++directoryIter )
  {
    if ( fs::is_regular_file( directoryIter->status() ) ) 
    {
      /* read the images and scale them,
      should support pgm, jpg, png etc.
      read the opencv docs about imread for
      best luck. It does depend on the libs
      available on the computer this is run on. */
      cv::Mat currentImage = cv::imread( directoryIter->path().string(), 0 );
      cv::Mat scaledImage;
      cv::resize( currentImage, scaledImage, this->enforcedSize );
      inputImages.push_back( scaledImage.clone() );
      labels.push_back( 1 );
     /* all images in this collection are the same label, since we are
     just using the eigenfacerecognizer model to get the eigenface easily */
    }
  }

  printf("number of images being processed: %zu\n", inputImages.size());
  if ( inputImages.size() < 1 ) {
    throw "The image path had no images";
  }

  cv::Ptr< cv::FaceRecognizer > model = cv::createEigenFaceRecognizer(
    this->numberOfComponentsToUse,/* how many eigenvectors to keep */
    0 /* threshold is irrelevant since we don't
    use the prediction capaibility of the model */
    );

  printf("%s\n", "beginning heavy lifting");
  model->train( inputImages, labels );
  printf("%s\n", "finished heavy lifting");
  
  /* pull stats out of the model */
  this->eigenvalues  = model->getMat( "eigenvalues"  ).clone();
  this->eigenvectors = model->getMat( "eigenvectors" ).clone();
  this->sampleMean   = model->getMat( "mean" ).clone();
  this->eigenvectorsComputed = true;
  // std::cout << this->eigenvalues;
}


int main(int argc, char const *argv[])
{
  face eigenface = face();
  eigenface.constructEigenFaceFromGreyFaceImagesInDir("./croppedfaces");
  
  /* note that it is important that the images are greyscale, and
  are the same size as the samples */

  /* this will test to see if two similar images get
  a low euclidian distance */
  cv::Mat weightsIm1 = eigenface.applyToImage( cv::imread("./croppedfaces/yaleB01_P00A+070E+45.pgm",0) );
  cv::Mat weightsIm2 = eigenface.applyToImage( cv::imread("./croppedfaces/yaleB01_P00A+110E+40.pgm",0) );
  std::cout << weightsIm1 << std::endl << weightsIm2;
  std::cout << "norm is :" << cv::norm( weightsIm1, weightsIm2 ) << " for pictures of the same guy " << std::endl;
  // cv::Mat weightsIm3 = eigenface.applyToImage( cv::imread("./croppedfaces/yaleB01_P00A+070E+45.pgm",0) );
  // cv::Mat weightsIm4 = eigenface.applyToImage( cv::imread("./croppedfaces/yaleB02_P00A+110E+15.pgm",0) );
  
  // std::cout << "norm is :" << cv::norm( weightsIm3, weightsIm4 ) << " for pictures of diff people " << std::endl;

  return 0;
}