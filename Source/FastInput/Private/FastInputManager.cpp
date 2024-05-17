#include "FastInputManager.h"

UFastInputManager* UFastInputManager::FastInputManagerCppInstance = nullptr;
UObject* UFastInputManager::FastInputManagerBPInstance = nullptr;
TWeakPtr<SWindow> UFastInputManager::EditorWidgetWindow = nullptr;

UFastInputManager::UFastInputManager()
{
	EditableTextSharedPtr = nullptr;
}

UFastInputManager* UFastInputManager::GetCppInstance()
{
	return FastInputManagerCppInstance;
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
	TSharedPtr<SWidget> TempWidgetSharedPtr = WidgetSharedPtr;

	while (depth-- > 0) {
		TempWidgetSharedPtr = TempWidgetSharedPtr->GetParentWidget();
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
	}
	UE_LOG(LogTemp, Warning, TEXT("markov GetSDetailSingleItemRow OutDetailSingleItemRow not found"));
}

void UFastInputManager::GetAllPropertiesNameAndClass(TSharedPtr<SDetailSingleItemRow>& InDetailSingleItemRowPtr)
{
	if (!InDetailSingleItemRowPtr.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("markov GetAllPropertiesNameAndClass InDetailSingleItemRow !IsValid"));
		return;
	}

	TArray<TSharedPtr<IPropertyHandle>> PropertyHandlePtrArray = InDetailSingleItemRowPtr->WidgetRow.GetPropertyHandles();

	/*for (auto& PropertyHandlePtr : PropertyHandlePtrArray) {
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

	}*/

	if (PropertyHandlePtrArray.Num()) {
		TSharedPtr<IPropertyHandle> PropertyHandlePtr = PropertyHandlePtrArray[0];
		PropertyPtr = PropertyHandlePtr->GetProperty();
		PropertyName = PropertyPtr->GetName();
		PropertyOwnerClass = PropertyPtr->GetOwnerClass();
		PropertyOwnerStruct = PropertyPtr->GetOwnerStruct();
		PropertyOwnerStructName = PropertyOwnerStruct->GetName();
		PropertyActorClass = GetSelectedActorClass();
		this->FIReadJson();
		this->GetSelections();
		this->UpdateCustomSelections();
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
	if (EditableTextSharedPtr.IsValid())
	{
		auto Type = EditableTextSharedPtr->GetTypeAsString();
		if (Type.Equals("SEditableText")) {
			EditableTextSharedPtr->SetText(FText::FromString(InputString));
			EditableTextSharedPtr->OnTextCommitted(EditableTextSharedPtr->GetText(), ETextCommit::Default);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("markov EditableTextSharedPtr->GetTypeAsString() %s"), *Type);
		}
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

void UFastInputManager::UpdateCustomSelections_Implementation()
{
}

void UFastInputManager::RunEditorWidgetWindow()
{
	FStringAssetReference WidgetAssetPath(TEXT("/FastInput/EUW_FastInput.EUW_FastInput"));
	UEditorUtilityWidgetBlueprint* EditorUtilityBlueprint = Cast<UEditorUtilityWidgetBlueprint>(WidgetAssetPath.TryLoad());
	// Get the Editor Utility Subsystem
	UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();

	if (EditorUtilityBlueprint)
	{
		// Run the Editor Utility Blueprint
		EditorUtilitySubsystem->SpawnAndRegisterTab(EditorUtilityBlueprint);
	}
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
	FileName = PropertyOwnerStructName + " " + PropertyName + ".json";
	return FilePath + FileName;
}

void UFastInputManager::GetSelections()
{
	Selections.Empty();

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
				if (ColumnName.Equals("RowName", ESearchCase::IgnoreCase)) {
					Selections.Add(RowPair.Key.ToString());
				}
				else {
					// Get the property from the struct using the specified column name
					FProperty* Property = RowStruct->FindPropertyByName(*ColumnName);

					if (Property && Property->IsA<FStrProperty>())
					{
						FStrProperty* StringProperty = CastField<FStrProperty>(Property);
						FString Value = StringProperty->GetPropertyValue_InContainer(DataRow);

						// Add the FString value to the result array
						Selections.Add(Value);
					}
				}
			}
		}
	}
	else {
		Selections.Empty();
	}
}

void UFastInputManager::FIReadJson()
{
	FString FilePath = FIGetJsonPath();
	FString Content;
	if (FFileHelper::LoadFileToString(Content, *FilePath))
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Content);

		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			if (PropertyActorClass) {
				DTRef = JsonObject->GetObjectField("DTRef")->GetStringField(PropertyActorClass->GetName());
				ColumnName = JsonObject->GetObjectField("ColumnName")->GetStringField(PropertyActorClass->GetName());
			}
			else {
				DTRef = JsonObject->GetObjectField("DTRef")->GetStringField("Default");
				ColumnName = JsonObject->GetObjectField("ColumnName")->GetStringField("Default");
			}
		}
		else
		{
			DTRef.Empty();
			ColumnName.Empty();
			JsonObject = MakeShareable(new FJsonObject());
			UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON file: %s"), *FilePath);
		}
	}
	else
	{
		DTRef.Empty();
		ColumnName.Empty();
		JsonObject = MakeShareable(new FJsonObject());
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file: %s"), *FilePath);
	}
}

TSharedPtr<FJsonObject> UFastInputManager::FIMakeJson()
{
	TSharedPtr<FJsonObject> DTRefJsonObject = MakeShareable(new FJsonObject);
	TSharedPtr<FJsonObject> ColumnNameJsonObject = MakeShareable(new FJsonObject);
	if (JsonObject) {
		DTRefJsonObject = JsonObject->GetObjectField("DTRef");
		ColumnNameJsonObject = JsonObject->GetObjectField("ColumnName");
	}
	if (PropertyActorClass) {
		DTRefJsonObject->SetStringField(PropertyActorClass->GetName(), DTRef);
		ColumnNameJsonObject->SetStringField(PropertyActorClass->GetName(), ColumnName);
		JsonObject->SetObjectField("DTRef", DTRefJsonObject);
		JsonObject->SetObjectField("ColumnName", ColumnNameJsonObject);
	}
	else {
		DTRefJsonObject->SetStringField("Default", DTRef);
		ColumnNameJsonObject->SetStringField("Default", ColumnName);
		JsonObject->SetObjectField("DTRef", DTRefJsonObject);
		JsonObject->SetObjectField("ColumnName", ColumnNameJsonObject);
	}
	return JsonObject;
}

bool UFastInputManager::FISaveJson()
{
	FString FilePath = FIGetJsonPath();
	JsonObject = FIMakeJson();

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
