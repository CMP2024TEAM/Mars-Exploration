#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include "enums.h"

class MarsStation;

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
    std::string WaitingMission_Buf[static_cast<size_t>(MissionType::MAX)];
    std::string InExecutionMiss_Rov_Buf[static_cast<size_t>(MissionType::MAX)];
    std::string CompletedMission_Buf[static_cast<size_t>(MissionType::MAX)];
    std::string AvailableRovers_Buf[static_cast<size_t>(RoverType::MAX)];
    std::string InCheckupRovers_Buf[static_cast<size_t>(RoverType::MAX)];
    /*=================================================
                         Constants
    =================================================*/
    static const char EnclosingChar[static_cast<size_t>(MissionType::MAX)][2];
    static const std::string LineBreak;
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
    void FormatBuffersToConsole();
    void ClearBuffers();
    void Wait();
    //Print final statistics at the end of the simulation
    void PrintStatistics(MarsStation* Station);
public:
    UI(OutputType OutputT);
    UI(OutputType OutputT, std::string IFileName, std::string OFileName);
    
    //Read all parameters from input file
    void RealAll(MarsStation* Station); 

    //Get all station info at the end of the day and output to file/console
    void Print(MarsStation* Station);
    
    //TODO: Add initial print to console and final print to console
    //TODO: When doing final print to console should also save statistics to OFile
    ~UI();
};