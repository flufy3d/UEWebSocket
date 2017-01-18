#include "UEWebSocketPrivatePCH.h"

#include "WebSocket.h"

UWebSocketWrap::UWebSocketWrap(const class FObjectInitializer& PCIP)
	: Super(PCIP), websocket(nullptr)
{


}
void UWebSocketWrap::OnPacketRecieved(void* Data, int32 Count)
{
	FString _tmp = FString(Count,ANSI_TO_TCHAR((char*)Data));
	PacketRecievedCallBack.Broadcast(_tmp);
	
}

void UWebSocketWrap::OnPacketRecievedCPP(void* Data, int32 Count)
{
	FString _tmp = FString(Count, ANSI_TO_TCHAR((char*)Data));
	PacketRecievedCallBackCPP.ExecuteIfBound(_tmp);

}

void UWebSocketWrap::OnConnected()
{
	ConnectedCallBack.Broadcast();
}

void UWebSocketWrap::OnError()
{
	ErrorCallBack.Broadcast();
}

void UWebSocketWrap::Init(UObject* WorldContextObject)
{
	websocket = new FMyWebSocket(URL,Port);

	FWebsocketPacketRecievedCallBack CallBack;
	CallBack.BindUObject(this, &UWebSocketWrap::OnPacketRecieved);
	websocket->SetRecieveCallBack(CallBack);

	FWebsocketInfoCallBack  connectedCallBack;
	connectedCallBack.BindUObject(this, &UWebSocketWrap::OnConnected);
	websocket->SetConnectedCallBack(connectedCallBack);

	FWebsocketInfoCallBack  errorCallBack;
	errorCallBack.BindUObject(this, &UWebSocketWrap::OnError);
	websocket->SetErrorCallBack(errorCallBack);

}
void UWebSocketWrap::Init()
{
	websocket = new FMyWebSocket(URL, Port);

	FWebsocketPacketRecievedCallBack CallBack;
	CallBack.BindUObject(this, &UWebSocketWrap::OnPacketRecievedCPP);
	websocket->SetRecieveCallBack(CallBack);

	FWebsocketInfoCallBack  connectedCallBack;
	connectedCallBack.BindUObject(this, &UWebSocketWrap::OnConnected);
	websocket->SetConnectedCallBack(connectedCallBack);

	FWebsocketInfoCallBack  errorCallBack;
	errorCallBack.BindUObject(this, &UWebSocketWrap::OnError);
	websocket->SetErrorCallBack(errorCallBack);
}


void UWebSocketWrap::Send(const FString& data)
{
	if (websocket) websocket->Send((uint8*)TCHAR_TO_ANSI(*data), data.Len());
}

void UWebSocketWrap::FinishDestroy()
{
	Super::FinishDestroy();
	delete websocket;
	websocket = nullptr;
}
void UWebSocketWrap::Tick(float DeltaTime)
{
	if (websocket) websocket->Tick();
}

bool UWebSocketWrap::IsTickable() const
{
	return true;
}

bool UWebSocketWrap::IsTickableInEditor() const
{
	return false;
}

bool UWebSocketWrap::IsTickableWhenPaused() const
{
	return false;
}

TStatId UWebSocketWrap::GetStatId() const
{
	return TStatId();
}

UWorld* UWebSocketWrap::GetWorld() const
{
	return GetOuter()->GetWorld();
}