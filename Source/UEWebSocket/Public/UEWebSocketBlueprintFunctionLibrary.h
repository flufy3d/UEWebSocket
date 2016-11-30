#pragma once


#include "UEWebSocketBlueprintFunctionLibrary.generated.h"

UCLASS(ClassGroup = UEWebSocket, Blueprintable)
class UUEWebSocketBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{

    GENERATED_UCLASS_BODY()

    UFUNCTION(BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", DisplayName = "Create WebSocket", CompactNodeTitle = "WebSocket", Keywords = "new create WebSocket"), Category = UEWebSocket)
        static UWebSocketWrap* NewWebSocket(UObject* WorldContextObject);



};

