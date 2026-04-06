# EchoChamber 코딩 컨벤션 (Code Convention)

본 문서는 **EchoChamber** 프로젝트에서 준수해야 할 C++ 코딩 표준 및 가이드라인입니다. 모든 팀원은 코드 리뷰 및 작성 시 아래의 규칙을 준수해야 합니다.

---

## 📝 1. `const` 위치 (East Const)
- `const` 키워드는 **항상 타입의 뒤쪽**에 붙입니다. (East Const 스타일 권장)
- 예시:
  ```cpp
  // ❌ Bad (West Const)
  const int Count = 10;
  const FString& Name;

  // 🟢 Good (East Const)
  int const Count = 10;
  FString const& Name;
  ```

## 🔄 2. `auto` 키워드 사용
- **람다(Lambda) 표현식**, **복잡한 템플릿 타입(Template Types)**, 그리고 **범위 기반 for 루프(Ranged-for loop)**에서는 `auto` 키워드를 우선적으로 사용합니다.
- 단순 스칼라 타입이나 명확하게 타입을 명시해야 하는 경우에는 지양합니다.
- 예시:
  ```cpp
  // 람다 표현식
  auto const myLambda = []() { ... };
  
  // 복잡한 템플릿/이터레이터
  auto const it = myMap.Find(Key);
  
  // Ranged-for loop
  for (auto const& item : ItemArray) { ... }
  ```

## 🏗️ 3. 생성자 (Constructors)
- 1개 이상의 파라미터를 받는 객체의 생성자(Parameterized Constructor)는 의도치 않은 암시적 형변환(Implicit Conversion)을 막기 위해 **반드시 `explicit` 키워드**를 지정해야 합니다.
- 예시:
  ```cpp
  class FUserConfig
  {
  public:
      // 🟢 Good
      explicit FUserConfig(int const configId);
  };
  ```

## 🔡 4. 명명 규칙 (Naming Convention)
- **일반 변수명**: 소문자로 시작하는 카멜 케이스(`lowerCamelCase`)를 사용합니다.
- **멤버 변수명**: 언더바(`_`) 접두사 + 카멜 케이스(`_lowerCamelCase`)를 사용합니다.
- 예시:
  ```cpp
  class FPlayerStats
  {
  private:
      int32 _currentHealth;   // 멤버 변수
      float _moveSpeed;       // 멤버 변수

  public:
      void UpdateHealth(int32 const newHealth)
      {
          int32 const maxLimit = 100; // 일반 지역 변수
          _currentHealth = FMath::Min(newHealth, maxLimit);
      }
  };
  ```

## 🔀 5. 함수 파라미터 전달 (Function Parameters)
- **객체 타입**: 불필요한 복사 오버헤드를 줄이기 위해 **상수 레퍼런스(`const&`)**로 전달합니다. (예: `FString`, `TArray`, 구조체 등)
- **프리미티브 타입**: 레지스터를 활용한 컴파일러 최적화를 극대화하기 위해 레퍼런스 대신 **값(Value)**으로 직접 전달합니다. (예: `int`, `float`, `bool` 등)
- 예시:
  ```cpp
  // ❌ Bad (프리미티브를 레퍼런스로 전달, 객체를 값으로 전달)
  void ProcessData(int const& index, FString data);

  // 🟢 Good
  void ProcessData(int const index, FString const& data);
  ```

## 🧱 6. 구조체 초기화 (Struct Initialization)
- 구조체 초기화 시 별도의 생성자를 만들어 호출하는 대신, **중괄호 초기화(C++20 Designated Initializers)** 방식을 사용합니다.
- 예시:
  ```cpp
  // ❌ Bad (생성자 호출 방식)
  FItemData Data = FItemData(10, 3.14f);

  // 🟢 Good (필드 지정 중괄호 초기화)
  FItemData Data
  {
      .Id = 10,
      .Weight = 3.14f
  };
  ```

## 🔗 7. 언리얼 오브젝트 포인터 (UObject Pointers)
- 언리얼 엔진 5의 권장 사항에 따라 가비지 컬렉터(GC) 성능 최적화 및 안정성을 위해 `UObject` 생 포인터(`*`) 대신 **`TObjectPtr<>`** 래퍼를 사용합니다.
- 단, 함수나 지역 변수 내부에서는 여전히 생 포인터 사용이 유효하며, 주로 `UPROPERTY()`로 노출되는 클래스 멤버 변수에 `TObjectPtr<>`을 엄격히 적용합니다.
- 예시:
  ```cpp
  // ❌ Bad
  UPROPERTY()
  AActor* TargetActor;

  // 🟢 Good
  UPROPERTY()
  TObjectPtr<AActor> TargetActor;
  ```

## 🛠️ 8. 멤버 변수 기본 초기화 (Member Variable Initialization)
- 변수 초기화가 필요한 프리미티브 타입(int, float, bool 등) 및 내부적으로 초기화가 보장되지 않는 언리얼 구조체(`FVector`, `FRotator` 등)는 클래스나 구조체 선언 시 **빈 중괄호 `{}`를 사용하여 반드시 명시적으로 초기화**해야 합니다.
- 예시:
  ```cpp
  class FTransformData
  {
  private:
      // ❌ Bad (가비지 값 발생 위험)
      int32 _id;
      FVector _position;
      FRotator _rotation;

      // 🟢 Good (안전하게 0 또는 기본값 초기화)
      int32 _id{};
      FVector _position{};
      FRotator _rotation{};
  };
  ```

## 🖥️ 9. 위젯 클래스 선언 (Widget Class Declaration)
- `UCommonUserWidget` 기반의 UI 위젯 클래스를 선언할 때는 불필요한 매 프레임 업데이트(Tick)를 막아 성능을 극대화하기 위해 **`UCLASS(meta=(DisableNativeTick))`** 메타 태그를 항상 적용해야 합니다.
- 특정 위젯에서 반드시 Tick이 필요한 경우가 아니라면 기본적으로 차단합니다.
- 예시:
  ```cpp
  // ❌ Bad
  UCLASS()
  class UMyButtonWidget : public UCommonUserWidget
  {
      GENERATED_BODY()
  };

  // 🟢 Good
  UCLASS(meta=(DisableNativeTick))
  class UMyButtonWidget : public UCommonUserWidget
  {
      GENERATED_BODY()
  };
  ```

## 🚫 10. 논리 부정 연산자 (Logical NOT Operator)
- 현재 프로젝트의 `EventDispatcherSubsystem` 등 전반적인 코드 작성 패턴을 볼 때, 논리 부정 연산 시 가독성을 떨어뜨리는 느낌표(`!`) 기호 대신 C++ 표준 대체 토큰인 **`not` 키워드**를 일관되게 사용하고 있습니다. 이를 컨벤션으로 확립합니다.
- 예시:
  ```cpp
  // ❌ Bad
  if (!bIsValid || !ObjectPtr.IsValid())
  {
      return;
  }

  // 🟢 Good
  if (not bIsValid || not ObjectPtr.IsValid())
  {
      return;
  }
  ```

## 11. 기타
- 얼리 리턴 등의 가벼운 구분이더라도 반드시 중괄호 블록 사용 및 개행 처리
- if 문 등 조건 비교 시 리터럴 상수를 좌측에 표기
- null 체크 및 조건 체크 블록이 중첩되지 않도록 early-return 활용
