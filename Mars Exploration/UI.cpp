#include "UI.h"
#include "MarsStation.h"
#include "DataStructures/Stack.h"

#define BR_LENGTH 25

const char EnclosingChar[static_cast<size_t>(MissionType::MAX)][2] = {
    //Emergency
    {'[',']'},
    //Mountainous
    {'{','}'},
    //Polar
    {'(',')'}
};

const std::string LineBreak(BR_LENGTH, '-');

inline MissionType UI::ParseMissionType(char c_type)
{
    switch (c_type) {
    case 'E': return MissionType::Emergency;
    case 'M': return MissionType::Mountainous;
    case 'P': return MissionType::Polar;
    default: throw(std::runtime_error("Unknown Mission type input: " + c_type));
    }
}

inline void UI::ReadRoverData(MarsStation* Station)
{
    for (int rType = 0; rType < static_cast<int>(RoverType::MAX); ++rType) {
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
    for (int rType = 0; rType < static_cast<int>(RoverType::MAX); ++rType) {
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

void UI::FillBuffersFromStation(MarsStation* Station)
{
    Mission* MissionPtr;
    Rover* RoverPtr;

    int mType;
    for (mType = 0; mType < static_cast<int>(MissionType::MAX); ++mType) {
        Queue<Mission*> Waiting_Missions = Station->GetWaitingMissions(mType);
        while (Waiting_Missions.dequeue(MissionPtr))
            WaitingMission_Buf[mType] += std::to_string(MissionPtr->GetID()) + ", ";
    }

    PriorityQueue<Mission*> InExec_Missions = Station->GetInExecutionMissions();
    while (InExec_Missions.dequeue(MissionPtr)) {
        mType = static_cast<int>(MissionPtr->GetMissionType());
        InExecutionMiss_Rov_Buf[mType] += std::to_string(MissionPtr->GetID()) + '/' + std::to_string(MissionPtr->GetRover()->GetID()) + ", ";
    }


    for (int rType = 0; rType < static_cast<int>(RoverType::MAX); ++rType) {
        Queue<Rover*> Available_Rovers = Station->GetAvailableRovers(rType);
        Queue<Rover*> InCheckup_Rovers = Station->GetInCheckupRovers(rType);
        while (Available_Rovers.dequeue(RoverPtr))
            AvailableRovers_Buf[rType] += std::to_string(RoverPtr->GetID()) + ", ";
        while (InCheckup_Rovers.dequeue(RoverPtr))
            InCheckupRovers_Buf[rType] += std::to_string(RoverPtr->GetID()) + ", ";
    }
}

void UI::FormatBuffersToConsole()
{
    std::cout << " Waiting Misions: ";
    for (int mType = 0; mType < static_cast<int>(MissionType::MAX); ++mType) {
        if (!WaitingMission_Buf[mType].empty()) {
            WaitingMission_Buf[mType].pop_back(); WaitingMission_Buf[mType].pop_back();
            std::cout << EnclosingChar[mType][0] << WaitingMission_Buf[mType] << EnclosingChar[mType][1] << ' ';
        }
    }

    std::cout << '\n' << LineBreak << '\n';

    std::cout << " In-Execution Missions/Rovers: ";
    for (int mType = 0; mType < static_cast<int>(MissionType::MAX); ++mType) {
        if (!InExecutionMiss_Rov_Buf[mType].empty()) {
            InExecutionMiss_Rov_Buf[mType].pop_back(); InExecutionMiss_Rov_Buf[mType].pop_back();
            std::cout << EnclosingChar[mType][0] << InExecutionMiss_Rov_Buf[mType] << EnclosingChar[mType][1] << ' ';
        }
    }

    std::cout << '\n' << LineBreak << '\n';

    std::cout << " Available Rovers: ";
    for (int rType = 0; rType < static_cast<int>(RoverType::MAX); ++rType) {
        if (!AvailableRovers_Buf[rType].empty()) {
            AvailableRovers_Buf[rType].pop_back(); AvailableRovers_Buf[rType].pop_back();
            std::cout << EnclosingChar[rType][0] << AvailableRovers_Buf[rType] << EnclosingChar[rType][1] << ' ';
        }
    }

    std::cout << '\n' << LineBreak << '\n';

    std::cout << " In-Checkup Rovers: ";
    for (int rType = 0; rType < static_cast<int>(RoverType::MAX); ++rType) {
        if (!InCheckupRovers_Buf[rType].empty()) {
            InCheckupRovers_Buf[rType].pop_back(); InCheckupRovers_Buf[rType].pop_back();
            std::cout << EnclosingChar[rType][0] << InCheckupRovers_Buf[rType] << EnclosingChar[rType][1] << ' ';
        }
    }

    std::cout << '\n' << LineBreak << '\n';

    std::cout << " Completed Missions: ";
    for (int mType = 0; mType < static_cast<int>(MissionType::MAX); ++mType) {
        if (!CompletedMission_Buf[mType].empty()) {
            CompletedMission_Buf[mType].pop_back(); CompletedMission_Buf[mType].pop_back();
            std::cout << EnclosingChar[mType][0] << CompletedMission_Buf[mType] << EnclosingChar[mType][1] << ' ';
        }
    }
}

inline void UI::ClearBuffers()
{
    for (int mType = 0; mType < static_cast<int>(MissionType::MAX); ++mType) {
        WaitingMission_Buf[mType].clear();
        InExecutionMiss_Rov_Buf[mType].clear();
        CompletedMission_Buf[mType].clear();
    }

    for (int rType = 0; rType < static_cast<int>(RoverType::MAX); ++rType) {
        AvailableRovers_Buf[rType].clear();
        InCheckupRovers_Buf[rType].clear();
    }
}

inline void UI::Wait()
{
    //Default behavior is to wait for input
    //Will also work when user presses the Enter button
    std::cin.get();
}

UI::UI(OutputType OutputT) :
    OType(OutputT)
{
    IFile.open("input.txt");
    OFile.open("output.txt");
}

UI::UI(OutputType OutputT, std::string IFileName, std::string OFileName) :
    OType(OutputT)
{
    IFile.open(IFileName + ".txt");
    OFile.open(OFileName + ".txt");
}

void UI::RealAll(MarsStation* Station)
{
    ReadRoverData(Station);
    ReadCheckupInfo(Station);
    ReadAutoPromotion(Station);
    ReadEvents(Station);
}

void UI::Print(MarsStation* Station)
{
    Mission* MissionPtr; 

    //In all cases UI should output completed missions to OFile
    Stack<Mission*> Completed_Missions = Station->GetCompletedMissions();
    while (Completed_Missions.pop(MissionPtr)) {
        OFile << MissionPtr->GetCD() << '\t' << MissionPtr->GetID() << '\t'
            << MissionPtr->GetFormulationDay() << '\t' << MissionPtr->GetWaitingDays() << '\t'
            << MissionPtr->GetED() << '\n';

        //Save completed missions in buffer if not silent
        if (OType != OutputType::Silent)
            CompletedMission_Buf[static_cast<size_t>(MissionPtr->GetMissionType())] += std::to_string(MissionPtr->GetID()) + ", ";
    }

    if (OType != OutputType::Silent) {
        //Add all information to buffers
        FillBuffersFromStation(Station);

        //Print CurrentDay
        std::cout << "Current Day: " << Station->GetCurrentDay() << '\n';

        //Print and format all buffer information
        FormatBuffersToConsole();

        //Clear buffers for next day
        ClearBuffers();

        //Wait for user input before continuing program
        if (OType == OutputType::StepByStep)
            Wait();
    }
}

void UI::PrintStatistics(MarsStation* Station)
{}

UI::~UI()
{
    IFile.close();
    OFile.close();
}
