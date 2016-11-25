#include "UEWebSocketPrivatePCH.h"


UUEWebSocketBlueprintFunctionLibrary::UUEWebSocketBlueprintFunctionLibrary(const class FObjectInitializer& PCIP)
: Super(PCIP)
{

}

UUEWebSocketInst* UUEWebSocketBlueprintFunctionLibrary::NewUEWebSocketInst(UObject* WorldContextObject)
{

    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
    UUEWebSocketInst* tempObject = Cast<UUEWebSocketInst>(StaticConstructObject_Internal(UUEWebSocketInst::StaticClass()));

    return tempObject;

}
