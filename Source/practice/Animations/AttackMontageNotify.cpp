// 版权归快下雨了L所有


#include "AttackMontageNotify.h"
#include "practice/Character/PracticeCharacter.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Components/SkeletalMeshComponent.h"

void UAttackMontageNotify::BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload)
{
	Character = Cast<APracticeCharacter>(BranchingPointPayload.SkelMeshComponent->GetOwner());
}

void UAttackMontageNotify::BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload)
{
}
