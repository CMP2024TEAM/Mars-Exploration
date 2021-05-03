#pragma once

enum class RoverType {
	Emergency,
	Mountainous,
	Polar,
	RoverType_MAX
};

enum class RoverStatus
{
	Available,
	InCheckUp,
	InMaintenance
};

enum class MissionType {
	Emergency,
	Mountainous,
	Polar
};

enum class MissionStatus
{
	Waiting,
	InExecution,
	Completed
};

enum class EventType
{
	Formulation,
	Cancelation,
	Promotion,
};