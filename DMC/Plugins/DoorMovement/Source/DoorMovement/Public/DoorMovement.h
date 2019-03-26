// Copyright 2019 Inu Games

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FDoorMovementModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
