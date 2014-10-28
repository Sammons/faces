#include "CommonHeaders.hpp"

#include "face.hpp"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

void face::serializeAndWriteEigenvectorsToFile( std::string filepath ) 
{

}

void face::readEigenvectorsFromFile( std::string filepath ) 
{

}

std::vector< std::string > face::readFileNamesFromDirectory( std::string dirpath )
{

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
        directoryIter != dirEndIter;
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
  cv::Mat eigenvalues  = model->getMat( "eigenvalues"  );
  cv::Mat eigenvectors = model->getMat( "eigenvectors" );
 // std::cout << std::endl << eigenvalues << std::endl;
  cv::Mat sampleMean   = model->getMat( "mean"         );
  //std::cout << sampleMean;
  std::cout << eigenvalues;
  /* write the eigenvectors to a file */

  //this->serializeAndWriteEigenvectorsToFile( outPutFilePathForEigenvectors );

}


int main(int argc, char const *argv[])
{
  face eigenface = face();
  eigenface.constructEigenFaceFromGreyFaceImagesInDir("./croppedfaces");
  return 0;
}