class face
{

public:


void serializeAndWriteEigenvectorsToFile( std::string filepath );
void readEigenvectorsFromFile( std::string filepath );
void constructEigenFaceFromGreyFaceImagesInDir( 
  std::string collectionDir );
cv::Mat applyToImage( cv::Mat );

private:

bool eigenvectorsComputed = false;
cv::Mat eigenvectors;
cv::Mat eigenvalues;
cv::Mat sampleMean;

/* all images this object handles are forced to be
this scale */
static const cv::Size enforcedSize;
static const int numberOfComponentsToUse;

std::vector< std::string > readFileNamesFromDirectory( std::string dirpath );

};

/* static initializers */
const cv::Size face::enforcedSize = cv::Size(32,32);
const int face::numberOfComponentsToUse = 2;