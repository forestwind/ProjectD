#pragma once

#define BBKEY_TARGET		TEXT("Target")
#define BBKEY_HOMPOS 		TEXT("HomePos")
#define BBKEY_PATROLPOS		TEXT("PatrolPos")

UENUM()
enum class EAIState : uint8
{
	Ready,
	Idle,
	Move,
	Attack,
	Damage,
	Die,
	Victory,
	Max
};
