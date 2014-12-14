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
    float32 getValueAsNumber(Local<Value> arg) {
        return (localNum::Cast(arg))->NumberValue();
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
    Handle<Value> initialize_eigenfaces(const Arguments& args)
    {
        HandleScope scope;

        return scope.Close(Undefined());
    }

    /* if initialize has not been called,
    then initializes with fast configuration.
    Then scores the image */
    Handle<Value> evaluate_image(const Arguments& args) 
    {
        HandleScope scope; 
        try {
         
            if (args.length < 1) {
                return scope.Close(Handle<Array>());
            }
            if (!initialized)
            {
                unique_ptr<eigenface> localPtr(new eigenface(
                    32,
                    10,
                    CV_64FC1,
                    "default_fast.mdl",
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
          {"evaluate"  , evaluate_image}
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