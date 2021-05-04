#include "UI.h"
#include "MarsStation.h"

MissionType UI::ParseMissionType(char c_type)
{
    switch (c_type) {
    case 'E': return MissionType::Emergency;
    case 'M': return MissionType::Mountainous;
    case 'P': return MissionType::Polar;
    }
}

UI::UI()
{
    IFile.open("input.txt");
}

inline void UI::ReadRoverData(MarsStation* Station)
{
    for (int rType = 0; rType < (int)RoverType::RoverType_MAX; ++rType) {
        //Number of rovers in each rover type
        int n;
        IFile >> n;

        //Speed of each rover
        int rSpeed;
        for (int j = 0; j < n; ++j) {
            IFile >> rSpeed;
            Station->CreateRover(RoverType(rType), rSpeed);
        }
    }
}

inline void UI::ReadCheckupInfo(MarsStation* Station)
{
    int CheckupDays;
    IFile >> CheckupDays;
    Station->SetNumberOfMissionsTheRoverCompletesBeforeCheckup(CheckupDays);
    for (int rType = 0; rType < (int)RoverType::RoverType_MAX; ++rType) {
        IFile >> CheckupDays;
        Station->SetCheckupDuration(RoverType(rType), CheckupDays);
    }
}

inline void UI::ReadAutoPromotion(MarsStation* Station)
{
    int AutoP;
    IFile >> AutoP;
    Station->SetAutoP(AutoP);
}

inline void UI::ReadEvents(MarsStation* Station)
{
    //Number of events to read
    int n;
    IFile >> n;

    char EventType;
    char FormulationType; int ED; int ID; int TLOC; int MDUR; int SIG;
    for (int i = 0; i < n; ++i) {
        IFile >> EventType;
        switch (EventType) {
        case 'F': {
            IFile >> FormulationType >> ED >> ID >> TLOC >> MDUR >> SIG;
            Station->CreateFormulationEvent(ParseMissionType(FormulationType), ED, ID, TLOC, MDUR, SIG);
            break;
        }
        case 'X': {
            IFile >> ED >> ID;
            Station->CreateCancellationEvent(ED, ID);
            break;
        }
        case 'P': {
            IFile >> ED >> ID;
            Station->CreatePromotionEvent(ED, ID);
            break;
        }
        }
    }
}

UI::~UI()
{
    IFile.close();
}
