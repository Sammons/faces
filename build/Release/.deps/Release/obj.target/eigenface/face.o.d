cmd_Release/obj.target/eigenface/face.o := g++ '-D_LARGEFILE_SOURCE' '-D_FILE_OFFSET_BITS=64' '-DBUILDING_NODE_EXTENSION' -I/home/catalyst/.node-gyp/0.10.33/src -I/home/catalyst/.node-gyp/0.10.33/deps/uv/include -I/home/catalyst/.node-gyp/0.10.33/deps/v8/include -I/usr/include/boost  -fPIC -Wall -Wextra -Wno-unused-parameter -pthread -m64 -O2 -fno-strict-aliasing -fno-tree-vrp -fno-omit-frame-pointer -std=c++11 -fexceptions `pkg-config --cflags opencv` -MMD -MF ./Release/.deps/Release/obj.target/eigenface/face.o.d.raw  -c -o Release/obj.target/eigenface/face.o ../face.cpp
Release/obj.target/eigenface/face.o: ../face.cpp ../CommonHeaders.hpp \
 ../face.hpp
../face.cpp:
../CommonHeaders.hpp:
../face.hpp:
