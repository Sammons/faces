cmd_Makefile := cd ..; /usr/lib/node_modules/node-gyp/gyp/gyp_main.py -fmake --ignore-environment "--toplevel-dir=." -I/home/catalyst/Desktop/faces/build/config.gypi -I/usr/lib/node_modules/node-gyp/addon.gypi -I/home/catalyst/.node-gyp/0.10.33/common.gypi "--depth=." "-Goutput_dir=." "--generator-output=build" "-Dlibrary=shared_library" "-Dvisibility=default" "-Dnode_root_dir=/home/catalyst/.node-gyp/0.10.33" "-Dmodule_root_dir=/home/catalyst/Desktop/faces" binding.gyp
