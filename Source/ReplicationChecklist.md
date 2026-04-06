# 리플리케이션 데이터 흐름 체크리스트

분석 대상: `GameMode`, `GameState`, `PlayerController`, `PlayerState`, `Character`, `HUD`

---

## [심각] 확실한 런타임 버그

### ✅ 1. PlayerState OnRep에서 로컬 플레이어 확인 누락
**파일**: `Private/GameFramework/RoomEscapeFPSPlayerState.cpp`  
**함수**: `OnRep_InventoryInfo()`, `OnRep_PipeGameSuccessInfo()`

**문제**  
PlayerState는 기본적으로 모든 클라이언트에 리플리케이트된다.  
`OnRep_InventoryInfo`에서 `GetPlayerController(GetWorld(), 0)->GetHUD()`로 로컬 HUD를 수정하는데,  
다른 플레이어의 PlayerState OnRep에서도 이 코드가 실행되어 **로컬 HUD가 타 플레이어 데이터로 덮어씌워진다**.

**수정 내용**  
두 함수 진입부에 소유자 확인 early-return 추가:
```cpp
APawn* pawn = GetPawn();
if (not pawn || not pawn->IsLocallyControlled() || GetNetMode() != NM_Client)
{
    return;
}
```
> `OnRep_InitializePipeGame()`은 이미 올바르게 처리하고 있었음.

---

### ✅ 2. OnRep 콜백에서 Replicated 변수 직접 변경
**파일**: `Private/GameFramework/RoomEscapeFPSPlayerState.cpp`  
**함수**: `OnRep_InventoryInfo()` 내 `fFlashIntensity` 대입  
**변수**: `fFlashIntensity` (`UPROPERTY(Replicated)`)

**문제**  
클라이언트 OnRep 내부에서 Replicated 속성을 변경하면,  
다음 리플리케이션 주기에 **서버 값(0)이 클라이언트 값을 덮어써** 광량이 0으로 리셋된다.

**수정 내용**  
- `fFlashIntensity` → `UPROPERTY(ReplicatedUsing = OnRep_FlashIntensity)`로 변경
- `UpdateFlashIntensityByBattery()` 서버 전용 함수 추가, `UpdateBatteryRemainValue()` 호출 시 함께 계산
- `OnRep_FlashIntensity()` 추가 → `character->UpdateFlashIntensity(fFlashIntensity)` 호출
- `OnRep_InventoryInfo`에서 fFlashIntensity 관련 코드 제거

---

### ✅ 3. 클라이언트 예측 UI와 서버 권위 상태 불일치
**파일**: `Private/Character/RoomEscapeFPSCharacter.cpp`  
**함수**: `OnFire()`

**문제**  
클라이언트에서 `ServerOnFire()` 호출 전에 HUD 부적 카운트를 미리 차감(예측 업데이트)한다.  
서버에서 아이템 부족 등 이유로 처리가 실패해도 **롤백 메커니즘이 없어 UI와 실제 상태가 불일치**한다.

**수정 내용**  
클라이언트 측 UI 선행 업데이트 코드 제거.  
서버에서 `AddItemToInventory(-1)` → `InventoryInfo` 리플리케이션 → `OnRep_InventoryInfo`에서 UI 업데이트하는 단일 경로로 통일.

---

## [중요] 설계 및 성능 문제

### ✅ 4. 개인 데이터에 DOREPLIFETIME_CONDITION 미적용
**파일**: `Private/GameFramework/RoomEscapeFPSPlayerState.cpp`  
**함수**: `GetLifetimeReplicatedProps()`

**문제**  
인벤토리, 배터리, 파이프게임 등 플레이어 개인 데이터가 `DOREPLIFETIME`으로  
**모든 클라이언트에 브로드캐스트**된다. 불필요한 대역폭 소모 + 타 클라이언트에 민감 데이터 노출.

**수정 내용**  
개인 데이터 전체를 `DOREPLIFETIME_CONDITION(..., COND_OwnerOnly)`로 변경:
```cpp
DOREPLIFETIME_CONDITION(ARoomEscapeFPSPlayerState, PipeGameInfo,       COND_OwnerOnly);
DOREPLIFETIME_CONDITION(ARoomEscapeFPSPlayerState, bInitializePipeGame, COND_OwnerOnly);
DOREPLIFETIME_CONDITION(ARoomEscapeFPSPlayerState, PipeGameSuccessInfo, COND_OwnerOnly);
DOREPLIFETIME_CONDITION(ARoomEscapeFPSPlayerState, InventoryInfo,       COND_OwnerOnly);
DOREPLIFETIME_CONDITION(ARoomEscapeFPSPlayerState, BatteryMaxValue,     COND_OwnerOnly);
DOREPLIFETIME_CONDITION(ARoomEscapeFPSPlayerState, BatteryUpdateValue,  COND_OwnerOnly);
DOREPLIFETIME_CONDITION(ARoomEscapeFPSPlayerState, fFlashIntensity,     COND_OwnerOnly);
```

---

### ✅ 5. 게임플레이 핵심 정보를 Unreliable Client RPC로 전송
**파일**: `Public/GameFramework/RoomEscapeFPSPlayerController.h`

**문제**  
아래 두 RPC가 `Unreliable`로 선언되어 패킷 손실 시 플레이어가 게임 진행 정보를 영구적으로 놓친다:
- `ClientProcessHUDOnFirstItemGet` — 아이템 최초 획득 팝업
- `ClientProcessHUDOnUpdateNextInfo` — 다음 목표 안내 업데이트

**수정 내용**  
두 함수를 `Reliable`로 변경.

---

### ✅ 6. GameState에 Server RPC 선언
**파일**: `Public/GameFramework/RoomEscapeFPSGameState.h`  
**함수**: `ServerIncreaseGhostDeadCount` (`UFUNCTION(Server, Reliable)`)

**문제**  
Server RPC는 **클라이언트가 소유한 액터**에서 클라이언트→서버 호출을 위해 사용한다.  
GameState는 서버가 소유하며, 실제 호출처도 서버 사이드인 `GhostAIController`이다.  
서버에서 Server RPC를 호출하면 `_Implementation`이 로컬에서 직접 실행되므로 **동작은 하지만**,  
의미적으로 잘못된 사용이며 불필요한 RPC 오버헤드가 발생한다.

**수정 내용**  
`UFUNCTION(Server, Reliable)` 제거 → 일반 함수로 변경.  
함수명: `ServerIncreaseGhostDeadCount` → `IncreaseGhostDeadCount`  
호출처(`GhostAIController.cpp`)도 동일하게 변경.

---

## [경미] 참고 사항 (미수정)

### ⏭ 7. Server RPC Validate가 형식적
**파일**: `Private/Character/RoomEscapeFPSCharacter.cpp`  
**함수**: `ServerOnUse_Validate`, `ServerOnFlash_Validate`, `ServerOnFire_Validate`

모두 `return true`만 반환. Validate에서 실질적 검증(소지 아이템, 범위 등)을 수행하지 않아  
이론적으로 치트 클라이언트가 서버 로직을 임의로 실행 가능.  
현재 서버 측 `_Implementation`에서 재검증하고 있으므로 치명적이지는 않음.

---

*분석 기준일: 2026-04-04*
