#pragma once
#include "WebSocketWrap.generated.h"

class FMyWebSocket;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWebsocketPacketRecievedCallBackBP,const FString&, Data);
DECLARE_DELEGATE_OneParam(FWebsocketPacketRecievedCallBackCPP, const FString&);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWebsocketInfoCallBackBP);


UCLASS(ClassGroup = UEWebSocket, Blueprintable)
class UEWEBSOCKET_API UWebSocketWrap : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

	UWebSocketWrap(const class FObjectInitializer& PCIP);

public:


	// Initialize function, should be called after properties are set 
	UFUNCTION(BlueprintCallable, Category = "UEWebSocket", meta = (WorldContext = "WorldContextObject"))
		void Init(UObject* WorldContextObject);


	//this is call from CPP
		void Init();

	UFUNCTION(BlueprintCallable, Category = "UEWebSocket")
		void Send(const FString& data);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UEWebSocket")
		FString URL;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UEWebSocket")
		int32 Port;

	UPROPERTY(BlueprintAssignable)
		FWebsocketInfoCallBackBP ConnectedCallBack;

	UPROPERTY(BlueprintAssignable)
		FWebsocketInfoCallBackBP ErrorCallBack;

	UPROPERTY(BlueprintAssignable)
		FWebsocketPacketRecievedCallBackBP PacketRecievedCallBack;

	FWebsocketPacketRecievedCallBackCPP PacketRecievedCallBackCPP;



private:
	
	void OnPacketRecieved(void* Data, int32 Count);
	void OnPacketRecievedCPP(void* Data, int32 Count);

	void OnConnected();

	void OnError();

	FMyWebSocket* websocket;

	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	UWorld* GetWorld() const override;
	void FinishDestroy() override;

};
