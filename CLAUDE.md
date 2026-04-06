# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 프로젝트 개요

UE 5.7 기반 멀티플레이 방탈출 FPS 게임. 단일 C++ 모듈 (`RoomEscapeFPS`), 데디케이티드 서버 권위적 복제 구조.

## 빌드

Visual Studio 솔루션 (`RoomEscapeFPS.sln`)으로 빌드하거나 UE 에디터 내 빌드 버튼 사용.

세 가지 빌드 타겟:
- `RoomEscapeFPS` (Game)
- `RoomEscapeFPSEditor` (Editor)
- `RoomEscapeFPSServer` (Dedicated Server)

## 코드 생성

DataTable 관련 파일 변경 시 전체 파이프라인 재실행:

```bat
Tools\ExportTable.bat
```

**자동 생성 파일 — 직접 편집 금지:**
- `Source/RoomEscapeFPS/Public/Globals/g_enum.h` (`Yaml/enum.yaml` → `enum_generator.py`)
- `Source/RoomEscapeFPS/Public/Globals/g_struct.h` (`ExcelTable/*.xlsx` → `struct_generator.py`)
- `Source/RoomEscapeFPS/Public/Subsystems/TableSubsystem.h/.cpp` (`g_struct.h` → `TableSubsystemGenerator.py`)

## 아키텍처 핵심

### 서브시스템 (전역 서비스)
`URoomEscapeFPSGameInstance`는 빈 셸. 모든 전역 서비스는 `UGameInstanceSubsystem`으로 구현.

```cpp
Helper::GetSubsystem<UUISubsystem>(GetWorld())  // Helper.h 단축 접근
```

| 서브시스템 | 역할 |
|-----------|------|
| `UUISubsystem` | 위젯 풀 관리 (`FUserWidgetPool`), 로딩화면 |
| `UTableSubsystem` | 자동 생성 DataTable 접근자 |
| `UEventDispatcherSubsystem` | 타입 안전 이벤트 버스 |
| `UViewModelSubsystem` | MVVM ViewModel 팩토리 |
| `UContentSubsystem` | 이벤트 수신 가능한 서브시스템 기반 클래스 |

### UI 위젯 기반 클래스
두 가지 기반 클래스 — 전체화면 전환 가능 위젯은 `UBaseActivatableWidget`, 그 외는 `UBaseWidget`.

| 기반 클래스 | 부모 | 사용처 |
|------------|------|--------|
| `UBaseActivatableWidget` | `UCommonActivatableWidget` | 전체화면 페이지 (InventoryPanel 등) |
| `UBaseWidget` | `UCommonUserWidget` | 팝업/노드 위젯 |
| `UBaseHUDWidget` | `UCommonUserWidget` | 루트 HUD 컨테이너 |

### 이벤트 시스템
UI → 서브시스템 단방향. `EventTypeValues.inl`에 `DECLARE_EVENT_TYPE`으로 선언.

```cpp
// 수신 (UContentSubsystem 서브클래스만 가능)
BindEvent<EEventType::MyEvent>(this, &UMySubsystem::Handler);
// 송신 (UBaseActivatableWidget 서브클래스만 가능)
Dispatch<EEventType::MyEvent>(args...);
```

### 인터랙션 객체 계층
`AInteractiveObject` → `AGetableObject` → `AClueObject` → `ATriggerObject`  
`AInteractiveObject` → `ADynamicMtInteractiveObject` → `ASwitchButtonObject`  
퍼즐 정답 검증은 `ASolutionWrapper`가 담당.

## 네트워크 규칙

### Server RPC 작성 시 필수 사항
- `Server, Reliable` RPC에는 반드시 `WithValidation` 지정. `_Validate`에서 입력 파라미터 범위 검사 및 권한 검증 수행.
- `_Validate`에서 반환하는 검증을 `_Implementation` 내부에서도 방어적으로 재검증.
- 클라이언트 입력값(인덱스, 카운트 등)은 절대 신뢰하지 않음. `check()` 대신 `if` 가드 + early-return 사용.

### NetMulticast 신뢰성 기준
- 일회성 게임 진행 이벤트(퍼즐 완료 알림, 문 열림 등): `Reliable` 필수.
- 고빈도 시각 효과(투사체 폭발 등 손실 허용 가능): `Unreliable` 허용.

### 데디케이티드 서버 최적화
- 생성자에서 렌더링 전용 컴포넌트(`SceneCaptureComponent2D` 등) 생성 시 `IsRunningDedicatedServer()` 가드 적용. (생성자에서는 `IsNetMode()` 사용 불가)
- 시각 전용 Tick(타임라인 애니메이션 등)은 `IsNetMode(NM_DedicatedServer)` early-return으로 서버에서 제외.
- `uint32` 산술 시 언더플로우 방지 가드 필수.

## 코딩 컨벤션

상세 규칙은 `CodeConvention.md` 참조. 핵심만 요약:

| 규칙 | 적용 |
|------|------|
| `const` 위치 | East Const (`int const`, `FString const&`) |
| 논리 NOT | `not` 키워드 (`!` 사용 금지) |
| 멤버 변수명 | `_lowerCamelCase` (언더바 접두사) |
| UObject 포인터 | `TObjectPtr<T>` (UPROPERTY 멤버에 한함) |
| 위젯 선언 | `UCLASS(meta=(DisableNativeTick))` 필수 |
| 멤버 초기화 | 빈 중괄호 `{}` 사용 |
| 조건 비교 | 리터럴 좌측 배치 (Yoda conditions) |
| 중첩 방지 | early-return 활용 |

## 모듈 의존성 (Build.cs)

Public: `Core`, `CoreUObject`, `Engine`, `InputCore`, `AIModule`, `UMG`, `CommonUI`, `CommonInput`, `DeveloperSettings`, `ModelViewViewModel`  
Private: `Slate`, `SlateCore`, `Paper2D`
