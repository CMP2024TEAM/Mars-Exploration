#pragma once

namespace rovertype {
	enum rovertype {
		Emergency,
		Mountainous,
		Polar,
		RoverCount
	};
}

enum class roverstatus
{
	Available,
	InCheckUp,
	InMaintenance
};

namespace MissionType {
	enum MissionType {
		Emergency,
		Mountainous,
		Polar
	};
}

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