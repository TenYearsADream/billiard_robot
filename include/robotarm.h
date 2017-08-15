//Header file for robot functionality
//By Carlos Benaim, Deep Learning Robotics, ltd. Copyright 2012-2016. All rights reserved.

#ifndef ROBOTARM_H
#define ROBOTARM_H
#include "roblogging.h"
#include "robotarm_global.h"
#include <netinet/in.h>
#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>
//#include <stdlib.h>	// for itoa() call
#define DEFAULT_Robot_IP "192.168.100.10" // "192.168.2.100" // //"192.168.100.10" //
#define DEFAULT_RobotModel MHL5
#define DEFAULT_ControllerModel DX100
#define DEFAULT_RobotPort 80 //80 //2000
#define DEFAULT_NumOfInternalAxes 6  //robot axes
#define SOCKET_RECEIVE_TIMEOUT 10 // Timeout in seconds for receive from socket. Default=10 seconds.
#define SOCKET_SEND_TIMEOUT 20 //Timeout in seconds for send to socket. Default=20 seconds.


// Structure for field Type returned from some robot commands
typedef struct {bool Flip; bool NoFlip; bool UpperArm; bool LowerArm; bool Front; bool Back; bool R_LT180; bool R_GE180; bool T_LT180; bool T_GE180; bool S_LT180; bool S_GE180;
    } PType;
;
// Structure for coordinates in mm
typedef struct {double x; double y; double z;} POINT3D;

// Structure for wrist angle
typedef struct {float x; float y; float z;} RANGLE;

// Robot Position in Pulses
typedef struct { int S; int L; int U; int R; int B; int T; int E;} ROBPOSPULSE;

// Robot Position in Cartesian
typedef struct {int CoordinateNum; POINT3D Coord; RANGLE R; float ReAngle; PType Type;} ROBPOSXYZ;

//External Axes Position in Pulses. For travel axis, A7, A8 and A9 units are mm instead of pulses.
typedef struct { double A7; double A8; double A9; int A10; int A11; int A12; int A13;} EXTPOSPULSE;  //CHECK ROBOT TO SEE RETURN TYPES FOR THIS AXES

//Structure for Answer from RStat Command
typedef struct {int ExitCode;
 bool Step; bool OneCycle; bool Auto; bool Running; bool SafetySpeedOperation; bool Teach; bool Play; bool CommandRemote;
  bool HoldProgrammingPendant; bool HoldExternal; bool HoldCommand; bool AlarmOn; bool ErrorOn; bool ServoOn;
  } RSTATS;

//Structure for answer from RALARM Command
typedef struct {int ExitCode; int ErrorCode; int ErrorData; int AlarmCode1; int AlarmData1; int AlarmCode2; int AlarmData2; int AlarmCode3; int AlarmData3; int AlarmCode4; int AlarmData4;
  } RALARM ;

//Structure for Answer from RPOSJ command
typedef struct {int ExitCode; ROBPOSPULSE Axis; EXTPOSPULSE ExtAxis;
  } RPOSJ ;

//Structure for Answer from RPOSC command
typedef struct {int ExitCode; POINT3D Coord; RANGLE R;
                PType Type; int Tool; EXTPOSPULSE ExtAxis; float ReAngle;
  } RPOSC ;
//Structure for Answer from RJSEQ command
typedef struct {int ExitCode; std::string JobName; int LineNum; int StepNum;
  } RJSEQ;

typedef struct {int ExitCode; POINT3D Org; RANGLE OrgR; ; PType OrgType; POINT3D XX; RANGLE XXR ; PType XXType; POINT3D XY; RANGLE XYR; PType XYType;
                int ToolNum; EXTPOSPULSE ExtAxis ;
                bool ExternalAxis;  //bool OrgUpperArm; bool OrgLowerArm; bool OrgFront; bool OrgBack; bool OrgR_LT180; bool OrgT_LT180; bool OrgS_LT180;
  } RUFRAME;

typedef struct {int ExitCode; bool PulseType; bool CartesianType; ROBPOSPULSE Pulses; ROBPOSXYZ Cartesian;  int ToolNum ;

                char ValueByte; int ValueInt; std::string ValueString; double ValueDouble; float ValueFloat;
  } SAVEV;


typedef struct {float MotionSpeedPerc; ROBPOSXYZ Pos; int ToolNum; EXTPOSPULSE ExtAxis;} JOINTMOTION;

typedef struct {int SpeedType; float Speed;  ROBPOSXYZ Pos; int ToolNum; EXTPOSPULSE ExtAxis; }  LINEARMOTION;
typedef struct {float MotionSpeedPerc; ROBPOSPULSE Axis; int ToolNum; EXTPOSPULSE ExtAxis;} PULSEMOTION;
typedef struct {int SpeedType; float Speed; ROBPOSPULSE Axis; int ToolNum; EXTPOSPULSE ExtAxis;} LINPULSEMOTION;

typedef struct {ROBPOSXYZ ORG; ROBPOSXYZ XX; ROBPOSXYZ XY; int ToolNum; EXTPOSPULSE ExtAxis; } WUFRAME;


class robotarm: public roblogging
{

public:
    enum RobotModels {MHL5};
    enum CotrollerModels {DX100};
    enum RobotMode {TeachMode=1, PlayMode=2};
    enum CycleType {StepCycle=1, OneCycle=2, AutoCycle=3};
    enum MotionSpeedType {SpeedType=0, PostureSpeedType=1}; //0: V (speed), 1:VR (posture speed)
    //Commands
        //Monitor/status
    RSTATS rstats();
    RALARM ralarm();
    RPOSJ rposj();
    RPOSC rposc(int CoordinateSystemNumber, bool WithExternalAxis=false);
    RJSEQ rjseq();
    RUFRAME ruframe(int UserCoordNum);
    SAVEV  savev(std::string TypeOfVariable, int VariableNum);
        //Control
    int hold(bool TurnHoldStatusOnOff);
    int reset();
    int cancel();
    int mode(int ModeToSelect);
    int cycle(int CycleToSelect);
    int svon(bool OnOff);
    int hlock(bool OnOff);
    int mdsp(std::string msgToPendant);
    int start(std::string JobName);
    int movj(JOINTMOTION TargetMove);
    int movl(LINEARMOTION TargetLinMove );
    int imov(LINEARMOTION TargetIncrMove);
    int pmovj(PULSEMOTION TargetPulseMove);
    int pmovl(LINPULSEMOTION TargetPLinMove);
        // Job Edit
    int deletejob(std::string JobToDelete);
    int cvtrj(std::string JobName, int UserCoordNum);
    int cvtsj(std::string JobName, int ConversionMethod, int RefPosVariableNum );
    int wuframe(WUFRAME UsrCoordDataToWrite);
    //template <class T> int loadv(T inputparam);
    //int loadv(std::string TypeOfVariable, int VariableNum, std::string ValueString);
    int loadv(int VariableNum, unsigned char ValueToUpload);
    int loadv(int VariableNum, int ValueToUpload);
    int loadv(int VariableNum, float ValueToUpload);
    int loadv(int VariableNum, long int ValueToUpload);
    int loadv(int VariableNum, std::string ValueToUpload);
    int loadv(std::string TypeOfVariable, int VariableNum, ROBPOSXYZ Cartesian, int ToolNum );
    int loadv(std::string TypeOfVariable, int VariableNum, ROBPOSPULSE Pulses, int ToolNum );
        // I/O commands
        // Try using vector instead of int *
    int ioread(int StartingContactPointNum);
    int iowrite(int StartingContactPointNum, int ContactPointsToWrite, int * ContactPointsData );

    //Constructs
    robotarm();
    robotarm(RobotModels , CotrollerModels , std::string RobotIPAddr , int RobotPort, int NumOfInternalAxesParam ,std::string RobotTraceFileNameParam, std::string RobotDataInspFileNameParam);
    robotarm(std::string RobotIPAddressParam, int RobotPortParam, std::string RobotTraceFileNameParam, std::string RobotDataInspFileNameParam);


    virtual ~robotarm();

    //Gets
    std::string GetRobotIPAddress();
    int GetRobotPort();
    int GetSockID();

    //Structs
    RobotModels Model;
    CotrollerModels ControllerModel;
    struct sockaddr_in serv_addr;

    //Parsing
    std::string DataParseToBytes( std::string  SringToParse);
    int getBit(char ByteToRead, int PositionInBit);
    int *ParseToInts(std::string StringToParse, int &NumOfFieldFound);
    double *ParseToDoubles(std::string StringToParse, int &NumOfFieldFound);
    PType ParseToPType(int ByteToParse );
    std::string ParsePTypeToByte(PType PtypeToParse);

    // Communication
    int Connect(bool KeepAlife);
    int Disconnect();
    int SendLine(std::string LineToSend, bool AddLineFeedAtEndOfString);
    int ReceiveLine(std::string& StoreAnswerHere);

    //Tools
    std::string DisplayCRLF(std::string InputString);
    std::string Repeat(std::string s, int n);
    bool IsIPAddressValid();

protected:
    std::string RobotIPAddress;
    int SockID;
    int RobotRun( std::string, std::string, std::string& );
    int RobotPort;
    int NumOfInternalAxes;
private:
    int init();
    std::string RobotCommandLine;
    std::string RobotCommandData;
    bool ValidIPAddress;
};
#endif


