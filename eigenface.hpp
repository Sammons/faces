#pragma once

class eigenface
{

public:

eigenface(
  int n,               /* images are scaled to nxn */
  int numComponents,   /* how many eigenvectors */
  int dataType,        /* type to use in math */
  std::string loadPath /* where to load from*/
  );

eigenface(
  int n,                 /* images are scaled to nxn */
  int numComponents,     /* how many eigenvectors */
  int dataType,          /* type to use in math */
  std::string savePath,  /* where to save state */
  std::string collection /* path to train from */
  );

/* serialize and save eigenvectors */
void save( std::string filepath );

/* load up eigenvectors */
void load( std::string filepath );

/* train and produce new eigenvectors */
void train( std::string imageCollectionDirectoryPath );

/* note the image does not have to be greyscale,
 they are read in as greyscale. Size does not matter either
 since they are resized to the this->size proportions. */
cv::Mat score( std::string imagePath );

private:

void initialize(  
  int n,               /* images are scaled to nxn */
  int numComponents,   /* how many eigenvectors */
  int dataType,        /* type to use in math */
  std::string loadPath /* where to load from*/
  );

cv::Mat eigenvectors;/* used to compute weights */

cv::Mat eigenvalues;/* not used */

cv::Mat sampleMean;/* mean from training collection */

/* standard mat size to work with
All images read in are handled in this proportion. Must
be square */
cv::Size size; 

/* number of principle components to use
when scoring an image, this is the same as
the number of eigenvectors */
int numComponents;

/* whenever train is called, it serializes the
eigenvectors to a file; this is where it puts them */
std::string savePath;

/* cv type to work with when doing math */
int dataType;

/* have we got stats to use? */
bool trained;

};
