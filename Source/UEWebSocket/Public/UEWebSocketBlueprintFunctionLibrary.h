#pragma once


#include "UEWebSocketBlueprintFunctionLibrary.generated.h"

UCLASS(ClassGroup = UEWebSocket, Blueprintable)
class UUEWebSocketBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{

    GENERATED_UCLASS_BODY()

    UFUNCTION(BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", DisplayName = "Create UEWebSocketInst", CompactNodeTitle = "UEWebSocketInst", Keywords = "new create mars UEWebSocket inst"), Category = UEWebSocket)
        static UUEWebSocketInst* NewUEWebSocketInst(UObject* WorldContextObject);



};

