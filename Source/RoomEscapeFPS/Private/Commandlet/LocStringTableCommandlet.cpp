#include "Commandlet/LocStringTableCommandlet.h"

#if !WITH_EDITOR
int32 ULocStringTableCommandlet::Main(FString const& params) { return -1; }
#else

#include "PackageHelperFunctions.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/TextLocalizationResource.h"
#include "Internationalization/TextKey.h"

// --- 내부 데이터 구조 ---

struct FLocEntry
{
	FString _key;
	TMap<FString, FString> _cultureToText; // culture code → text
};

struct FLocNamespace
{
	FString _name;
	TArray<FLocEntry> _entries;
};

// --- CSV 한 줄 파서 ---
// pandas to_csv 출력 기준: 쉼표 구분, 특수문자/쉼표 포함 셀은 "..." 감싸기, 내부 따옴표는 "" 이스케이프.
static TArray<FString> ParseCSVLine(FString const& line)
{
	TArray<FString> cells;
	FString current;
	bool inQuotes = false;

	for (int32 i = 0; i < line.Len(); ++i)
	{
		TCHAR const c = line[i];
		if (inQuotes)
		{
			if (TEXT('"') == c)
			{
				if (i + 1 < line.Len() && TEXT('"') == line[i + 1])
				{
					// 이스케이프된 따옴표 ("")
					current.AppendChar(TEXT('"'));
					++i;
				}
				else
				{
					inQuotes = false;
				}
			}
			else
			{
				current.AppendChar(c);
			}
		}
		else
		{
			if (TEXT('"') == c)
			{
				inQuotes = true;
			}
			else if (TEXT(',') == c)
			{
				cells.Add(current);
				current.Reset();
			}
			else
			{
				current.AppendChar(c);
			}
		}
	}
	cells.Add(current);
	return cells;
}

// --- Main ---

int32 ULocStringTableCommandlet::Main(FString const& params)
{
	// 파라미터 파싱
	FString csvPath, stringTableOutPath, nativeCulture, culturesStr;
	FParse::Value(*params, TEXT("CsvPath="), csvPath);
	FParse::Value(*params, TEXT("StringTableOutPath="), stringTableOutPath);
	FParse::Value(*params, TEXT("NativeCulture="), nativeCulture);
	FParse::Value(*params, TEXT("Cultures="), culturesStr);

	if (stringTableOutPath.IsEmpty())
	{
		stringTableOutPath = TEXT("/Game/Localization/StringTables/");
	}
	if (nativeCulture.IsEmpty())
	{
		nativeCulture = TEXT("ko");
	}
	if (culturesStr.IsEmpty())
	{
		culturesStr = TEXT("ko+en");
	}

	if (csvPath.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Usage: -run=LocStringTableCommandlet -CsvPath=\"...\" [-StringTableOutPath=...] [-NativeCulture=ko] [-Cultures=ko+en]"));
		FPlatformProcess::Sleep(3.0f);
		return -1;
	}

	// Cultures 파싱
	TArray<FString> cultures;
	culturesStr.ParseIntoArray(cultures, TEXT("+"));

	// CSV 파일 수집
	TArray<FString> csvFiles;
	IFileManager::Get().FindFiles(csvFiles, *FPaths::Combine(csvPath, TEXT("*.csv")), true, false);
	if (csvFiles.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("No CSV files found in: %s"), *csvPath);
		FPlatformProcess::Sleep(3.0f);
		return -1;
	}

	// CSV 파싱
	TArray<FLocNamespace> namespaces;
	for (FString const& csvFilename : csvFiles)
	{
		FString const namespaceName(FPaths::GetBaseFilename(csvFilename));
		FString content;
		if (not FFileHelper::LoadFileToString(content, *FPaths::Combine(csvPath, csvFilename)))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load CSV: %s"), *csvFilename);
			continue;
		}

		// BOM 스트립 (U+FEFF)
		if (content.StartsWith(TEXT("\xFEFF")))
		{
			content = content.Mid(1);
		}

		TArray<FString> lines;
		content.ParseIntoArrayLines(lines);
		if (2 > lines.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("CSV has no data rows: %s"), *csvFilename);
			continue;
		}

		// 헤더 파싱
		TArray<FString> const headers = ParseCSVLine(lines[0]);
		if (headers.IsEmpty() || not headers[0].TrimStartAndEnd().Equals(TEXT("LocalKey")))
		{
			UE_LOG(LogTemp, Error, TEXT("CSV first column must be 'LocalKey': %s"), *csvFilename);
			continue;
		}

		TArray<FString> headerCultures;
		for (int32 i = 1; i < headers.Num(); ++i)
		{
			headerCultures.Add(headers[i].TrimStartAndEnd());
		}

		FLocNamespace ns;
		ns._name = namespaceName;

		// 데이터 행 파싱
		for (int32 lineIdx = 1; lineIdx < lines.Num(); ++lineIdx)
		{
			if (lines[lineIdx].IsEmpty())
			{
				continue;
			}

			TArray<FString> const cells = ParseCSVLine(lines[lineIdx]);
			if (cells.IsEmpty())
			{
				continue;
			}

			FLocEntry entry;
			entry._key = cells[0].TrimStartAndEnd();
			if (entry._key.IsEmpty())
			{
				continue;
			}

			for (int32 colIdx = 0; colIdx < headerCultures.Num(); ++colIdx)
			{
				int32 const cellIdx = colIdx + 1;
				FString const text = (cellIdx < cells.Num()) ? cells[cellIdx] : TEXT("");
				entry._cultureToText.Add(headerCultures[colIdx], text);
			}

			ns._entries.Add(MoveTemp(entry));
		}

		UE_LOG(LogTemp, Display, TEXT("Parsed CSV '%s': %d entries"), *namespaceName, ns._entries.Num());
		namespaces.Add(MoveTemp(ns));
	}

	if (namespaces.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("No data parsed from CSV files"));
		FPlatformProcess::Sleep(3.0f);
		return -1;
	}

	// UStringTable 에셋 생성/갱신 (네이티브 culture만)
	for (FLocNamespace const& ns : namespaces)
	{
		FString const assetName(FString::Printf(TEXT("ST_%s"), *ns._name));
		FString const packagePath(FPaths::Combine(stringTableOutPath, assetName));
		FString const assetPath(FString::Printf(TEXT("%s.%s"), *packagePath, *assetName));

		UPackage* package = CreatePackage(*packagePath);
		package->FullyLoad();

		UStringTable* stringTableAsset{};
		if (auto* existing = LoadObject<UStringTable>(nullptr, *assetPath))
		{
			stringTableAsset = existing;
		}
		else
		{
			stringTableAsset = NewObject<UStringTable>(package, FName(*assetName), RF_Public | RF_Standalone);
		}

		FStringTableRef const stringTable = stringTableAsset->GetMutableStringTable();
		stringTable->SetNamespace(FTextKey(ns._name));
		stringTable->ClearSourceStrings();

		for (FLocEntry const& entry : ns._entries)
		{
			FString const* nativeText = entry._cultureToText.Find(nativeCulture);
			if (not nativeText)
			{
				continue;
			}
			stringTable->SetSourceString(FTextKey(entry._key), *nativeText);
		}

		if (stringTableAsset->MarkPackageDirty())
		{
			FAssetRegistryModule::AssetCreated(stringTableAsset);
			FString const packageFileName(FPackageName::LongPackageNameToFilename(package->GetName(), FPackageName::GetAssetPackageExtension()));
			if (SavePackageHelper(package, packageFileName))
			{
				UE_LOG(LogTemp, Log, TEXT("Successfully saved StringTable: %s"), *assetName);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to save StringTable: %s"), *assetName);
			}
		}
	}

	// culture별 .locres 생성
	for (FString const& culture : cultures)
	{
		FTextLocalizationResource resource;

		for (FLocNamespace const& ns : namespaces)
		{
			for (FLocEntry const& entry : ns._entries)
			{
				FString const* nativeTextPtr = entry._cultureToText.Find(nativeCulture);
				FString const* localizedTextPtr = entry._cultureToText.Find(culture);
				FString const& nativeText = nativeTextPtr ? *nativeTextPtr : entry._key;
				FString const& localizedText = localizedTextPtr ? *localizedTextPtr : nativeText;

				uint32 const sourceHash = FTextLocalizationResource::HashString(nativeText);
				resource.AddEntry(FTextKey(ns._name), FTextKey(entry._key), sourceHash, localizedText, 0);
			}
		}

		FString const locResDir(FPaths::ProjectContentDir() / TEXT("Localization/Game/") / culture);
		IFileManager::Get().MakeDirectory(*locResDir, true);
		FString const locResPath(locResDir / TEXT("Game.locres"));

		if (resource.SaveToFile(locResPath))
		{
			UE_LOG(LogTemp, Log, TEXT("Successfully saved .locres for culture: %s → %s"), *culture, *locResPath);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save .locres for culture: %s"), *culture);
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Check Log 3s(close soon)"));
	FPlatformProcess::Sleep(3.0f);
	return 0;
}

#endif // WITH_EDITOR
