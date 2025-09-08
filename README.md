# Project_J — 기술 심화 설명 (Unreal Engine + C++ 전문성 강조)

이 문서는 Project_J의 실제 소스(Repository의 Source 폴더)를 기반으로, 면접관이 언리얼 엔진과 C++ 숙련도를 바로 파악할 수 있도록 "구현된 기능", "사용된 엔진/언어 기법", 그리고 "핵심 코드 발췌 + 전문가 관점의 해설"을 제공합니다. 실제 파일을 인용하고(경로 표기) 필요하면 안전·성능 관점에서의 개선 예시도 함께 제시합니다.

---

요약(핵심 파일 근거)
- Gameplay Tag: Source/Project_J/PJGameplayTags.h / PJGameplayTags.cpp
- Montage 데이터: Source/Project_J/Data/PJMontageActionData.h / .cpp
- 전투/무기 충돌: Source/Project_J/Components/PJWeaponCollisionComponent.cpp, Components/PJCombatComponent.h, Equipments/PJWeapon.h, Equipments/PJFistWeapon.h
- AI (Behavior Tree): Source/Project_J/AI/Decorator/BTDecorator_Chance.h/.cpp, AI/Task/BTTaskNode_PerformAttack.h/.cpp, AI/Service/BTService_SelectBehavior.h
- 인터페이스: Source/Project_J/Interfaces/*.cpp (PJCombatInterface, PJInteract, PJTargeting)
- 모듈/진입점: Source/Project_J/Project_J.cpp

아래 섹션은 각 주제별로 "무엇을 구현했는지", "왜 그렇게 했는지", "어떤 언리얼/C++ 기법을 사용했는지", 그리고 "핵심 코드(실제 발췌) + 개선/모범 예시"로 구성됩니다.

---

1) Gameplay Tags — 상태/액션 분류 (데이터-드리븐)
- 파일: Source/Project_J/PJGameplayTags.h / PJGameplayTags.cpp
- 구현 의도: Character 상태(Attacking, Rolling 등)와 액션/공격 타입을 네이티브 GameplayTag로 선언/정의하여 애니메이션/AI/로직에서 일관된 태그 기반 분기 처리를 하도록 설계.
- 사용 기술: Native Gameplay Tags (UE_DECLARE_GAMEPLAY_TAG_EXTERN / UE_DEFINE_GAMEPLAY_TAG)
- 발췌:
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
- 전문성 포인트: GameplayTag을 네이티브로 선언하면 런타임 문자열 비교를 피하고, 에디터/블루프린트와의 연동, DataDriven 구성(예: Montage 매핑)에서 매우 유리합니다.

---

2) Montage / 애니메이션 데이터 분리 (DataAsset)
- 파일: Source/Project_J/Data/PJMontageActionData.h/.cpp
- 구현 의도: FGameplayTag를 키로 하는 Montage 그룹(TMap)을 만들어 애니메이션을 코드가 아니라 데이터로 관리. 몽타주 선택/랜덤화를 DataAsset에서 처리.
- 발췌:
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
- 전문성 포인트: DataAsset(PrimaryDataAsset)을 이용하면 아티스트/디자이너가 에디터에서 몽타주를 쉽게 바꿀 수 있고 빌드/테스트가 편리합니다. 또한 C++ 코드는 몽타주 플레이 로직에만 집중하게 되어 유지보수성이 좋아집니다.

---

3) 무기 충돌 & 히트 판정 — Trace 기반, 이벤트/델리게이트로 분리
- 파일: Source/Project_J/Components/PJWeaponCollisionComponent.cpp
- 실제 발췌(히트 판정과 브로드캐스트 부분):
```c++
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
- 사용 기술: UKismetSystemLibrary::SphereTraceMultiForObjects (멀티 히트 판정), 콜백(델리게이트) 브로드캐스트로 데미지/피격 파이프라인 분리
- 전문성 포인트:
  - 물리/트레이스 판정은 컴포넌트 단에서 처리하고, 결과는 델리게이트로 방송하여 데미지 처리/효과 적용을 분리(단일 책임 원칙).
  - DrawDebugType를 통해 디버그 시각화가 가능하도록 해 테스트 편의성 확보.

- 개선 권장(메모리/안정성/중복 방지):
  - AlreadyHitActors가 AActor* 포인터 배열로 중복/댕글링(ptr to destroyed actor) 문제를 가질 수 있음. 권장: TSet<TWeakObjectPtr<AActor>> 또는 TSet<AActor*> + IsValid 체크.
  - 예시 (권장 코드):
```c++
private:
    TSet<TWeakObjectPtr<AActor>> AlreadyHitActorSet;

... // 판정 부분
if (CanHitActor(HitActor))
{
    if (!AlreadyHitActorSet.Contains(HitActor))
    {
        AlreadyHitActorSet.Add(HitActor);
        if (OnHitActor.IsBound()) OnHitActor.Broadcast(Hit);
    }
}
```
  - TWeakObjectPtr 사용으로 가비지 컬렉션 이후 접근 안전성(Actor가 이미 Destroy된 경우 IsValid 체크로 방지)을 확보할 수 있음.

---

4) 델리게이트 활용(애니메이션 종료 처리) — 람다 바인딩 및 안전성
- 파일: Source/Project_J/AI/Task/BTTaskNode_PerformAttack.cpp
- 실제 발췌 (MontageEndedDelegate 및 람다 바인딩):
```c++
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
- 사용 기술: FOnMontageEnded 타입 델리게이트, 람다 바인딩, StateComponent 접근, FinishLatentTask 호출으로 BehaviorTree 작업 종료 통보
- 전문성 포인트 및 주의사항:
  - 람다에서 로컬 참조(&OwnerComp)를 캡처하면, 델리게이트가 비동기로 호출될 때 참조가 유효하지 않을 위험이 있음. 안전하게 하려면 포인터를 값으로 캡처(UBehaviorTreeComponent* OwnerCompPtr = &OwnerComp; 캡처 OwnerCompPtr)하고 IsValid(OwnerCompPtr)로 체크합니다.
  - 예시(안전한 캡처):
```c++
UBehaviorTreeComponent* OwnerCompPtr = &OwnerComp;
APawn* CapturedPawn = ControllerPawn;
MontageEndedDelegate.BindLambda([this, OwnerCompPtr, CapturedPawn](UAnimMontage* Montage, bool bInterrupted)
{
    if (OwnerCompPtr == nullptr || !IsValid(OwnerCompPtr)) return;
    if (CapturedPawn == nullptr || !IsValid(CapturedPawn)) return;

    if (UPJStateComponent* StateComponent = CapturedPawn->GetComponentByClass<UPJStateComponent>())
    {
        StateComponent->ClearState();
    }

    FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Succeeded);
});
```
  - 이유: 람다가 호출되는 시점은 원래 함수 스택이 사라진 이후일 수 있어 포인터/참조 안정성이 중요합니다.

---

5) Behavior Tree 확장(Decorator / Task / Service) — AI 파이프라인 설계
- 파일: Source/Project_J/AI/Decorator/BTDecorator_Chance.h/.cpp
  - 난수 기반 조건(ChanceRate)으로 확률적인 행동 분기 구현:
```c++
bool UBTDecorator_Chance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    return ChanceRate > FMath::RandRange(1, 100);
}
```
- 파일: Source/Project_J/AI/Task/BTTaskNode_PerformAttack.h/.cpp
  - IPJCombatInterface를 통해 공격 실행을 위임하고, 애니메이션 종료 콜백으로 태스크 성공/실패를 알림.
- 파일: Source/Project_J/AI/Service/BTService_SelectBehavior.h
  - Blackboard key 셋/업데이트, 거리 기반 행동 전환(AttackRangeDistance) 등을 통해 행동 상태(순찰/추적/공격) 결정
- 전문성 포인트:
  - Behavior Tree 노드에서 직접 애니메이션/상태를 변경하지 않고 인터페이스를 통해 분리(IPJCombatInterface). 이로써 테스트/유닛화/재사용성이 높아짐.
  - Blackboard 및 KeySelector를 적극적으로 사용하여 블루프린트와 C++ 간 계약을 체결.

---

6) 컴포넌트 설계 & 성능 고려
- 파일: Source/Project_J/Components/PJCombatComponent.h (FORCEINLINE 사용)
```c++
FORCEINLINE APJWeapon* GetMainWeapon() const { return MainWeapon; }
FORCEINLINE FGameplayTag GetLastAttackType() const { return LastAttackType; }
```
- 전문성 포인트:
  - 빈번하게 호출되는 단순 게터는 FORCEINLINE으로 인라인화하여 호출 오버헤드를 줄임.
  - 컴포넌트 기반 설계(UActorComponent 파생)는 책임 분리(Combat / WeaponCollision / State 등)를 자연스럽게 해 로직 테스트 및 재사용을 용이하게 함.

---

7) 인터페이스 기반 설계 (IPJCombatInterface 등)
- 파일: Source/Project_J/Interfaces/*.cpp (인터페이스 스텁 존재)
- 전문성 포인트:
  - 인터페이스로 전투/상호작용 API를 정의하면 Pawn, NPC, 오브젝트 등 여러 타입이 동일 계약을 구현할 수 있어 Behavior Tree / UI / 데미지 시스템에서 일관된 호출 가능.
  - 테스트/Mock 구현을 쉽게 만들어 단위 테스트 환경 구축이 용이.

---

8) 안정성/메모리/GC 관련 실무 팁 (이 프로젝트에 권장/적용 가능한 기법)
- UObject 포인터는 UPROPERTY로 관리하거나 TWeakObjectPtr 사용: GC에 의한 댕글링 포인터 방지.
- 델리게이트 -> 람다 캡처는 '값 캡처(포인터값)'을 선호: 참조 캡처는 위험.
- 반복적/핫스팟 코드(예: 매 프레임 체크)는 가능하면 빈도 줄이기(쿨타임, 타임 슬라이스) 또는 쿼드 트리/체크 간격 적용.
- 데이터-드리븐(태그, DataAsset)으로 구현한 부분은 유닛 테스트/에디터 툴로 에셋 무결성 검사 자동화 권장.

---

9) README에 바로 넣을 수 있는 "심화 섹션" 예시 (면접관용 — 복사해서 사용)
아래 섹션은 본 README에 추가할 수 있는 고급 기술 설명 텍스트입니다. 면접관은 이 섹션만으로도 C++/언리얼 사용 방식, 안전성 고려, 아키텍처 의도를 빠르게 이해할 수 있습니다.

```markdown
심화: 언리얼/C++ 사용 기술 요약

- 데이터-드리븐 애니메이션: UPJMontageActionData (PrimaryDataAsset)로 FGameplayTag -> MontageGroup 매핑을 제공. 런타임에서 태그 기반 몽타주 선택(인덱스별, 랜덤)을 수행하여 코드-애셋 분리를 달성.
- 델리게이트 & 람다: 애니메이션 종료 콜백(FOnMontageEnded)에 람다 바인딩을 사용하여 BTTask의 비동기 완료를 처리. 실제 코드에서는 안전한 포인터 캡처(IsValid 체크)로 스택/객체 유효성 보장.
- 충돌/판정 파이프라인: UKismetSystemLibrary::SphereTraceMultiForObjects로 다중 히트 판정 수행 후 OnHitActor 델리게이트를 통해 데미지/이팩트 처리 분리. AlreadyHitActors 관리는 TSet<TWeakObjectPtr<AActor>> 권장(중복/댕글링 회피).
- AI 확장: BehaviorTree의 Decorator/Service/Task를 C++로 확장(확률 기반 Decorator, 행동 선택 Service, 애니메이션 연동 Task). IPJCombatInterface를 통해 Attack 명령을 위임, 모듈 간 결합도 최소화.
- 성능/안정성: FORCEINLINE 게터, UPROPERTY로 GC 관리, TWeakObjectPtr 사용 권장, 디버그 시각화를 위한 DrawDebug 옵션 제공.
```

---

마무리 / 다음 제안
- 원하시면 README에 위 '심화 섹션'을 붙여 최종 README.md 파일을 만들어 드리겠습니다(프로젝트 루트에 직접 추가하는 커밋 메시지까지 생성 가능).  
- 또는 각 기능(WeaponCollision, Montage 시스템, BTTask)의 전체 함수 바디를 발췌해 "문제-해결-결과" 관점으로 기술한 상세 섹션(면접 답변용 포인트 포함)도 만들어 드릴 수 있습니다.  
어떤 항목을 README에 더 부각시키길 원하시는지 알려주시면, 그에 맞춰 바로 README 마크다운을 생성해 드리겠습니다.
