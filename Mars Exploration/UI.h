#pragma once
#include<iostream>
#include<fstream>
#include "enums.h"

class MarsStation;

class UI {
private:
    std::ifstream IFile;
    //ostream OFile;

    MissionType ParseMissionType(char c_type);
public:
    UI();

    void ReadRoverData(MarsStation* Station);
    void ReadCheckupInfo(MarsStation* Station);
    void ReadAutoPromotion(MarsStation* Station);
    void ReadEvents(MarsStation* Station);

    ~UI();
};