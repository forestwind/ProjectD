#pragma once

#include "CoreMinimal.h"
#include "PDDefine.generated.h"

#define CPROFILE_PDPAWN TEXT("PDPawn")
#define CPROFILE_PDTRIGGER TEXT("PDTrigger")
#define CCHANNEL_PDACTION ECC_GameTraceChannel1

UENUM(BlueprintType)
enum class EUnitKind : uint8
{
	Player	UMETA(DisplayName = "Player"),
	Monster	UMETA(DisplayName = "Monster")
};