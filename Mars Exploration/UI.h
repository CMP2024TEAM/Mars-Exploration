#pragma once
#include<iostream>
#include<fstream>
#include "enums.h"

class MarsStation;
using namespace std;
class UI {
private:
    ifstream IFile;
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