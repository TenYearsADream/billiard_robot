#ifndef __DLRARM__
#define __DLRARM__

#include <robotarm.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <time.h>
#include "opencv2/core.hpp"

using namespace std;
using namespace cv;


typedef struct Pose_struct
{
    Point3f position;
    Point3f orientation;
} Pose;


class DLRarm
{
    public:
        DLRarm();
        DLRArm(string config_file);
        ~DLRarm();
        void move_to(Pose pose);
        void move_relative(Pose pose);

    private:
        float speed;
        Pose curr_pose;
        robotarm myRobot;

        int robot_bringup(string address = "192.168.100.10", int port = 80);
        void waitUntilPosReached();
};


#endif