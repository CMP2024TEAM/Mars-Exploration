#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "enums.h"
#include "Mission.h"

//Forward class declaration
class MarsStation;

//Maximum number of Mission types
constexpr int mTypeMax = static_cast<int>(MissionType::MAX);
//Maximum number of Rover Types
constexpr int rTypeMax = static_cast<int>(RoverType::MAX);

class UI {
private:
    /*=================================================
                         Members
    =================================================*/
    std::ifstream IFile;
    std::ofstream OFile;
    OutputType OType;

    /*=================================================
                         Buffers
    =================================================*/  
    int TotalFailures, FailedCount, WaitingCount, InExecutionCount, AvailableCount, CheckUpCount, MaintenanceCount, CompletedCount;
    std::string FailedMission_Buf[mTypeMax];
    std::string WaitingMission_Buf[mTypeMax];
    std::string InExecutionMiss_Rov_Buf[mTypeMax];
    std::string CompletedMission_Buf[mTypeMax];
    std::string AvailableRovers_Buf[rTypeMax];
    std::string InCheckupRovers_Buf[rTypeMax];
    std::string MaintenanceRovers_Buf[rTypeMax];

    /*=================================================
                        Constants
    =================================================*/
    //The respective enclosing character for each mission type
    static const char EnclosingChar[mTypeMax][2];
    //Linebreak between each line of output
    static const std::string LineBreak;
    //Linebreak between each critical line of output
    static const std::string WarningBreak;

    /*=================================================
                       Input Utility
    =================================================*/
    MissionType ParseMissionType(char c_type);
    void ReadRoverData(MarsStation* Station);
    void ReadCheckupInfo(MarsStation* Station);
    void ReadAutoPromotion(MarsStation* Station);
    void ReadEvents(MarsStation* Station);

    /*=================================================
                      Output Utility
    =================================================*/
    //Add all other information to buffers. This DOES NOT load completed
    //Mission information
    void FillBuffersFromStation(MarsStation* Station);
    //Output buffer content to console in proper format
    void FormatBuffersToConsole();
    //Clear all buffer content and reset counts. This DOES NOT clear completed
    //Missions buffers
    void ClearBuffers();
    //Waits for some input (To be used by interactive mode)
    void WaitForUserInput();
    //Print final statistics at the end of the simulation
    void PrintStatistics(MarsStation* Station);

public:
    UI(OutputType OutputT, std::string IFileName = "input", std::string OFileName = "output");
    
    //Read all parameters from input file
    void ReadAll(MarsStation* Station); 

    //Print initial message to console and header line to file
    void InitialDisplayMessage();

    //Log any occurence of a mission failure
    void LogMissionFailure(Mission* const FailedMission);

    //Get all station info at the end of the day and output to file/console
    void PrintCurrentDay(MarsStation* Station);
    
    //Print final statistics to file
    void FinalDisplayMessage(MarsStation* Station);

    ~UI();
};