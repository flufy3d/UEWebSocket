#pragma once
#include "UEWebSocketInst.generated.h"


UCLASS(ClassGroup = UEWebSocket, Blueprintable)
class UUEWebSocketInst : public UObject
{

    GENERATED_BODY()

    UUEWebSocketInst(const class FObjectInitializer& PCIP);

public:
    //GENERATED_UCLASS_BODY()

    // Initialize function, should be called after properties are set 
    UFUNCTION(BlueprintCallable, Category = "UEWebSocket", meta = (WorldContext = "WorldContextObject"))
        void Init(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "UEWebSocket")
		void Send(const FString& data);



};
