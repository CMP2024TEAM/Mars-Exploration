#pragma once
#include<iostream>
#include<fstream>
#include"MarsStation.h"

using namespace std;
class UI {
private:
    ifstream IFile;
    //ostream OFile;

    MissionType::MissionType ParseMissionType(char c_type);
public:
    UI();

    void ReadRoverData(MarsStation* Station);
    void ReadCheckupInfo(MarsStation* Station);
    void ReadAutoPromotion(MarsStation* Station);
    void ReadEvents(MarsStation* Station);

    ~UI();
};