#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "TableExportCommandlet.generated.h"

UCLASS()
class UTableExportCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	int32 Main(FString const& params) override;
};
