// Fill out your copyright notice in the Description page of Project Settings.

#include "PDTableManagerSubsystem.h"
#include "Engine/DataTable.h"
#include "Misc/PackageName.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "PDGameInstance.h"

namespace
{
	static int32 GetRowMapKey(const FPDUnitRow& Row)
	{
		return Row.ID;
	}

	static int32 GetRowMapKey(const FPDUnitStatRow& Row)
	{
		return Row.ID;
	}

	static int32 GetRowMapKey(const FPDUnitLevelRow& Row)
	{
		return Row.Level;
	}

	static int32 GetRowMapKey(const FPDStageRow& Row)
	{
		return Row.ID;
	}

	static int32 GetRowMapKey(const FPDBattleItemRow& Row)
	{
		return Row.ID;
	}

	template <typename RowType>
	void BuildRowMapById(
		UDataTable* InDataTable,
		TMap<int32, const RowType*>& OutMap,
		const TCHAR* InTableLabel,
		const TCHAR* InContext)
	{
		OutMap.Empty();

		if (!InDataTable)
		{
			UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] %sDataTable is null!"), InTableLabel);
			return;
		}

		int32 LoadedCount = 0;
		TArray<RowType*> AllRows;
		InDataTable->GetAllRows(InContext, AllRows);

		for (const RowType* Row : AllRows)
		{
			if (!Row)
			{
				continue;
			}

			const int32 RowID = GetRowMapKey(*Row);
			if (RowID <= 0)
			{
				continue;
			}
			if (OutMap.Contains(RowID))
			{
				continue;
			}

			OutMap.Add(RowID, Row);
			++LoadedCount;
		}

		UE_LOG(LogTemp, Log, TEXT("[PD][TableManager] %sMap built. Total entries: %d"), InTableLabel, LoadedCount);
	}

	// "DA_Unit_001" 처럼 "에셋 이름"만 허용
	static FString MakeUnitDataAssetObjectPathFromName(FString In)
	{
		In.TrimStartAndEndInline();
		if (In.IsEmpty())
		{
			return FString();
		}
		
		const FString AssetName = FPackageName::GetShortName(In);
		if (AssetName.IsEmpty())
		{
			return FString();
		}

		return FString::Printf(TEXT("/Game/DataAsset/Unit/%s.%s"), *AssetName, *AssetName);
	}

	static FString MakeStageDataAssetObjectPathFromName(FString In)
	{
		In.TrimStartAndEndInline();
		if (In.IsEmpty())
		{
			return FString();
		}

		const FString AssetName = FPackageName::GetShortName(In);
		if (AssetName.IsEmpty())
		{
			return FString();
		}

		return FString::Printf(TEXT("/Game/DataAsset/Stage/%s.%s"), *AssetName, *AssetName);
	}
}

void UPDTableManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UnitDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Table/DataTable/DT_Unit"));
	UnitStatDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Table/DataTable/DT_UnitStat"));
	UnitLevelDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Table/DataTable/DT_UnitLevel"));
	StageDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Table/DataTable/DT_Stage"));
	BattleItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Table/DataTable/DT_BattleItem"));
	UIBaseDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Table/DataTable/DT_UIBase"));
	ImageDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Table/DataTable/DT_Image"));

	// DataTable을 맵으로 변환 (RowName 의존 제거)
	BuildUnitMap();
	BuildUnitStatMap();
	BuildUnitLevelMap();
	BuildStageMap();
	BuildBattleItemMap();
	BuildUIBaseMap();
	BuildImageMap();
}

void UPDTableManagerSubsystem::Deinitialize()
{
	UnitDataAssetCache.Empty();
	StageDataAssetCache.Empty();
	ImageTextureCache.Empty();
	UnitMap.Empty();
	UnitStatMap.Empty();
	UnitLevelMap.Empty();
	StageMap.Empty();
	BattleItemMap.Empty();
	UIBaseMap.Empty();
	ImageMap.Empty();

	UnitDataTable = nullptr;
	UnitStatDataTable = nullptr;
	UnitLevelDataTable = nullptr;
	StageDataTable = nullptr;
	BattleItemDataTable = nullptr;
	UIBaseDataTable = nullptr;
	ImageDataTable = nullptr;
	Super::Deinitialize();
}

void UPDTableManagerSubsystem::BuildUnitMap()
{
	BuildRowMapById<FPDUnitRow>(
		UnitDataTable,
		UnitMap,
		TEXT("Unit"),
		TEXT("UPDTableManagerSubsystem::BuildUnitMap"));
}

void UPDTableManagerSubsystem::BuildUnitStatMap()
{
	BuildRowMapById<FPDUnitStatRow>(
		UnitStatDataTable,
		UnitStatMap,
		TEXT("UnitStat"),
		TEXT("UPDTableManagerSubsystem::BuildUnitStatMap"));
}

void UPDTableManagerSubsystem::BuildUnitLevelMap()
{
	BuildRowMapById<FPDUnitLevelRow>(
		UnitLevelDataTable,
		UnitLevelMap,
		TEXT("UnitLevel"),
		TEXT("UPDTableManagerSubsystem::BuildUnitLevelMap"));
}

void UPDTableManagerSubsystem::BuildStageMap()
{
	BuildRowMapById<FPDStageRow>(
		StageDataTable,
		StageMap,
		TEXT("Stage"),
		TEXT("UPDTableManagerSubsystem::BuildStageMap"));
}

void UPDTableManagerSubsystem::BuildBattleItemMap()
{
	BuildRowMapById<FPDBattleItemRow>(
		BattleItemDataTable,
		BattleItemMap,
		TEXT("BattleItem"),
		TEXT("UPDTableManagerSubsystem::BuildBattleItemMap"));
}

void UPDTableManagerSubsystem::BuildUIBaseMap()
{
	UIBaseMap.Empty();

	if (!UIBaseDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] UIBaseDataTable is null!"));
		return;
	}

	TArray<FPDUIBase*> AllRows;
	UIBaseDataTable->GetAllRows(TEXT("UPDTableManagerSubsystem::BuildUIBaseMap"), AllRows);

	for (const FPDUIBase* Row : AllRows)
	{
		if (!Row || Row->UIType == EUIType::None)
		{
			continue;
		}
		if (UIBaseMap.Contains(Row->UIType))
		{
			continue;
		}
		UIBaseMap.Add(Row->UIType, Row);
	}

	UE_LOG(LogTemp, Log, TEXT("[PD][TableManager] UIBaseMap built. Total entries: %d"), UIBaseMap.Num());
}

const FPDUnitRow* UPDTableManagerSubsystem::GetUnit(int32 UnitID) const
{
	if (UnitID <= 0)
	{
		return nullptr;
	}
	const FPDUnitRow* const* FoundRow = UnitMap.Find(UnitID);
	return FoundRow ? *FoundRow : nullptr;
}

const FPDUnitStatRow* UPDTableManagerSubsystem::GetUnitStat(int32 StatID) const
{
	if (StatID <= 0)
	{
		return nullptr;
	}
	const FPDUnitStatRow* const* FoundRow = UnitStatMap.Find(StatID);
	return FoundRow ? *FoundRow : nullptr;
}

const FPDUnitLevelRow* UPDTableManagerSubsystem::GetUnitLevel(int32 Level) const
{
	if (Level <= 0)
	{
		return nullptr;
	}
	const FPDUnitLevelRow* const* FoundRow = UnitLevelMap.Find(Level);
	return FoundRow ? *FoundRow : nullptr;
}

const FPDStageRow* UPDTableManagerSubsystem::GetStage(int32 StageID) const
{
	if (StageID <= 0)
	{
		return nullptr;
	}
	const FPDStageRow* const* FoundRow = StageMap.Find(StageID);
	return FoundRow ? *FoundRow : nullptr;
}

const FPDBattleItemRow* UPDTableManagerSubsystem::GetBattleItem(int32 ItemID) const
{
	if (ItemID <= 0)
	{
		return nullptr;
	}
	const FPDBattleItemRow* const* FoundRow = BattleItemMap.Find(ItemID);
	return FoundRow ? *FoundRow : nullptr;
}

const FPDUIBase* UPDTableManagerSubsystem::GetUIBase(EUIType UIType) const
{
	if (UIType == EUIType::None)
	{
		return nullptr;
	}
	const FPDUIBase* const* Found = UIBaseMap.Find(UIType);
	return Found ? *Found : nullptr;
}

UClass* UPDTableManagerSubsystem::GetWidgetClass(EUIType InUIType) const
{
	const FPDUIBase* Row = GetUIBase(InUIType);
	if (!Row || Row->WidgetClassPath.IsEmpty())
	{
		return nullptr;
	}
	return StaticLoadClass(UUserWidget::StaticClass(), nullptr, *Row->WidgetClassPath);
}

UClass* UPDTableManagerSubsystem::GetUnitBP(int32 InUnitID)
{
	const FPDUnitRow* UnitData = GetUnit(InUnitID);
	if (!UnitData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid UnitData %d"), InUnitID);
		return nullptr;
	}

	UPDUnitDataAsset* UnitAsset = GetUnitDataAssetByName(UnitData->DataAssetName);
	if (!UnitAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][Test] Failed to load UnitDataAsset: %s"), *UnitData->DataAssetName);
		return nullptr;
	}

	const FString UnitBPPath = UnitAsset->UnitBP.ToSoftObjectPath().ToString();
	UClass* ObjectClass = StaticLoadClass(UObject::StaticClass(), NULL, *UnitBPPath, NULL, LOAD_None, NULL);
	return ObjectClass;
}

void UPDTableManagerSubsystem::BuildImageMap()
{
	ImageMap.Empty();

	if (!ImageDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] ImageDataTable is null!"));
		return;
	}

	TArray<FPDImageRow*> AllRows;
	ImageDataTable->GetAllRows(TEXT("UPDTableManagerSubsystem::BuildImageMap"), AllRows);

	for (const FPDImageRow* Row : AllRows)
	{
		if (!Row || Row->ImageName.IsNone())
		{
			continue;
		}
		if (ImageMap.Contains(Row->ImageName))
		{
			continue;
		}
		ImageMap.Add(Row->ImageName, Row);
	}

	UE_LOG(LogTemp, Log, TEXT("[PD][TableManager] ImageMap built. Total entries: %d"), ImageMap.Num());
}

UTexture2D* UPDTableManagerSubsystem::GetImage(FName ImageName)
{
	if (ImageName.IsNone())
	{
		return nullptr;
	}

	if (TObjectPtr<UTexture2D>* Cached = ImageTextureCache.Find(ImageName))
	{
		if (IsValid(*Cached))
		{
			return Cached->Get();
		}
		ImageTextureCache.Remove(ImageName);
	}

	const FPDImageRow* const* FoundRow = ImageMap.Find(ImageName);
	if (!FoundRow || !*FoundRow || (*FoundRow)->FileName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] GetImage: ImageName '%s' not found in ImageMap"), *ImageName.ToString());
		return nullptr;
	}

	const FName FileName = (*FoundRow)->FileName;

	FString BasePath;
	if (const UPDGameInstance* GI = Cast<UPDGameInstance>(GetGameInstance()))
	{
		BasePath = GI->ImageBasePath;
	}

	const FString TexturePath = FString::Printf(TEXT("%s/%s.%s"), *BasePath, *FileName.ToString(), *FileName.ToString());
	UTexture2D* Loaded = LoadObject<UTexture2D>(nullptr, *TexturePath);
	if (!Loaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] GetImage: Failed to load texture. ImageName: '%s' Path: '%s'"), *ImageName.ToString(), *TexturePath);
		return nullptr;
	}

	ImageTextureCache.Add(ImageName, Loaded);
	return Loaded;
}

UPDUnitDataAsset* UPDTableManagerSubsystem::GetUnitDataAssetByName(const FString& AssetName, bool bForceReload /*=false*/)
{
	const FString ObjectPathStr = MakeUnitDataAssetObjectPathFromName(AssetName);
	if (ObjectPathStr.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] GetUnitDataAssetByName only accepts asset name. Input: %s"), *AssetName);
		return nullptr;
	}

	// 캐시 키는 "에셋 이름" 기준으로 통일 (입력 공백/케이스 차이 방지)
	const FString AssetNameOnly = FPackageName::GetShortName(ObjectPathStr);
	const FName CacheKey(*AssetNameOnly);

	if (!bForceReload)
	{
		if (TObjectPtr<UPDUnitDataAsset>* Found = UnitDataAssetCache.Find(CacheKey))
		{
			if (IsValid(*Found))
			{
				return Found->Get();
			}

			// 캐시에 있는데 무효면 제거 후 재로드
			UnitDataAssetCache.Remove(CacheKey);
		}
	}
	else
	{
		UnitDataAssetCache.Remove(CacheKey);
	}

	UPDUnitDataAsset* Loaded = LoadObject<UPDUnitDataAsset>(nullptr, *ObjectPathStr);
	if (!Loaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] Failed to load UnitDataAsset. Name: %s Path: %s"), *AssetNameOnly, *ObjectPathStr);
		return nullptr;
	}

	UnitDataAssetCache.Add(CacheKey, Loaded);
	return Loaded;
}

UPDStageDataAsset* UPDTableManagerSubsystem::GetStageDataAssetByName(const FString& AssetName, bool bForceReload)
{
	const FString ObjectPathStr = MakeStageDataAssetObjectPathFromName(AssetName);
	if (ObjectPathStr.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] GetStageDataAssetByName only accepts asset name. Input: %s"), *AssetName);
		return nullptr;
	}

	// 캐시 키는 "에셋 이름" 기준으로 통일 (입력 공백/케이스 차이 방지)
	const FString AssetNameOnly = FPackageName::GetShortName(ObjectPathStr);
	const FName CacheKey(*AssetNameOnly);

	if (!bForceReload)
	{
		if (TObjectPtr<UPDStageDataAsset>* Found = StageDataAssetCache.Find(CacheKey))
		{
			if (IsValid(*Found))
			{
				return Found->Get();
			}

			// 캐시에 있는데 무효면 제거 후 재로드
			StageDataAssetCache.Remove(CacheKey);
		}
	}
	else
	{
		StageDataAssetCache.Remove(CacheKey);
	}

	UPDStageDataAsset* Loaded = LoadObject<UPDStageDataAsset>(nullptr, *ObjectPathStr);
	if (!Loaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PD][TableManager] Failed to load StageDataAsset. Name: %s Path: %s"), *AssetNameOnly, *ObjectPathStr);
		return nullptr;
	}

	StageDataAssetCache.Add(CacheKey, Loaded);
	return Loaded;
}
