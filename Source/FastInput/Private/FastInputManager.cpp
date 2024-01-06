#include "FastInputManager.h"
#include "Kismet/GameplayStatics.h"
#include "EditorUtilityWidget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "Editor.h"

UFastInputManager* UFastInputManager::self = nullptr;

UFastInputManager::UFastInputManager()
{
	self = this;
	EditableTextSharedPtr = nullptr;
}

UFastInputManager* UFastInputManager::Get()
{
	return self;
}

void UFastInputManager::GetSDetailSingleItemRow(FSlateApplication& SlateApp, TSharedPtr<SDetailSingleItemRow>& OutDetailSingleItemRow)
{
	TSharedPtr<SWidget> WidgetSharedPtr = SlateApp.GetUserFocusedWidget(0);
	if (!WidgetSharedPtr.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("markov GetSDetailSingleItemRow Widget !IsValid"));
		return;
	}
	// RTTI? need to learn that.
	// StaticCastSharedPtr will always output a valid ptr. which is misleading.
	if (!WidgetSharedPtr->GetTypeAsString().Equals("SEditableText")) {
		UE_LOG(LogTemp, Warning, TEXT("markov GetSDetailSingleItemRow !WidgetPtr->GetTypeAsString().Equals(\"SEditableText\")"));
		return;
	}
	int depth = 20;
	TSharedPtr<SWidget> TempWidgetSharedPtr;

	while (depth-- > 0) {
		TempWidgetSharedPtr = WidgetSharedPtr->GetParentWidget();
		if (!TempWidgetSharedPtr.IsValid()) {
			UE_LOG(LogTemp, Warning, TEXT("markov GetSDetailSingleItemRow !TempWidgetSharedPtr.IsValid()"));
			return;
		}
		if (TempWidgetSharedPtr->GetTypeAsString().Equals("SDetailSingleItemRow")) {
			TSharedPtr<SDetailSingleItemRow> DetailSingleItemRow = StaticCastSharedPtr<SDetailSingleItemRow>(TempWidgetSharedPtr);
			OutDetailSingleItemRow = DetailSingleItemRow;
			EditableTextSharedPtr = StaticCastSharedPtr<SEditableText>(WidgetSharedPtr);
			UE_LOG(LogTemp, Warning, TEXT("markov GetSDetailSingleItemRow OutDetailSingleItemRow found"));
			return;
		}
		WidgetSharedPtr = TempWidgetSharedPtr;
	}
	UE_LOG(LogTemp, Warning, TEXT("markov GetSDetailSingleItemRow OutDetailSingleItemRow not found"));
}

void UFastInputManager::GetAllPropertiesNameAndClass(TSharedPtr<SDetailSingleItemRow>& InDetailSingleItemRowPtr)
{
	if (!InDetailSingleItemRowPtr.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("markov GetAllPropertiesNameAndClass InDetailSingleItemRow !IsValid"));
		return;
	}

	TArray<TSharedPtr<IPropertyHandle>> PropertyHandlePtrArray = InDetailSingleItemRowPtr->GetPropertyHandles();

	for (auto& PropertyHandlePtr : PropertyHandlePtrArray) {
		FProperty* Property = PropertyHandlePtr->GetProperty();
		auto PropertyNameT = Property->GetName();
		auto PropertyNameCPP = Property->GetNameCPP();
		auto PropertyClassName = Property->GetClass()->GetName();
		PropertyOwnerClass = Property->GetOwnerClass();
		PropertyOwnerStruct = Property->GetOwnerStruct();
		if (PropertyOwnerClass) {
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("markov GetAllPropertiesNameAndClass Property GetName: %s \t GetNameCPP: %s \t GetClass->GetName: %s \t GetOwnerClass->GetName: %s"),
				*PropertyNameT,
				*PropertyNameCPP,
				*PropertyClassName,
				*PropertyOwnerClass->GetName()
			);
		}
		if (PropertyOwnerStruct) {
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("markov GetAllPropertiesNameAndClass Property GetName: %s \t GetNameCPP: %s \t GetStruct->GetName: %s \t GetOwnerStruct->GetName: %s"),
				*PropertyNameT,
				*PropertyNameCPP,
				*PropertyClassName,
				*PropertyOwnerStruct->GetName()
			);
		}

	}

	if (PropertyHandlePtrArray.Num()) {
		TSharedPtr<IPropertyHandle> PropertyHandlePtr = PropertyHandlePtrArray[0];
		FProperty* Property = PropertyHandlePtr->GetProperty();
		PropertyName = Property->GetName();
		PropertyOwnerClass = Property->GetOwnerClass();
		PropertyOwnerStruct = Property->GetOwnerStruct();
		PropertyActorClass = GetSelectedActorClass();
		this->TriggerEUWEvent("Update");
	}
}

UClass* UFastInputManager::GetSelectedActorClass()
{
	TArray<AActor*> SelectedActors;

	// Access the editor's selected actors.
	if (GEditor)
	{
		USelection* SelectedActorsList = GEditor->GetSelectedActors();
		if (SelectedActorsList)
		{
			for (FSelectionIterator It(*SelectedActorsList); It; ++It)
			{
				AActor* SelectedActor = Cast<AActor>(*It);
				if (SelectedActor)
				{
					SelectedActors.Add(SelectedActor);
				}
			}
		}
	}
	UClass* SelectedActorClass = nullptr;
	if (SelectedActors.Num()) {
		SelectedActorClass = SelectedActors[0]->GetClass();
	}
	return SelectedActorClass;
}

void UFastInputManager::SetEditableText(FString InputString)
{
	if (EditableTextSharedPtr.IsValid()) {
		EditableTextSharedPtr->SetText(FText::FromString(InputString));
	}
}

FString UFastInputManager::GetUClassName(UClass* PropertyOwnerClass)
{
	FString Name("");
	if (PropertyOwnerClass)
		return PropertyOwnerClass->GetName();
	return Name;
}

FString UFastInputManager::GetUStructName(UStruct* PropertyOwnerStruct)
{
	FString Name("");
	if (PropertyOwnerStruct)
		return PropertyOwnerStruct->GetName();
	return Name;
}

void UFastInputManager::TriggerEUWEvent(FString EventName)
{
	FStringAssetReference WidgetAssetPath(TEXT("/FastInput/EUW_FastInput.EUW_FastInput"));
	UEditorUtilityWidgetBlueprint* WidgetBlueprint = Cast<UEditorUtilityWidgetBlueprint>(WidgetAssetPath.TryLoad());
	if (WidgetBlueprint) {
		UEditorUtilityWidget* FastInputWidget = WidgetBlueprint->GetCreatedWidget();
		if (FastInputWidget) {
			UFunction* TriggerHelloFunction = FastInputWidget->FindFunction(*EventName);
			if (TriggerHelloFunction) {
				FastInputWidget->ProcessEvent(TriggerHelloFunction, nullptr);
			}
		}
	}
}

TArray<FString> UFastInputManager::GetAllPropertyNames(UClass* InClass)
{
	TArray<FString> PropertyNamesArray;
	if (!InClass)
	{
		UE_LOG(LogTemp, Error, TEXT("TargetClass is nullptr."));
		return PropertyNamesArray;
	}

	for (TFieldIterator<FProperty> PropertyIterator(InClass); PropertyIterator; ++PropertyIterator)
	{
		FProperty* Property = *PropertyIterator;
		PropertyNamesArray.Add(Property->GetName());
	}
	return PropertyNamesArray;
}

FString UFastInputManager::FIGetJsonPath()
{
	FString FilePath(FPaths::ProjectContentDir() + "FastInput/");
	FString FileName("");
	if (PropertyOwnerStruct) {
		FileName = PropertyOwnerStruct->GetName() + " " + PropertyName + ".json";
	}
	else {
		return FString("");
	}
	return FilePath + FileName;
}

TArray<FString> UFastInputManager::GetAllSelections()
{
	TArray<FString> Result;

	// Retrieve the data table from the reference
	UDataTable* DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DTRef));

	if (DataTable)
	{
		// Get the row map from the data table
		const TMap<FName, uint8*>& RowMap = DataTable->GetRowMap();

		for (const auto& RowPair : RowMap)
		{
			// Get the row struct
			const UScriptStruct* RowStruct = DataTable->GetRowStruct();
			uint8* DataRow = RowPair.Value;

			if (RowStruct)
			{
				// Get the property from the struct using the specified column name
				FProperty* Property = RowStruct->FindPropertyByName(*ColumnName);

				if (Property && Property->IsA<FStrProperty>())
				{
					FStrProperty* StringProperty = Cast<FStrProperty>(Property);
					FString Value = StringProperty->GetPropertyValue_InContainer(DataRow);

					// Add the FString value to the result array
					Result.Add(Value);
				}
			}
		}
	}

	return Result;
}


TSharedPtr<FJsonObject> UFastInputManager::FIReadJson()
{
	FString FilePath = FIGetJsonPath();
	FString Content;
	if (FFileHelper::LoadFileToString(Content, *FilePath))
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Content);

		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			DTRef = JsonObject->GetStringField("DTRef");
			ColumnName = JsonObject->GetStringField("ColumnName");
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON file: %s"), *FilePath);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file: %s"), *FilePath);
	}
}

TSharedPtr<FJsonObject> UFastInputManager::FIMakeJson()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("DTRef", DTRef);
	JsonObject->SetStringField("ColumnName", ColumnName);
	return JsonObject;
}

bool UFastInputManager::FISaveJson()
{
	FString FilePath = FIGetJsonPath();
	TSharedPtr<FJsonObject> JsonObject = FIMakeJson();

	if (JsonObject.IsValid())
	{
		FString Content;
		TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&Content);

		if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter))
		{
			// Get the file directory
			FString FileDirectory = FPaths::GetPath(FilePath);

			// Check if the directory exists, and create it if not
			if (!FPaths::DirectoryExists(FileDirectory))
			{
				IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
				PlatformFile.CreateDirectoryTree(*FileDirectory);
			}

			if (FFileHelper::SaveStringToFile(Content, *FilePath))
			{
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to save JSON into file: %s"), *FilePath);
				return false;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to serialize JSON object"));
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid JSON object"));
		return false;
	}
}
