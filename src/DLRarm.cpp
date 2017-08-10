#include "DLRarm.hpp"


DLRarm::DLRarm()
{
    Pose pose;
    pose.position = Point3f(0,0,0);
    pose.orientation = Point3f(0,0,0);
    curr_pose = pose;
    speed = 1;
    robot_bringup();
}



DLRarm::DLRarm(string config_file)
{
    FileStorage fs;
    fs.open(config_file, FileStorage::READ);
    fs["speed"] >> speed;
    fs["curr_pose"] >> curr_pose;
    fs.release();
    robot_bringup();
}


DLRarm::~DLRarm()
{
    myRobot.svon(false);
    myRobot.Disconnect();
}



int robot_bringup(string address /*= "192.168.100.10"*/, int port /*= 80*/)
{
    myRobot = robotarm(address, port, "thisTraceFile.log", "thisCommLog.log");
    int con_code = myRobot.Connect(true);
    myRobot.svon(true);

    return con_code;
}



void DLRarm::move_to(Pose pose)
{
    JOINTMOTION mot;
    mot.Pos.Coord.x = pose.position.x;
    mot.Pos.Coord.y = pose.position.y;
    mot.Pos.Coord.z = pose.position.z;
    mot.Pos.R.x = pose.orientation.x;
    mot.Pos.R.y = pose.orientation.y;
    mot.Pos.R.z = pose.orientation.z;
    mot.MotionSpeedPerc = speed;

    myRobot.movj(mot);
    waitUntilPosReached(pose.position);
    curr_pose = pose;
}



void DLRarm::move_relative(Pose pose)
{
    Pose rel_pose;
    rel_pose.position = pose.position + curr_pose.position;
    rel_pose.orientation = pose.orientation + curr_pose.orientation;

    move_to(rel_pose);
}



void DLRarm::waitUntilPosReached(Point3f target_pos)
{
    while (1 > 0)
    {
        Point3f rob_pos(myRobot.rposc(0, false).Coord.x,
                        myRobot.rposc(0, false).Coord.y,
                        myRobot.rposc(0, false).Coord.z);

        if (norm(rob_pose - target_pos) < 1)
            break;

        usleep(3);
    }
}