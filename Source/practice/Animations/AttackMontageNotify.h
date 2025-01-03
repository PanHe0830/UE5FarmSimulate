// 版权归快下雨了L所有

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "AttackMontageNotify.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UAttackMontageNotify : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()
public:
	virtual void BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	virtual void BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload) override;

private:
	class APracticeCharacter* Character = nullptr;
};
