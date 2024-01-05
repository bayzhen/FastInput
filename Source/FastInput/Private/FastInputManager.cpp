#include "FastInputManager.h"
#include "Kismet/GameplayStatics.h"

UFastInputManager* UFastInputManager::self = nullptr;

UFastInputManager::UFastInputManager()
{
	self = this;
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
	TSharedPtr<SEditableText> EditableTextSharedPtr = StaticCastSharedPtr<SEditableText>(WidgetSharedPtr);
	EditableText = EditableTextSharedPtr.Get();
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
		auto PropertyName = Property->GetName();
		auto PropertyNameCPP = Property->GetNameCPP();
		auto PropertyClassName = Property->GetClass()->GetName();
		UClass* PropertyOwnerClass = Property->GetOwnerClass();
		UStruct* PropertyOwnerStruct = Property->GetOwnerStruct();
		if (PropertyOwnerClass) {
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("markov GetAllPropertiesNameAndClass Property GetName: %s \t GetNameCPP: %s \t GetClass->GetName: %s \t GetOwnerClass->GetName: %s"),
				*PropertyName,
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
				*PropertyName,
				*PropertyNameCPP,
				*PropertyClassName,
				*PropertyOwnerStruct->GetName()
			);
		}

	}

	if (PropertyHandlePtrArray.Num()) {
		TSharedPtr<IPropertyHandle> PropertyHandlePtr = PropertyHandlePtrArray[0];
		FProperty* Property = PropertyHandlePtr->GetProperty();
		FString PropertyName = Property->GetName();
		UClass* PropertyOwnerClass = Property->GetOwnerClass();
		UStruct* PropertyOwnerStruct = Property->GetOwnerStruct();
		OnFastInputDetect.Broadcast(PropertyName, PropertyOwnerClass, PropertyOwnerStruct, GetSelectedActorClass());
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
		FString PropertyName = Property->GetName();
		PropertyNamesArray.Add(PropertyName);
	}
	return PropertyNamesArray;
}

FString UFastInputManager::FIGetJsonPath(FString PropertyName, UClass* Class, UStruct* Struct)
{
	FString FilePath(FPaths::ProjectContentDir() + "FastInput/");
	FString FileName(PropertyName + " ");
	if (Class) {
		FileName += Class->GetName() + ".json";
	}
	if (Struct) {
		FileName += Struct->GetName() + ".json";
	}
	return FilePath + FileName;
}

TArray<FString> UFastInputManager::GetAllSelections(FString PropertyName, UClass* Class, UStruct* Struct, UClass* ObjectClass)
{
	TArray<FString> SelectionsArray;
	TSharedPtr<FJsonObject> PropertyFastInputJsonSPtr = FIReadJson(FIGetJsonPath(PropertyName, Class, Struct));
	if (!PropertyFastInputJsonSPtr) return SelectionsArray;
	FString DTRef = PropertyFastInputJsonSPtr->GetStringField("DTRef");
	UDataTable* DT = nullptr;
	ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("DataTable'/Game/Path/To/Your/DataTable.DataTable'"));
	if (DataTableAsset.Object != NULL)
	{
		DT = DataTableAsset.Object;
	}
	if (DT) {
		for (auto& RowName : DT->GetRowNames()) {
			SelectionsArray.Add(RowName.ToString());
		}
	}
	return SelectionsArray;
}

TSharedPtr<FJsonObject> UFastInputManager::FIReadJson(const FString& FilePath)
{
	FString Content;
	if (FFileHelper::LoadFileToString(Content, *FilePath))
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Content);

		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			return JsonObject;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON file: %s"), *FilePath);
			return nullptr;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file: %s"), *FilePath);
		return nullptr;
	}
}

TSharedPtr<FJsonObject> UFastInputManager::FIMakeJson(UDataTable* DT)
{
	FString DTRef = DT->GetPathName();
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("DTRef"), DTRef);
	return JsonObject;
}

bool UFastInputManager::FISaveJson(FString PropertyName, UClass* Class, UStruct* Struct, UClass* ObjectClass, UDataTable* DT)
{
	FString FilePath = FIGetJsonPath(PropertyName, Class, Struct);
	TSharedPtr<FJsonObject> JsonObject = FIMakeJson(DT);

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
