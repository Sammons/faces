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

  //std::cout << "cols: " << this->eigenvectors.cols << std::endl;
  cv::Mat scaledInputImage;
  cv::Mat scaledInputImageAsFloats;
  cv::Mat results;
  results.create(this->eigenvectors.cols, 1, CV_64FC1 );/* nx1 vector */
  /* scale the input image to the eigenvector dims */
  cv::resize( inputImage, scaledInputImage, this->enforcedSize );
  
  cv::namedWindow("disp", cv::WINDOW_AUTOSIZE);
  for (int i = 0; i < this->eigenvectors.cols; ++i) {

    cv::Mat ev = this->eigenvectors.col(i).clone();
    // Reshape to original size & normalize to [0...255] for imshow.
    cv::Mat grayscale = norm_0_255(ev.reshape(1, this->enforcedSize.height));
    // Show the image & apply a Jet colormap for better sensing.
    cv::Mat cgrayscale;
    //cv::applyColorMap(grayscale, cgrayscale, cv::COLORMAP_JET);
    // Display or save:
    cv::imshow("disp",grayscale);
    // cv::Mat curEigenVect1d = this->eigenvectors.col(i);
    // cv::Mat curEigenVect1dAsInts;
    // cv::Mat curEigenVect;
    // scaledInputImage = scaledInputImage.reshape(0, 1);
    // scaledInputImage.convertTo( scaledInputImageAsFloats, CV_64FC1 );

    // // dumpMatProps( scaledInputImageAsFloats);
    // // dumpMatProps( this->sampleMean);
    // scaledInputImageAsFloats -= this->sampleMean;
    // curEigenVect1dAsInts = norm_0_255(curEigenVect1d);
    // curEigenVect1dAsInts.convertTo( curEigenVect1d, CV_64FC1 );

    // // /* make sure the mats are the same dimensions */
    // // copy1DMatInto2DFloat( 
    // //  curEigenVect1d,
    // //  curEigenVect,
    // //  this->enforcedSize.width,
    // //  this->enforcedSize.height );
    // // std::cout << std::endl << scaledInputImageAsFloats.type() << " " << scaledInputImageAsFloats.size();
    // // std::cout << std::endl << curEigenVect1d.type() << " " << curEigenVect1d.size();
    // results.at<float>( 1, i ) = ( scaledInputImageAsFloats.dot( curEigenVect1d.t() ) );

    cv::waitKey(0);
  }

  //results = norm_0_255(results);
  cv::Mat normalizedResult;
  //results.convertTo( normalizedResult, CV_64FC1 );
  // std::cout << "type:" << scaledInputImageAsFloats.type() << " " << curEigenVect.type() << std::endl;
  // std::cout << "size:" << scaledInputImageAsFloats.size() << " " << curEigenVect.size() << std::endl; 
  // std::cout << "dot:" << scaledInputImageAsFloats.dot( curEigenVect ) << std::endl;
  return normalizedResult;
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
        directoryIter != dirEndIter && inputImages.size() < 400;
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

  if ( inputImages.size() < 1 ) {
    throw "The image path had no images";
  }

  cv::Ptr< cv::FaceRecognizer > model = cv::createEigenFaceRecognizer(
    this->numberOfComponentsToUse,/* how many eigenvectors to keep */
    0 /* threshold is irrelevant since we don't
    use the prediction capaibility of the model */
    );

  model->train( inputImages, labels );


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
  // cv::Mat weightsIm2 = eigenface.applyToImage( cv::imread("./croppedfaces/yaleB01_P00A+110E+40.pgm",0) );
  // std::cout << weightsIm1 << std::endl << weightsIm2;
  // std::cout << "norm is :" << cv::norm( weightsIm1, weightsIm2 ) << " for pictures of the same guy " << std::endl;
  // cv::Mat weightsIm3 = eigenface.applyToImage( cv::imread("./croppedfaces/yaleB01_P00A+070E+45.pgm",0) );
  // cv::Mat weightsIm4 = eigenface.applyToImage( cv::imread("./croppedfaces/yaleB02_P00A+110E+15.pgm",0) );
  
  // std::cout << "norm is :" << cv::norm( weightsIm3, weightsIm4 ) << " for pictures of diff people " << std::endl;

  return 0;
}