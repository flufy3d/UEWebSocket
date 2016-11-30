#include "UEWebSocketPrivatePCH.h"


UUEWebSocketBlueprintFunctionLibrary::UUEWebSocketBlueprintFunctionLibrary(const class FObjectInitializer& PCIP)
: Super(PCIP)
{

}

UWebSocketWrap* UUEWebSocketBlueprintFunctionLibrary::NewWebSocket(UObject* WorldContextObject)
{

    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
    UWebSocketWrap* tempObject = Cast<UWebSocketWrap>(StaticConstructObject_Internal(UWebSocketWrap::StaticClass()));

    return tempObject;

}
