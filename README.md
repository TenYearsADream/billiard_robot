# billiard_robot
Robotic arm plays billiard

# compile
mkdir build; cd build;
g++ -w -o main ../src/main.cpp ../src/color_tracker.cpp -I ../include/ `pkg-config --cflags --libs opencv`
