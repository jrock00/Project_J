# Project_J 

Project_J는 Unreal Engine(C++) 기반으로 작성된 게임 프로젝트입니다. Gameplay Tag 정의, AI(Behavior Tree) 노드, 전투 관련 컴포넌트/인터페이스, 애니메이션 몽타주 데이터 등 게임 플레이 핵심 로직을 C++로 구현한 코드가 들어 있습니다

이 README는 포트폴리오/면접용으로, 구현한 핵심 기술과 엔진/C++ 전문성을 설명

---

- ## 핵심 구현 요약
- Gameplay Tag: Source/Project_J/PJGameplayTags.h / PJGameplayTags.cpp
- Montage 데이터: Source/Project_J/Data/PJMontageActionData.h / .cpp
- 전투/무기 충돌: Source/Project_J/Components/PJWeaponCollisionComponent.cpp, Components/PJCombatComponent.h
- AI (Behavior Tree): Source/Project_J/AI/Decorator/BTDecorator_Chance.h/.cpp, AI/Task/BTTaskNode_PerformAttack.h/.cpp, AI/Service/BTService_SelectBehavior.h
- 인터페이스: Source/Project_J/Interfaces/*.cpp (PJCombatInterface, PJInteract, PJTargeting)

---

1) Gameplay Tags
- 파일: Source/Project_J/PJGameplayTags.h / PJGameplayTags.cpp
- 구현 의도: Character 상태(Attacking, Rolling 등)와 액션/공격 타입을 네이티브 GameplayTag로 선언/정의하여 애니메이션/AI/로직에서 일관된 태그 기반 분기 처리를 하도록 설계
- 사용 기술: Native Gameplay Tags (UE_DECLARE_GAMEPLAY_TAG_EXTERN / UE_DEFINE_GAMEPLAY_TAG)
```c++
// Source/Project_J/PJGameplayTags.h
namespace PJGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Attacking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Rolling);
	// ...
}
```
```c++
// Source/Project_J/PJGameplayTags.cpp
namespace PJGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Attacking, "Character.State.Attacking");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Rolling, "Character.State.Rolling");
	// ...
}
```
- GameplayTag을 네이티브로 선언하면 런타임 문자열 비교를 피하고, 에디터/블루프린트와의 연동, DataDriven 구성(예: Montage 매핑)에서 매우 유리.

---

2) Montage / 애니메이션 데이터 분리 (DataAsset)
- 파일: Source/Project_J/Data/PJMontageActionData.h/.cpp
- 구현 의도: FGameplayTag를 키로 하는 Montage 그룹을 만들어 애니메이션을 코드가 아니라 데이터로 관리. 몽타주 선택/랜덤화를 DataAsset에서 처리
```c++
// Source/Project_J/Data/PJMontageActionData.h
UAnimMontage* GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index) const;
UAnimMontage* GetRandomMontageForTag(const FGameplayTag& GroupTag) const;
```
```c++
// Source/Project_J/Data/PJMontageActionData.cpp
UAnimMontage* UPJMontageActionData::GetRandomMontageForTag(const FGameplayTag& GroupTag) const
{
    if (MontageGroupMap.Contains(GroupTag))
    {
        const FPJMontageGroup& MontageGroup = MontageGroupMap[GroupTag];
        const int32 RendomIndex = FMath::RandRange(0, MontageGroup.Animations.Num() - 1);
        return GetMontageForTag(GroupTag, RendomIndex);
    }
    return nullptr;
}
```
-DataAsset을 이용하면 아티스트/디자이너가 에디터에서 몽타주를 쉽게 바꿀 수 있고 빌드/테스트가 편리, 또한 C++ 코드는 몽타주 플레이 로직에만 집중하게 되어 유지보수성이 좋아짐

---

3) 무기 충돌 & 히트 판정
- 파일: Source/Project_J/Components/PJWeaponCollisionComponent.cpp
- 사용 기술: UKismetSystemLibrary::SphereTraceMultiForObjects, 델리게이트 브로드캐스트로 데미지/피격 파이프라인 분리
- 물리/트레이스 판정은 컴포넌트 단에서 처리하고, 결과는 델리게이트로 전달하여 데미지 처리/효과 적용을 분리.
- DrawDebugType를 통해 디버그 시각화가 가능하도록 해 테스트 편의성 확보.
```c++
// 히트 판정과 브로드캐스트 부분 
// Source/Project_J/Components/PJWeaponCollisionComponent.cpp
const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
    GetOwner(),
    Start,
    End,
    TraceRadius,
    TraceObjectTypes,
    false,
    IgnoredActors,
    DrawDebugType,
    OutHits,
    true
);

if (bHit)
{
    for (const FHitResult& Hit : OutHits)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor == nullptr) continue;

        if (CanHitActor(HitActor))
        {
            AlreadyHitActors.Add(HitActor);

            if (OnHitActor.IsBound())
            {
                OnHitActor.Broadcast(Hit);
            }
        }
    }
}

```
---

4) 델리게이트 활용(애니메이션 종료 처리)
- 파일: Source/Project_J/AI/Task/BTTaskNode_PerformAttack.cpp
- 사용 기술: FOnMontageEnded 타입 델리게이트, 람다 바인딩, StateComponent 접근, FinishLatentTask 호출으로 BehaviorTree 작업 종료 통보
```c++
//MontageEndedDelegate 및 람다 바인딩
FOnMontageEnded MontageEndedDelegate;

MontageEndedDelegate.BindLambda([this, &OwnerComp, ControllerPawn](UAnimMontage* Montage, bool bInterrupted)
{
    UE_LOG(LogTemp, Log, TEXT("Execute MontageEndedDelegate"));

    if (::IsValid(&OwnerComp) == false)
    {
        return;
    }

    if (UPJStateComponent* StateComponent = ControllerPawn->GetComponentByClass<UPJStateComponent>())
    {
        StateComponent->ClearState();
    }

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
});
```

---
5) Behavior Tree 확장(Decorator / Task / Service)
- 파일: Source/Project_J/AI/Decorator/BTDecorator_Chance.h/.cpp
  - 난수 기반 조건으로 확률적인 행동 분기 구현:
```c++
bool UBTDecorator_Chance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    return ChanceRate > FMath::RandRange(1, 100);
}
```
- 파일: Source/Project_J/AI/Task/BTTaskNode_PerformAttack.h/.cpp
  - IPJCombatInterface를 통해 공격 실행을 위임하고, 애니메이션 종료 콜백으로 태스크 성공/실패를 알림
- 파일: Source/Project_J/AI/Service/BTService_SelectBehavior.h
  - Blackboard key 셋/업데이트, 거리 기반 행동 전환(AttackRangeDistance) 등을 통해 행동 상태(순찰/추적/공격) 결정
- Behavior Tree 노드에서 직접 애니메이션/상태를 변경하지 않고 인터페이스를 통해 분리(IPJCombatInterface). 이로써 테스트/유닛화/재사용성이 높아짐

---

6) 컴포넌트 설계 & 성능
- 파일: Source/Project_J/Components/PJCombatComponent.h (FORCEINLINE 사용)
```c++
FORCEINLINE APJWeapon* GetMainWeapon() const { return MainWeapon; }
FORCEINLINE FGameplayTag GetLastAttackType() const { return LastAttackType; }
```
- 빈번하게 호출되는 단순 게터는 FORCEINLINE으로 인라인화하여 호출 오버헤드를 줄임
- 컴포넌트 기반 설계(UActorComponent 파생)는 책임 분리(Combat / WeaponCollision / State 등)를 자연스럽게 해 로직 테스트 및 재사용을 용이하게 함

---

7) 인터페이스 기반 설계
- 파일: Source/Project_J/Interfaces/*.cpp (인터페이스 스텁 존재)
- 인터페이스로 전투/상호작용 API를 정의하면 Pawn, NPC, 오브젝트 등 여러 타입이 동일 계약을 구현할 수 있어 Behavior Tree / UI / 데미지 시스템에서 일관된 호출 가능

---


---
