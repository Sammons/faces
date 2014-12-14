#include "common.hpp"
#include "face.hpp"

namespace fs = boost::filesystem;

eigenface::eigenface(
    int n,               /* images are scaled to nxn */
    int numComponents,   /* how many eigenvectors */
    int dataType,        /* type to use in math */
    std::string loadPath /* where to load from*/
    )
{
    /* configure */
    initialize(n, numComponents, dataType, loadPath);

    /* try to restore training */
    this->attemptToReadEigenfaces();
}

eigenface::eigenface(
    int n,                 /* images are scaled to nxn */
    int numComponents,     /* how many eigenvectors */
    int dataType,          /* type to use in math */
    std::string savePath,  /* where to save state */
    std::string collection /* path to train from */
    )
{
    /* configure */
    this->initialize(n, numComponents, dataType, loadPath);

    /* train */
    this->train(collection);
}

void eigenface::initialize(  
    int n,               /* images are scaled to nxn */
    int numComponents,   /* how many eigenvectors */
    int dataType,        /* type to use in math */
    std::string loadPath) /* where to load from*/
{
    /* configuration */
    this->trained = false;
    this->numComponents = numComponents;
    this->size = cv::Size(n,n);
    this->dataType = dataType;
    this->savePath = loadPath;
}

/* checks for save file, and attempts to read from it */
void eigenface::attemptToReadEigenfaces()
{

}

/* takes the image path, reads in the image as greyscale
and resizes the image to match the this->size parameter.
Then subtracts the sampleMean. Following that it computes
the dot product of each eigenvector with the pre-processed
image to produce a 1x(this->numComponents) matrix
of weights, where the numComponents is the same
as the number of eigenvectors, which is the same
as the number of principle compontents used */
cv::Mat face::score( std::string imagePath ) 
{

    cv::Mat
        image,
        mean,
        inputImage = imread(imagePath, CV_LOAD_IMAGE_GRAYSCALE),
        results(1, this->numComponents, this->dataType);

    if (!this->trained)
    {
        throw "not trained!";
    }

    /* resize image to be nxn */
    cv::resize( inputImage, image, this->size );

    /*convert mean to be nxn */
    mean = this->sampleMean.reshape( 1, image.rows );

    /* ensure image & mean are the same type so that ops are valid*/
    image.convertTo( image, this->dataType );
    mean.convertTo ( mean,  this->dataType );

    /* subtract mean from the incoming image */
    image -= mean;

    /* dot product each eigenvector with image */
    for (int i=0; i < this->eigenvectors.cols; i++) 
    {
        cv::Mat curCol;
        this->eigenvectors.col(i).copyTo(curCol);

        curCol = curCol.reshape(1, image.rows);
        results.col(i) = curCol.dot(image);
    }

    /* return resulting vector of weights */
    return results;
}

/* reads all of the images in the given directory into
a vector, which is then injected into a opencv training algorithm
which performs a PCA on covariance matrix resulting from
this collection of images. We extract the eigenvectors
from that model, along with the sampleMean for the collection,

The labels are not really used, they are just there to satisfy
the method signature for the opencv train method */
void face::train(std::string imageCollectionDirectory) 
{
    std::vector<cv::Mat> inputImages;
    std::vector<int> labels; /* will all be set to be 1 */

    fs::path collectionFullPath = 
        fs::system_complete( 
            fs::path(imageCollectionDirectory));

    if (!fs::is_directory(collectionFullPath)) 
    {
        throw "The directory path for the images is invalid";
    }

    /* iterate over the files at the first level in the folder */
    fs::directory_iterator dirIter( collectionFullPath );
    for ( ;dirIter != fs::directory_iterator(); ++directoryIter )
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
            cv::resize( currentImage, scaledImage, this->size );
            inputImages.push_back( scaledImage.clone() );

            /* all images in this collection are the same label, since we are
            just using the eigenfacerecognizer model to get the eigenfaces easily */
            labels.push_back( 1 );
        }
    }

    if ( inputImages.size() < 1 ) 
    {
        throw "The image path had no images";
    }

    cv::Ptr< cv::FaceRecognizer > model = cv::createEigenFaceRecognizer(
        this->numComponents,/* how many eigenvectors to keep */
        0 /* threshold is irrelevant since we don't
        use the prediction capaibility of the model */
        );

    model->train( inputImages, labels );

    /* pull stats out of the model */
    this->eigenvalues  = model->getMat( "eigenvalues"  ).clone();
    this->eigenvectors = model->getMat( "eigenvectors" ).clone();
    this->sampleMean   = model->getMat( "mean" ).clone();
    this->trained = true;
}