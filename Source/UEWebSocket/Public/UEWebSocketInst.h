#pragma once
#include "UEWebSocketInst.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceiveEvent, const FString&, data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectedEvent, const FString&, data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCloseEvent, const FString&, data);

DECLARE_DELEGATE_OneParam(FReceiveCPPEvent,const FString&);

UCLASS(ClassGroup = UEWebSocket, Blueprintable)
class UEWEBSOCKET_API UUEWebSocketInst : public UObject , public FTickableGameObject
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

	UPROPERTY(BlueprintAssignable)
		FReceiveEvent ReceiveEventEmitter;

	UPROPERTY(BlueprintAssignable)
		FReceiveEvent FConnectedEvent;

	UPROPERTY(BlueprintAssignable)
		FReceiveEvent FCloseEvent;


	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;

	UWorld* GetWorld() const override;

};
