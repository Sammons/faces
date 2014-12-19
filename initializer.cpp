#include "common.hpp"
#include <node.h>
#include <v8.h>
#include "eigenface.hpp"

using namespace std;
using namespace v8;




namespace initialize {

    /* convenient persistence between JS calls */
    static unique_ptr<eigenface> eigenfaces;
    static bool initialized = false;

    typedef Handle<Value> (*ValueFunc) (const Arguments& args);
    typedef Handle<Array> (*ArrayFunc) (const Arguments& args);
    typedef String::Utf8Value v8String;
    
    /* helpers for extracting args */
    string getValueAsString(Local<Value> arg) 
    {
        return string(*(v8String(arg)));
    }
    float getValueAsNumber(Local<Value> arg) {
        return (Local<Number>::Cast(arg))->NumberValue();
    }

    /* three usages for initialize:

    given 1 argument: "fast" or "slow"
        slow default values become:
            128,
            120,
            CV_64FC1, <- this is the integer 6
            "default_slow.mdl",
            "./croppedfaces"
        fast default values become:
            32,
            10,
            CV_64FC1, <- this is the integer 6
            "default_fast.mdl",
            "./croppedfaces"

    given 1 argument: "path_to_.mdl"
        reads from a .mdl file to
        restore state of model

    given 4 arguments:
        uses the arguments directly to
        prep the model */
    Handle<Value> train(const Arguments& args)
    {
        HandleScope scope;
        std::string imageCollectionDir;
        int matWidth = 128;
        int numComponents = 50;
        int dataType = CV_64FC1;
        if (args.Length() == 0)
        {
            unique_ptr<eigenface> localPtr(new eigenface(
                matWidth,
                numComponents,
                dataType,
                imageCollectionDir)
            );
            eigenfaces = std::move(localPtr);
            initialized = true;
        }
        else if (args.Length() == 4)
        {
            matWidth = static_cast<int>(getValueAsNumber(args[0]));
            numComponents = static_cast<int>(getValueAsNumber(args[1]));
            dataType = static_cast<int>(getValueAsNumber(args[2]));
            imageCollectionDir = static_cast<int>(getValueAsNumber(args[3]));

            unique_ptr<eigenface> localPtr(new eigenface(
                matWidth,
                numComponents,
                dataType,
                imageCollectionDir)
            );
            eigenfaces = std::move(localPtr);
            initialized = true;
        }
        else 
        {
            throw "not enough arguments!";
        }
        return scope.Close(Undefined());
    }

    /* if initialize has not been called,
    then initializes with fast configuration.
    Then scores the image */
    Handle<Value> evaluate_image(const Arguments& args) 
    {
        HandleScope scope; 
        try {
         
            if (args.Length() < 1) {
                return scope.Close(Handle<Array>());
            }
            if (!initialized)
            {
                unique_ptr<eigenface> localPtr(new eigenface(
                    128,
                    50,
                    CV_64FC1,
                    "./croppedfaces")
                );
                eigenfaces = std::move(localPtr);
                initialized = true;
            }
            eigenfaces->train("./croppedfaces");
            cv::Mat score = eigenfaces->score(getValueAsString(args[0]));
            Handle<Array> jsArray = Array::New(score.cols);
            for (int i = 0; i < score.cols; ++i) 
            {
              jsArray->Set(i, Number::New(score.at<double>(0,i)));
            }
            return scope.Close(jsArray);
        } 
        catch (...) 
        {
            return scope.Close(Handle<Array>());
        }
    }

    map <
      const char*
    , ValueFunc
    > arrayExportables = 
    // these are the Value functions which are exposed to the javascript
    {
          {"score"  , evaluate_image},
          {"train"  , train}
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