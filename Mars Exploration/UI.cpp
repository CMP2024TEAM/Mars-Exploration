#include "UI.h"
#include "enums.h"

UI::UI()
{
    IFile.open("input.txt");
}

inline void UI::ReadRoverData(MarsStation* Station)
{
    for (int rType = 0; rType < RoverCount; ++rType) {
        //Number of rovers in each rover type
        int n;
        IFile >> n;

        //Speed of each rover
        int rSpeed;
        for (int j = 0; j < n; ++j) {
            IFile >> rSpeed;
            Station->CreateRover(rovertype(rType), rSpeed);
        }
    }
}

inline void UI::ReadCheckupInfo(MarsStation* Station)
{
    int CheckupDays;
    IFile >> CheckupDays;
    Station.set(CheckupDays);
    for (int rType = 0; rType < RoverCount; ++rType) {
        IFile >> CheckupDays;
        Station.setCheckupDuration(rovertype(rType), CheckupDays);
    }
}

inline void UI::ReadAutoPromotion(MarsStation* Station)
{
    int AutoP;
    IFile >> AutoP;
    Station.setAutoP(AutoP);
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
            Station.CreateFormulationEvent(FormulationType, ED, ID, TLOC, MDUR, SIG);
            break;
        }
        case 'X': {
            IFile >> ED >> ID;
            Station.CreateCancellationEvent(ED, ID);
            break;
        }
        case 'P': {
            IFile >> ED >> ID;
            Station.CreatePromotionEvent(ED, ID);
            break;
        }
        }
    }
}

UI::~UI()
{
    IFile.close();
}
