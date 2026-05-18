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

UENUM(BlueprintType)
enum class EUILayer : uint8
{
	World	UMETA(DisplayName = "World"),	// 월드 공간 (캐릭터 HP바 등 3D 오브젝트 위 위젯)
	Screen	UMETA(DisplayName = "Screen"),	// 전체화면 (타이틀, 배틀HUD)
	Window	UMETA(DisplayName = "Window"),	// 부분화면 (인벤토리, 상점)
	Popup	UMETA(DisplayName = "Popup"),	// 소형창 (확인/취소, 알림)
	System	UMETA(DisplayName = "System"),	// 시스템 최상단 (로딩, 에러)
};

UENUM(BlueprintType)
enum class EUIType : uint8
{
	None			UMETA(DisplayName = "None"),

	// Title
	TitleMain		UMETA(DisplayName = "TitleMain"),

	// Battle - Screen
	BattleMain		UMETA(DisplayName = "BattleMain"),

	// Battle - Window
	BattleInventory	UMETA(DisplayName = "BattleInventory"),
	CharacterInfo	UMETA(DisplayName = "CharacterInfo"),

	// Battle - Popup
	BattleEnd		UMETA(DisplayName = "BattleEnd"),
	BattlePhaseMsg	UMETA(DisplayName = "BattlePhaseMsg"),

	// Battle - World
	BattleHpBar		UMETA(DisplayName = "BattleHpBar"),
	BattleDamageNum	UMETA(DisplayName = "BattleDamageNum"),
};