


#include "MainAssetManager.h"
#include "MainGameplayTags.h"

UMainAssetManager& UMainAssetManager::Get()
{
	check(GEngine);
	
	UMainAssetManager* MainAssetManager = Cast<UMainAssetManager>(GEngine->AssetManager);
	return *MainAssetManager;
}

void UMainAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FMainGameplayTags::InitializeNativeGameplayTags();
}
