#include <chrono>
#include <thread>
#include "UI.h"
#include "MarsStation.h"
#include "DataStructures/Stack.h"

/*=================================================
                    Constants
=================================================*/
constexpr int BreakLine_Length = 50;
constexpr int StepByStep_WaitingTime = 1;

const char UI::EnclosingChar[mTypeMax][2] = {
   //Emergency
   {'[',']'},
   //Mountainous
   {'{','}'},
   //Polar
   {'(',')'}
};

const std::string UI::LineBreak(BreakLine_Length, '-');
const std::string UI::WarningBreak(BreakLine_Length, '=');

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

inline void UI::ReadHealthInfo(MarsStation* Station)
{
    //Get Maximum Health of each rover type

    int MaximumRoverHealth;
    for (int rType = 0; rType < rTypeMax; ++rType) {
        IFile >> MaximumRoverHealth;
        Station->SetRoverMaxHealth(RoverType(rType), MaximumRoverHealth);
    }
}

inline void UI::ReadRoverData(MarsStation* Station)
{
    for (int rType = 0; rType < rTypeMax; ++rType) {
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
    //Get number of completed missions before rover goes into checkup

    int CheckupDays;
    IFile >> CheckupDays;
    Station->SetMissionsBeforeCheckup(CheckupDays);

    //Get number of days each rover types needs in checkup 

    for (int rType = 0; rType < rTypeMax; ++rType) {
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
    // Get counts from mars station

    WaitingCount = Station->GetWaitingMissionsCount();
    InExecutionCount = Station->GetInExecitionMissionsCount();
    AvailableCount = Station->GetAvailableRoversCount();
    CheckUpCount = Station->GetInCheckupRoversCount();
    MaintenanceCount = Station->GetInMaintenanceRoversCount();
    CompletedCount = Station->GetCompletedMissionsCount();

    //Temp pointers for dealing with Data Structures

    Mission* MissionPtr;
    Rover* RoverPtr;

    //Extract missions from Data Structures containing only one mission type

    int mType;
    for (mType = 0; mType < mTypeMax; ++mType) {
        Queue<Mission*> Waiting_Missions = Station->GetWaitingMissions(static_cast<MissionType>(mType));
        while (Waiting_Missions.dequeue(MissionPtr))
            WaitingMission_Buf[mType] += std::to_string(MissionPtr->GetID()) + ", ";
    }

    //Extract missions from Data Structures containing more than one mission type

    PriorityQueue<Mission*> InExec_Missions = Station->GetInExecutionMissions();
    while (InExec_Missions.dequeue(MissionPtr)) {
        mType = static_cast<int>(MissionPtr->GetMissionType());
        InExecutionMiss_Rov_Buf[mType] += std::to_string(MissionPtr->GetID()) + '/' + std::to_string(MissionPtr->GetRover()->GetID()) + ", ";
    }

    //Extract rovers from Data Structures containing only one rover type

    for (int rType = 0; rType < rTypeMax; ++rType) {
        PriorityQueue<Rover*> Available_Rovers = Station->GetAvailableRovers(static_cast<RoverType>(rType));            
        while (Available_Rovers.dequeue(RoverPtr))
            AvailableRovers_Buf[rType] += std::to_string(RoverPtr->GetID()) + ", ";

        Queue<Rover*> InCheckup_Rovers = Station->GetInCheckupRovers(static_cast<RoverType>(rType));
        while (InCheckup_Rovers.dequeue(RoverPtr))
            InCheckupRovers_Buf[rType] += std::to_string(RoverPtr->GetID()) + ", ";

        Queue<Rover*> InMaintenance_Rovers = Station->GetInMaintenanceRovers(static_cast<RoverType>(rType));
        while (InMaintenance_Rovers.dequeue(RoverPtr))
            InMaintenanceRovers_Buf[rType] += std::to_string(RoverPtr->GetID()) + ", ";
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
        for (int mType = 0; mType < mTypeMax; ++mType) {
            if (!FailedMission_Buf[mType].empty()) {
                FailedMission_Buf[mType].pop_back(); FailedMission_Buf[mType].pop_back();
                std::cout << EnclosingChar[mType][0] << FailedMission_Buf[mType] << EnclosingChar[mType][1] << ' ';
            }
        }

        std::cout << '\n' << WarningBreak << '\n';
    }
    
    std::cout << WaitingCount << " Waiting Misions: ";
    //Loop over all waiting missions buffers
    for (int mType = 0; mType < mTypeMax; ++mType) {
        //Print only non emtpy buffers
        if (!WaitingMission_Buf[mType].empty()) {
            //Since last two characters are ", " they need to be removed before output
            WaitingMission_Buf[mType].pop_back(); WaitingMission_Buf[mType].pop_back();
            //Output each cleaned buffer alongside its respective set of enclosing brackets
            std::cout << EnclosingChar[mType][0] << WaitingMission_Buf[mType] << EnclosingChar[mType][1] << ' ';
        }
    }

    std::cout << '\n' << LineBreak << '\n';

    std::cout << InExecutionCount << " In-Execution Missions/Rovers: ";
    for (int mType = 0; mType < mTypeMax; ++mType) {
        if (!InExecutionMiss_Rov_Buf[mType].empty()) {
            InExecutionMiss_Rov_Buf[mType].pop_back(); InExecutionMiss_Rov_Buf[mType].pop_back();
            std::cout << EnclosingChar[mType][0] << InExecutionMiss_Rov_Buf[mType] << EnclosingChar[mType][1] << ' ';
        }
    }

    std::cout << '\n' << LineBreak << '\n';

    std::cout << AvailableCount << " Available Rovers: ";
    for (int rType = 0; rType < rTypeMax; ++rType) {
        if (!AvailableRovers_Buf[rType].empty()) {
            AvailableRovers_Buf[rType].pop_back(); AvailableRovers_Buf[rType].pop_back();
            std::cout << EnclosingChar[rType][0] << AvailableRovers_Buf[rType] << EnclosingChar[rType][1] << ' ';
        }
    }

    std::cout << '\n' << LineBreak << '\n';

    std::cout << CheckUpCount << " In-Checkup Rovers: ";
    for (int rType = 0; rType < rTypeMax; ++rType) {
        if (!InCheckupRovers_Buf[rType].empty()) {
            InCheckupRovers_Buf[rType].pop_back(); InCheckupRovers_Buf[rType].pop_back();
            std::cout << EnclosingChar[rType][0] << InCheckupRovers_Buf[rType] << EnclosingChar[rType][1] << ' ';
        }
    }

    std::cout << '\n' << LineBreak << '\n';

    std::cout << MaintenanceCount << " In-Maintenance Rovers: ";
    for (int rType = 0; rType < rTypeMax; ++rType) {
        if (!InMaintenanceRovers_Buf[rType].empty()) {
            InMaintenanceRovers_Buf[rType].pop_back(); InMaintenanceRovers_Buf[rType].pop_back();
            std::cout << EnclosingChar[rType][0] << InMaintenanceRovers_Buf[rType] << EnclosingChar[rType][1] << ' ';
        }
    }
    
    std::cout << '\n' << LineBreak << '\n';

    std::cout << CompletedCount << " Completed Missions: ";
    for (int mType = 0; mType < mTypeMax; ++mType) {
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
    //Clear missions
    for (int mType = 0; mType < mTypeMax; ++mType) {
        FailedMission_Buf[mType].clear();
        WaitingMission_Buf[mType].clear();
        InExecutionMiss_Rov_Buf[mType].clear();
        //Completed missions are not cleared from buffer
    }

    //Clear Rovers
    for (int rType = 0; rType < rTypeMax; ++rType) {
        AvailableRovers_Buf[rType].clear();
        InCheckupRovers_Buf[rType].clear();
        InMaintenanceRovers_Buf[rType].clear();
    }

    //Reset Failed Mission Count
    FailedCount = 0;
}

inline void UI::WaitForUserInput()
{
    //Default behavior is to wait for any input
    //Will also work when user presses the Enter button
    std::cin.get();
}

inline void UI::PrintStatistics(MarsStation* Station)
{
    OFile << '\n' << LineBreak << "\n\n";
    OFile << "Missions: " << Station->GetCompletedMissionsCount() << " [M: " << Station->GetMountainouMissionCount()
        << ", P: " << Station->GetPolarMissionCount() << ", E: " << Station->GetEmergencyMissionCount() << "]"<< '\n';
    
    int R_MCount = Station->GetMountainouRoverCount();
    int R_PCount = Station->GetPolarRoverCount();
    int R_ECount = Station->GetEmergencyRoverCount();
    
    OFile << "Rovers: " << R_MCount + R_PCount + R_ECount << " [M: " << R_MCount << ", P: " << R_PCount << ", E: " << R_ECount << "]" << '\n';
    
    OFile << "Avg Wait: " << Station->GetAvgWait() << ", Avg Exec: " << Station->GetAvgExec() << '\n';
    
    OFile << "Auto-Promoted: " << Station->GetAutoPPercent() << "%\n";

    OFile << "Mission Failures: " << TotalFailures; 
}


/*=================================================
                 Public Functions
=================================================*/

UI::UI(std::string IFileName, std::string OFileName) : 
    TotalFailures(0),
    FailedCount(0),
    WaitingCount(0),
    InExecutionCount(0),
    AvailableCount(0),
    CheckUpCount(0),
    MaintenanceCount(0),
    CompletedCount(0)
{
    //Create/Open files from OS
    IFile.open(IFileName + ".txt");
    OFile.open(OFileName + ".txt");

    std::cout << "Enter which output mode should UI run in:\n0 for Interactive  |  1 for StepByStep  |  2 for silent\n";

    int x;
    std::cin >> x;
    switch (x) {
    case 0:OType = OutputType::Interactive; break;
    case 1:OType = OutputType::StepByStep; break;
    case 2:OType = OutputType::Silent; break;
    default:OType = OutputType::Silent; break;
    }

    //Remove last newline(if exists) from input stream
    std::cin.ignore();
}

UI::UI(OutputType OutputT, std::string IFileName, std::string OFileName) :
    OType(OutputT),
    TotalFailures(0),
    FailedCount(0),
    WaitingCount(0),
    InExecutionCount(0),
    AvailableCount(0),
    CheckUpCount(0),
    MaintenanceCount(0),
    CompletedCount(0)  
{
    //Create/Open files from OS
    IFile.open(IFileName + ".txt");
    OFile.open(OFileName + ".txt");

    //Remove last newline(if exists) from input stream
    std::cin.ignore();
}

void UI::ReadAll(MarsStation* Station)
{
    ReadHealthInfo(Station);
    ReadRoverData(Station);
    ReadCheckupInfo(Station);   
    ReadAutoPromotion(Station);
    ReadEvents(Station);
}

void UI::InitialDisplayMessage()
{
    //In all cases UI should output completed missions to OFile
    OFile << "CD\tID\tFD\tWD\tED\n";

    switch (OType) {
    //Initial message for Interactive mode
    case OutputType::Interactive: {
        std::cout << "Interactive Mode \nSimulation Starts...\nPress any key to move to next day\n";
        break;
    }
    //Initial message for StepByStep mode
    case OutputType::StepByStep: {
        std::cout << "StepByStep Mode\nSimulation Starts...\nWait " << StepByStep_WaitingTime << " second(s) before each day\n";
        break;
    }
    //Initial message for Silent mode
    case OutputType::Silent: {
        std::cout << "Silent Mode\nSimulation Starts...\n";
        break;
    }
    }
}

void UI::LogMissionFailure(Mission* const FailedMission)
{
    //Add mission to buffer for console output in non silent modes
    if (OType == OutputType::Interactive || OType == OutputType::StepByStep) {
        FailedMission_Buf[static_cast<int>(FailedMission->GetMissionType())] += std::to_string(FailedMission->GetID()) + ", ";
        ++FailedCount;
    }

    //In all cases total failures is printed to output file at the end of simulation
    ++TotalFailures;
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
            CompletedMission_Buf[static_cast<int>(MissionPtr->GetMissionType())] += std::to_string(MissionPtr->GetID()) + ", ";
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

        //Wait "StepByStep_WaitingTime" seconds before continuing program
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
    //Return input and output files back to OS

    IFile.close();
    OFile.close();
}
