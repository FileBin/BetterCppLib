mkdir build
cd build
cmake -S "../CMakeSource" -B "default" -DCMAKE_INSTALL_PREFIX=$ENV{HOME}/GameEngine
