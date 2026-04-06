#pragma once

#include "MVVMViewModelBase.h"
#include "Interface/View/ViewModelInterface.h"
#include "g_viewmodel.generated.h"

// 모든 프로젝트 ViewModel의 기반 클래스
UCLASS(Abstract, BlueprintType)
class ROOMESCAPEFPS_API UViewModelBase
	: public UMVVMViewModelBase
{
	GENERATED_BODY()
};
