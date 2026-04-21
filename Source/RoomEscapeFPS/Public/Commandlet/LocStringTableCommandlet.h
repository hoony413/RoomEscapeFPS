#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "LocStringTableCommandlet.generated.h"

UCLASS()
class ULocStringTableCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	int32 Main(FString const& params) override;
};
