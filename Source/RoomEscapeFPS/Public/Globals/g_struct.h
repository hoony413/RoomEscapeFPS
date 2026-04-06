// Generated C++ header file
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "g_enum.h"
#include "g_struct.generated.h"

USTRUCT(BlueprintType)
struct FSkillNo final
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    int32 _value{};

    // Constructors
    FSkillNo() = default;
    explicit FSkillNo(int32 InValue) : _value(InValue) {}

    // Arithmetic operators
    FSkillNo operator+(const FSkillNo& Other) const { return FSkillNo(_value + Other._value); }
    FSkillNo operator-(const FSkillNo& Other) const { return FSkillNo(_value - Other._value); }
    FSkillNo operator*(const FSkillNo& Other) const { return FSkillNo(_value * Other._value); }
    FSkillNo operator/(const FSkillNo& Other) const { return FSkillNo(_value / Other._value); }

    // Compound assignment operators
    FSkillNo& operator+=(const FSkillNo& Other) { _value += Other._value; return *this; }
    FSkillNo& operator-=(const FSkillNo& Other) { _value -= Other._value; return *this; }
    FSkillNo& operator*=(const FSkillNo& Other) { _value *= Other._value; return *this; }
    FSkillNo& operator/=(const FSkillNo& Other) { _value /= Other._value; return *this; }

    // Comparison operators
    bool operator==(const FSkillNo& Other) const { return _value == Other._value; }
    bool operator!=(const FSkillNo& Other) const { return _value != Other._value; }
    bool operator<(const FSkillNo& Other) const { return _value < Other._value; }
    bool operator<=(const FSkillNo& Other) const { return _value <= Other._value; }
    bool operator>(const FSkillNo& Other) const { return _value > Other._value; }
    bool operator>=(const FSkillNo& Other) const { return _value >= Other._value; }

    // Conversion to int32
    explicit operator int32() const { return _value; }
};

FORCEINLINE uint32 GetTypeHash(const FSkillNo& no)
{
    return GetTypeHash(no._value);
}

USTRUCT(BlueprintType)
struct ROOMESCAPEFPS_API FTableSkillRow final : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FSkillNo No;

    UPROPERTY(EditAnywhere)
    EEnumTest Name{};

    UPROPERTY(EditAnywhere)
    float Rate{};

    UPROPERTY(EditAnywhere)
    bool OnOff{};

    UPROPERTY(EditAnywhere)
    TArray<int32> AdditionalDatas;
};

USTRUCT(BlueprintType)
struct FTestNo final
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    int32 _value{};

    // Constructors
    FTestNo() = default;
    explicit FTestNo(int32 InValue) : _value(InValue) {}

    // Arithmetic operators
    FTestNo operator+(const FTestNo& Other) const { return FTestNo(_value + Other._value); }
    FTestNo operator-(const FTestNo& Other) const { return FTestNo(_value - Other._value); }
    FTestNo operator*(const FTestNo& Other) const { return FTestNo(_value * Other._value); }
    FTestNo operator/(const FTestNo& Other) const { return FTestNo(_value / Other._value); }

    // Compound assignment operators
    FTestNo& operator+=(const FTestNo& Other) { _value += Other._value; return *this; }
    FTestNo& operator-=(const FTestNo& Other) { _value -= Other._value; return *this; }
    FTestNo& operator*=(const FTestNo& Other) { _value *= Other._value; return *this; }
    FTestNo& operator/=(const FTestNo& Other) { _value /= Other._value; return *this; }

    // Comparison operators
    bool operator==(const FTestNo& Other) const { return _value == Other._value; }
    bool operator!=(const FTestNo& Other) const { return _value != Other._value; }
    bool operator<(const FTestNo& Other) const { return _value < Other._value; }
    bool operator<=(const FTestNo& Other) const { return _value <= Other._value; }
    bool operator>(const FTestNo& Other) const { return _value > Other._value; }
    bool operator>=(const FTestNo& Other) const { return _value >= Other._value; }

    // Conversion to int32
    explicit operator int32() const { return _value; }
};

FORCEINLINE uint32 GetTypeHash(const FTestNo& no)
{
    return GetTypeHash(no._value);
}

USTRUCT(BlueprintType)
struct ROOMESCAPEFPS_API FTableTestRow final : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FTestNo No;

    UPROPERTY(EditAnywhere)
    EEnumTest Name{};

    UPROPERTY(EditAnywhere)
    float Rate{};

    UPROPERTY(EditAnywhere)
    bool OnOff{};
};

USTRUCT(BlueprintType)
struct FTest2No final
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    int32 _value{};

    // Constructors
    FTest2No() = default;
    explicit FTest2No(int32 InValue) : _value(InValue) {}

    // Arithmetic operators
    FTest2No operator+(const FTest2No& Other) const { return FTest2No(_value + Other._value); }
    FTest2No operator-(const FTest2No& Other) const { return FTest2No(_value - Other._value); }
    FTest2No operator*(const FTest2No& Other) const { return FTest2No(_value * Other._value); }
    FTest2No operator/(const FTest2No& Other) const { return FTest2No(_value / Other._value); }

    // Compound assignment operators
    FTest2No& operator+=(const FTest2No& Other) { _value += Other._value; return *this; }
    FTest2No& operator-=(const FTest2No& Other) { _value -= Other._value; return *this; }
    FTest2No& operator*=(const FTest2No& Other) { _value *= Other._value; return *this; }
    FTest2No& operator/=(const FTest2No& Other) { _value /= Other._value; return *this; }

    // Comparison operators
    bool operator==(const FTest2No& Other) const { return _value == Other._value; }
    bool operator!=(const FTest2No& Other) const { return _value != Other._value; }
    bool operator<(const FTest2No& Other) const { return _value < Other._value; }
    bool operator<=(const FTest2No& Other) const { return _value <= Other._value; }
    bool operator>(const FTest2No& Other) const { return _value > Other._value; }
    bool operator>=(const FTest2No& Other) const { return _value >= Other._value; }

    // Conversion to int32
    explicit operator int32() const { return _value; }
};

FORCEINLINE uint32 GetTypeHash(const FTest2No& no)
{
    return GetTypeHash(no._value);
}

USTRUCT(BlueprintType)
struct ROOMESCAPEFPS_API FTableTest2Row final : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FTest2No No;

    UPROPERTY(EditAnywhere)
    FName Name;

    UPROPERTY(EditAnywhere)
    int32 Count{};

    UPROPERTY(EditAnywhere)
    bool OnOff{};
};
