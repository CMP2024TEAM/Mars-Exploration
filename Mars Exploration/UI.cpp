#include <chrono>
#include <thread>
#include "UI.h"
#include "MarsStation.h"
#include "DataStructures/Stack.h"

/*=================================================
                    Constants
=================================================*/
constexpr auto BreakLine_Length = 50;
constexpr auto StepByStep_WaitingTime = 1;

const char UI::EnclosingChar[static_cast<size_t>(MissionType::MAX)][2] = {
   //Emergency
   {'[',']'},
   //Mountainous
   {'{','}'},
   //Polar
   {'(',')'}
};

const std::string UI::WarningBreak(BreakLine_Length, '=');
const std::string UI::LineBreak(BreakLine_Length, '-');


/*=================================================
                   Input Utility
=================================================*/

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
    Station->SetMissionsBeforeCheckup(CheckupDays);
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

inline void UI::FillBuffersFromStation(MarsStation* Station)
{
    // Getters for counts from mars station
    WaitingCount = Station->GetWaitingMissionsCount();
    InExecutionCount = Station->GetInExecitionMissionsCount();
    AvailableCount = Station->GetAvailableRoversCount();
    CheckUpCount = Station->GetInCheckupRoversCount();
    CompletedCount = Station->GetCompletedMissionsCount();
    Mission* MissionPtr;
    Rover* RoverPtr;

    int mType;
    for (mType = 0; mType < static_cast<int>(MissionType::MAX); ++mType) {
        Queue<Mission*> Waiting_Missions = Station->GetWaitingMissions(static_cast<MissionType>(mType));
        while (Waiting_Missions.dequeue(MissionPtr))
            WaitingMission_Buf[mType] += std::to_string(MissionPtr->GetID()) + ", ";
    }

    PriorityQueue<Mission*> InExec_Missions = Station->GetInExecutionMissions();
    while (InExec_Missions.dequeue(MissionPtr)) {
        mType = static_cast<int>(MissionPtr->GetMissionType());
        InExecutionMiss_Rov_Buf[mType] += std::to_string(MissionPtr->GetID()) + '/' + std::to_string(MissionPtr->GetRover()->GetID()) + ", ";
    }


    for (int rType = 0; rType < static_cast<int>(RoverType::MAX); ++rType) {
        PriorityQueue<Rover*> Available_Rovers = Station->GetAvailableRovers(static_cast<RoverType>(rType));
        Queue<Rover*> InCheckup_Rovers = Station->GetInCheckupRovers(static_cast<RoverType>(rType));
        while (Available_Rovers.dequeue(RoverPtr))
            AvailableRovers_Buf[rType] += std::to_string(RoverPtr->GetID()) + ", ";
        while (InCheckup_Rovers.dequeue(RoverPtr))
            InCheckupRovers_Buf[rType] += std::to_string(RoverPtr->GetID()) + ", ";
    }
}


/*=================================================
                  Output Utility
=================================================*/

inline void UI::FormatBuffersToConsole()
{
    //Show failed missions if any failed otherwise don't print anything.
    if (FailedCount) {
        std::cout << WarningBreak << '\n';
        std::cout << "WARNING: " << FailedCount << " Failed Missions: ";
        for (int mType = 0; mType < static_cast<int>(MissionType::MAX); ++mType) {
            if (!FailedMission_Buf[mType].empty()) {
                FailedMission_Buf[mType].pop_back(); FailedMission_Buf[mType].pop_back();
                std::cout << EnclosingChar[mType][0] << FailedMission_Buf[mType] << EnclosingChar[mType][1] << ' ';
            }
        }

        std::cout << '\n' << WarningBreak << '\n';
    }
    
    std::cout << WaitingCount << " Waiting Misions: ";
    for (int mType = 0; mType < static_cast<int>(MissionType::MAX); ++mType) {
        if (!WaitingMission_Buf[mType].empty()) {
            WaitingMission_Buf[mType].pop_back(); WaitingMission_Buf[mType].pop_back();
            std::cout << EnclosingChar[mType][0] << WaitingMission_Buf[mType] << EnclosingChar[mType][1] << ' ';
        }
    }

    std::cout << '\n' << LineBreak << '\n';

    std::cout << InExecutionCount << " In-Execution Missions/Rovers: ";
    for (int mType = 0; mType < static_cast<int>(MissionType::MAX); ++mType) {
        if (!InExecutionMiss_Rov_Buf[mType].empty()) {
            InExecutionMiss_Rov_Buf[mType].pop_back(); InExecutionMiss_Rov_Buf[mType].pop_back();
            std::cout << EnclosingChar[mType][0] << InExecutionMiss_Rov_Buf[mType] << EnclosingChar[mType][1] << ' ';
        }
    }

    std::cout << '\n' << LineBreak << '\n';

    std::cout << AvailableCount << " Available Rovers: ";
    for (int rType = 0; rType < static_cast<int>(RoverType::MAX); ++rType) {
        if (!AvailableRovers_Buf[rType].empty()) {
            AvailableRovers_Buf[rType].pop_back(); AvailableRovers_Buf[rType].pop_back();
            std::cout << EnclosingChar[rType][0] << AvailableRovers_Buf[rType] << EnclosingChar[rType][1] << ' ';
        }
    }

    std::cout << '\n' << LineBreak << '\n';

    std::cout << CheckUpCount << " In-Checkup Rovers: ";
    for (int rType = 0; rType < static_cast<int>(RoverType::MAX); ++rType) {
        if (!InCheckupRovers_Buf[rType].empty()) {
            InCheckupRovers_Buf[rType].pop_back(); InCheckupRovers_Buf[rType].pop_back();
            std::cout << EnclosingChar[rType][0] << InCheckupRovers_Buf[rType] << EnclosingChar[rType][1] << ' ';
        }
    }

    std::cout << '\n' << LineBreak << '\n';

    std::cout << CompletedCount << " Completed Missions: ";
    for (int mType = 0; mType < static_cast<int>(MissionType::MAX); ++mType) {
        if (!CompletedMission_Buf[mType].empty()) {
            CompletedMission_Buf[mType].pop_back(); CompletedMission_Buf[mType].pop_back();
            std::cout << EnclosingChar[mType][0] << CompletedMission_Buf[mType] << EnclosingChar[mType][1] << ' ';
            //Keep buffer in a consistent state
            CompletedMission_Buf[mType] += ", ";
        }
    }

    std::cout << "\n\n";
}

inline void UI::ClearBuffers()
{
    for (int mType = 0; mType < static_cast<int>(MissionType::MAX); ++mType) {
        FailedMission_Buf[mType].clear();
        WaitingMission_Buf[mType].clear();
        InExecutionMiss_Rov_Buf[mType].clear();
        //Completed missions are not cleared from buffer
        //CompletedMission_Buf[mType].clear();
    }

    for (int rType = 0; rType < static_cast<int>(RoverType::MAX); ++rType) {
        AvailableRovers_Buf[rType].clear();
        InCheckupRovers_Buf[rType].clear();
    }

    FailedCount = 0;
}

inline void UI::WaitForUserInput()
{
    //Default behavior is to wait for input
    //Will also work when user presses the Enter button
    std::cin.get();
}

inline void UI::PrintStatistics(MarsStation* Station)
{
    OFile << "......................." << "\n" << "......................." << "\n";
    OFile << "Missions: " << Station->GetCompletedMissionsCount() << " [M: " << Station->GetMountainouMissionCount()
        << ", P: " << Station->GetPolarMissionCount() << ", E: " << Station->GetEmergencyMissionCount() << "]"<< '\n';
    
    int R_MCount = Station->GetMountainouRoverCount();
    int R_PCount = Station->GetPolarRoverCount();
    int R_ECount = Station->GetEmergencyRoverCount();
    
    OFile << "Rovers: " << R_MCount + R_PCount + R_ECount << " [M: " << R_MCount << ", P: " << R_PCount << ", E: " << R_ECount << "]" << '\n';
    
    OFile << "Avg Wait: " << Station->GetAvgWait() << ", Avg Exec: " << Station->GetAvgExec() << '\n';
    
    OFile << "Auto-Promoted: " << Station->GetAutoPPercent() << "%";
   
}


/*=================================================
                 Public Functions
=================================================*/

UI::UI(OutputType OutputT) :
    OType(OutputT),
    FailedCount(0),
    WaitingCount(0),
    InExecutionCount(0),
    AvailableCount(0),
    CheckUpCount(0),
    CompletedCount(0)
{
    IFile.open("input.txt");
    OFile.open("output.txt");
    std::cin.ignore();
}

UI::UI(OutputType OutputT, std::string IFileName, std::string OFileName) :
    OType(OutputT),
    FailedCount(0),
    WaitingCount(0),
    InExecutionCount(0),
    AvailableCount(0),
    CheckUpCount(0),
    CompletedCount(0)  
{
    IFile.open(IFileName + ".txt");
    OFile.open(OFileName + ".txt");
    std::cin.ignore();
}

void UI::ReadAll(MarsStation* Station)
{
    ReadRoverData(Station);
    ReadCheckupInfo(Station);
    ReadAutoPromotion(Station);
    ReadEvents(Station);
}

void UI::InitialDisplayMessage()
{
    //In all cases UI should output completed missions to OFile
    OFile << "CD\tID\tFD\tWD\tED\n";

    //Initial message for silent mode
    if (OType == OutputType::Silent) {
        std::cout << "Silent Mode\nSimulation Starts...\n";
    }

    //(OPTIONAL) add initial messages for other modes
}

void UI::LogMissionFailure(Mission* const FailedMission)
{
    FailedMission_Buf[static_cast<int>(FailedMission->GetMissionType())] += std::to_string(FailedMission->GetID()) + ", ";

    ++FailedCount;
}

void UI::PrintCurrentDay(MarsStation* Station)
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

    if (OType == OutputType::Interactive || OType == OutputType::StepByStep) {
        //Add all information to buffers
        FillBuffersFromStation(Station);

        //Print CurrentDay
        std::cout << "Current Day: " << Station->GetCurrentDay() << '\n';

        //Print and format all buffer information
        FormatBuffersToConsole();

        //Clear buffers for next day
        ClearBuffers();

        //Wait for user input before continuing program
        if (OType == OutputType::Interactive)
            WaitForUserInput();

        else if (OType == OutputType::StepByStep)
            std::this_thread::sleep_for(std::chrono::seconds(StepByStep_WaitingTime));
    }
}

void UI::FinalDisplayMessage(MarsStation* Station)
{
    PrintStatistics(Station);

    std::cout << "Simulation ends, Output file created\n";
}

UI::~UI()
{
    IFile.close();
    OFile.close();
}
