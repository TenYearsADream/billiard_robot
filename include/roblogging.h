//Header file for robot arm trace and data comm logging functionality
//By Carlos Benaim, RoboLogics, ltd / Deep Learning Robotics. Copyright 2012-2016. All rights reserved.

#ifndef ROBLOGGING_H
#define ROBLOGGING_H
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <errno.h>
#include <algorithm>
#include <ctime>
#include <iomanip>
#define  DEFAULT_RobotTraceFileName "robtrace.log"
#define DEFAULT_RobotDataInspFileName "robdatacomm.log"

class roblogging
{
    public:
        //Constructs
        roblogging();
        roblogging(std::string filenamefortracing, std::string FileNameForDataCommInsp);
        virtual ~roblogging();

        //
        //logging
        bool TraceModeOn;
        bool InspectCommOn;

        int StartTrace();
        int StopTrace();
        int StartCommInspect();
        int StopCommInspect();
        int RecLog(std::string LogLine);
        int RecDataLog(std::string LogDataLine);
        std::ofstream RobotTraceFile;
        std::ofstream RobotDataInspFile;
        std::string RobotTraceFileName;
        std::string RobotDataInspFileName;
    protected:
    private:
};

#endif // ROBOT_LOGGING
