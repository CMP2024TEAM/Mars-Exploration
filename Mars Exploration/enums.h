#pragma once
/*============================================================================
					   DO NOT CHANGE THE ORDER OF ENUMS
	You may only add extra items while keeping MAX as the last item in Enum
============================================================================*/

enum class RoverType
{
    Emergency,
    Mountainous,
    Polar,
    MAX
};

enum class RoverStatus
{
	Available,
	InCheckUp,
	InMaintenance,
	MAX
};

enum class MissionType 
{
	Emergency,
	Mountainous,
	Polar,
	MAX
};

enum class MissionStatus
{
	Waiting,
	InExecution,
	Completed,
	MAX
};

enum class EventType
{
	Formulation,
	Cancelation,
	Promotion,
	MAX
};

enum class OutputType 
{
	Interactive,
	StepByStep,
	Silent,
	MAX
};