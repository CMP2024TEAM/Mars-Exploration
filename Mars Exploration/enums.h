#pragma once

enum rovertype
{
	Emergency,
	Mountainous,
	Polar
};

enum class roverstatus
{
	Available,
	InCheckUp,
	InMaintenance
};

enum class MissionType
{
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

enum EventType
{
	FORMULATION,
	CANCELLATION,
	PROMOTION,
};