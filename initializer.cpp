#include "CommonHeaders.hpp"
#include <node.h>
#include <v8.h>
#include "face.hpp"

using namespace std;
using namespace v8;


// declare value function pointer type as ValueFunc
typedef Handle<Value> (*ValueFunc) (const Arguments& args);
typedef Handle<Array> (*ArrayFunc) (const Arguments& args);

typedef String::Utf8Value v8String;

namespace initialize {

    string getValueAsString(Local<Value> arg) 
    {
      return string(*(v8String(arg)));
    }

    Handle<Value> evaluate_image_path(const Arguments& args) 
    {
      HandleScope scope;  
      try {
        face eigenfaces = face();
        eigenfaces.learnFromGreyFaceImagesInDir("./croppedfaces");
        cv::Mat score = eigenfaces.evaluateImage( cv::imread(getValueAsString(args[0]),CV_LOAD_IMAGE_GRAYSCALE) );
        Handle<Array> jsArray = Array::New(eigenfaces.getNumEigenVectors());
        for (int i = 0; i < eigenfaces.getNumEigenVectors(); ++i) 
        {
          jsArray->Set(i, Number::New(score.at<double>(0,i)));
        }
        return scope.Close(jsArray);
      } catch (...) {
        return scope.Close(Handle<Array>());
      }
    }

    map <
      const char*
    , ValueFunc
    > arrayExportables = 
    // these are the Value functions which are exposed to the javascript
    {
          {"evaluate"  , evaluate_image_path}
    };
    

    void ExportValueFunction(
      Handle<Object> exports
    , Handle<Value> (*func)(const Arguments& args)
    , const char* name
    ) {
    //export function, expose as name
    exports->Set(
          String::NewSymbol(name)
        , FunctionTemplate::New(func)->GetFunction()
        );
    }
    
    void init(Handle<Object> exports) {
        map <
          const char*
        , ValueFunc
        >::iterator iter;

        // expose the Value functions
        for (
            iter = arrayExportables.begin();
            iter != arrayExportables.end();
            iter++
            ) {
            ExportValueFunction(exports, iter->second, iter->first); 
        }
    }


}

NODE_MODULE(eigenface, initialize::init)