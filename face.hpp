#pragma once
class face
{

public:

face();
void serializeAndWriteEigenvectorsToFile( std::string filepath );
void readEigenvectorsFromFile( std::string filepath );
void learnFromGreyFaceImagesInDir( 
  std::string collectionDir );
cv::Mat evaluateImage( cv::Mat );
int getNumEigenVectors();

private:

bool eigenvectorsComputed = false;
cv::Mat eigenvectors;
cv::Mat eigenvalues;
cv::Mat sampleMean;

/* all images this object handles are forced to be
this scale */
cv::Size enforcedSize;
int numberOfComponentsToUse;

std::vector< std::string > readFileNamesFromDirectory( std::string dirpath );

};
