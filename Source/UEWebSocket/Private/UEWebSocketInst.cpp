#include "UEWebSocketPrivatePCH.h"



UUEWebSocketInst::UUEWebSocketInst(const class FObjectInitializer& PCIP)
    : Super(PCIP)
{


}

void UUEWebSocketInst::Init(UObject* WorldContextObject)
{

}


void UUEWebSocketInst::Send(const FString& data)
{
	ReceiveEventEmitter.Broadcast(data);
}

void UUEWebSocketInst::Tick(float DeltaTime)
{
	static float tmp = 0;
	tmp += DeltaTime;
	GEngine->AddOnScreenDebugMessage(0, 0, FColor::Green, FString::SanitizeFloat(tmp));

}

bool UUEWebSocketInst::IsTickable() const
{
	return true;
}

bool UUEWebSocketInst::IsTickableInEditor() const
{
	return false;
}

bool UUEWebSocketInst::IsTickableWhenPaused() const
{
	return false;
}

TStatId UUEWebSocketInst::GetStatId() const
{
	return TStatId();
}

UWorld* UUEWebSocketInst::GetWorld() const
{
	return GetOuter()->GetWorld();
}