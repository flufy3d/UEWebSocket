// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#include "UEWebSocketPrivatePCH.h"
#include "WebSocket.h"


#if PLATFORM_WINDOWS
#include "AllowWindowsPlatformTypes.h"
#endif



#include "libwebsockets.h"
#include "private-libwebsockets.h"


#if PLATFORM_WINDOWS
#include "HideWindowsPlatformTypes.h"
#endif



uint8 PREPADDING[LWS_SEND_BUFFER_PRE_PADDING];
uint8 POSTPADDING[LWS_SEND_BUFFER_POST_PADDING];



static void libwebsocket_debugLogS(int level, const char *line)
{
	UE_LOG(LogUEWebSocket, Log, TEXT("client: %s"), ANSI_TO_TCHAR(line));
}


FWebSocket::FWebSocket(const FString& url, int port)
:IsServerSide(false)
{



#if !UE_BUILD_SHIPPING
	lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_DEBUG | LLL_INFO, libwebsocket_debugLogS);
#endif 

	Protocols = new libwebsocket_protocols[3];
	FMemory::Memzero(Protocols, sizeof(libwebsocket_protocols) * 3);

	Protocols[0].name = "binary";
	Protocols[0].callback = FWebSocket::unreal_networking_client;
	Protocols[0].per_session_data_size = 0;
	Protocols[0].rx_buffer_size = 10 * 1024 * 1024;

	Protocols[1].name = nullptr;
	Protocols[1].callback = nullptr;
	Protocols[1].per_session_data_size = 0;

	struct lws_context_creation_info Info;
	memset(&Info, 0, sizeof Info);

	Info.port = CONTEXT_PORT_NO_LISTEN;
	Info.protocols = &Protocols[0];
	Info.gid = -1;
	Info.uid = -1;
	Info.user = this;

	Context = libwebsocket_create_context(&Info);

	check(Context); 

	Wsi = libwebsocket_client_connect_extended
							(Context, 
							TCHAR_TO_ANSI(*url),
							port, 
							false, "/", TCHAR_TO_ANSI(*url), TCHAR_TO_ANSI(*url), Protocols[1].name, -1,this);

	check(Wsi);



}

FWebSocket::FWebSocket(WebSocketInternalContext* InContext, WebSocketInternal* InWsi)
	: Context(InContext)
	, Wsi(InWsi)
	, IsServerSide(true)
	, Protocols(nullptr)
{
}


bool FWebSocket::Send(uint8* Data, uint32 Size)
{
	TArray<uint8> Buffer;
	// insert size. 


	//Buffer.Append((uint8*)&PREPADDING, sizeof(PREPADDING));

	//this is for continue get data.the data is big ,so need write length in first int32.
	//Buffer.Append((uint8*)&Size, sizeof (uint32));

	Buffer.Append((uint8*)Data, Size);


	//Buffer.Append((uint8*)&POSTPADDING, sizeof(POSTPADDING));


	OutgoingBuffer.Add(Buffer);

	return true;
}

void FWebSocket::SetRecieveCallBack(FWebsocketPacketRecievedCallBack CallBack)
{
	RecievedCallBack = CallBack; 
}

FString FWebSocket::RemoteEndPoint()
{
	ANSICHAR Peer_Name[128];
	ANSICHAR Peer_Ip[128];
	libwebsockets_get_peer_addresses(Context, Wsi, libwebsocket_get_socket_fd(Wsi), Peer_Name, sizeof Peer_Name, Peer_Ip, sizeof Peer_Ip);
	return FString(Peer_Name);

}


FString FWebSocket::LocalEndPoint()
{
	return FString(ANSI_TO_TCHAR(libwebsocket_canonical_hostname(Context)));
}

void FWebSocket::Tick()
{
	HandlePacket();
}

void FWebSocket::HandlePacket()
{

	{
		libwebsocket_service(Context, 0);
		if (!IsServerSide)
			libwebsocket_callback_on_writable_all_protocol(&Protocols[0]);
	}


}

void FWebSocket::Flush()
{
	auto PendingMesssages = OutgoingBuffer.Num();
	while (OutgoingBuffer.Num() > 0 && !IsServerSide)
	{

		if (Protocols)
			libwebsocket_callback_on_writable_all_protocol(&Protocols[0]);
		else
			libwebsocket_callback_on_writable(Context, Wsi);

		HandlePacket();
		if (PendingMesssages >= OutgoingBuffer.Num()) 
		{
			UE_LOG(LogUEWebSocket, Warning, TEXT("Unable to flush all of OutgoingBuffer in FWebSocket."));
			break;
		}
	};
}

void FWebSocket::SetConnectedCallBack(FWebsocketInfoCallBack CallBack)
{
	ConnectedCallBack = CallBack; 
}

void FWebSocket::SetErrorCallBack(FWebsocketInfoCallBack CallBack)
{
	ErrorCallBack = CallBack; 
}

void FWebSocket::OnRawRecieve(void* Data, uint32 Size)
{
	RecievedCallBack.ExecuteIfBound(Data, Size);
	/* this is for continue get data.the data is big ,so need write length in first int32.

		RecievedBuffer.Append((uint8*)Data, Size); // consumes all of Data
		while (RecievedBuffer.Num())
		{
			uint32 BytesToBeRead = Size;
			if (BytesToBeRead <= ((uint32)RecievedBuffer.Num() - sizeof(uint32)))
			{
				RecievedCallBack.ExecuteIfBound((void*)((uint8*)RecievedBuffer.GetData() + sizeof(uint32)), BytesToBeRead);
				RecievedBuffer.RemoveAt(0, sizeof(uint32) + BytesToBeRead );
			}
			else
			{
				break;
			}
		}
	*/

}

void FWebSocket::OnRawWebSocketWritable(WebSocketInternal* wsi)
{
	if (OutgoingBuffer.Num() == 0)
		return;

	TArray <uint8>& Packet = OutgoingBuffer[0];



	uint32 TotalDataSize = Packet.Num();
	uint32 DataToSend = TotalDataSize;
	while (DataToSend)
	{

		int Sent = libwebsocket_write(Wsi, Packet.GetData() + (DataToSend-TotalDataSize), DataToSend, (libwebsocket_write_protocol)LWS_WRITE_TEXT);
		if (Sent < 0)
		{
			ErrorCallBack.ExecuteIfBound();
			return;
		}
		if ((uint32)Sent < DataToSend)
		{
			UE_LOG(LogUEWebSocket, Warning, TEXT("Could not write all '%d' bytes to socket"), DataToSend);
		}
		DataToSend-=Sent;
	}

	check(Wsi == wsi);

	// this is very inefficient we need a constant size circular buffer to efficiently not do unnecessary allocations/deallocations. 
	OutgoingBuffer.RemoveAt(0);

}

FWebSocket::~FWebSocket()
{
	RecievedCallBack.Unbind();
	Flush();
	if ( !IsServerSide)
	{
		libwebsocket_context_destroy(Context);
		Context = NULL;
		delete Protocols;
		Protocols = NULL;
	}
}

int FWebSocket::unreal_networking_client(
		struct libwebsocket_context *Context, 
		struct libwebsocket *Wsi, 
		enum libwebsocket_callback_reasons Reason, 
		void *User, 
		void *In, 
		size_t Len)
{
	FWebSocket* Socket = (FWebSocket*)libwebsocket_context_user(Context);;
	switch (Reason)
	{
	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		{
			Socket->ConnectedCallBack.ExecuteIfBound();
			libwebsocket_set_timeout(Wsi, NO_PENDING_TIMEOUT, 0);
			check(Socket->Wsi == Wsi);
		}
		break;
	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		{
			Socket->ErrorCallBack.ExecuteIfBound();
			return -1;
		}
		break;
	case LWS_CALLBACK_CLIENT_RECEIVE:
		{
			// push it on the socket. 
			Socket->OnRawRecieve(In, (uint32)Len); 
			check(Socket->Wsi == Wsi);
			libwebsocket_set_timeout(Wsi, NO_PENDING_TIMEOUT, 0);
			break;
		}
	case LWS_CALLBACK_CLIENT_WRITEABLE:
		{
			check(Socket->Wsi == Wsi);
			Socket->OnRawWebSocketWritable(Wsi); 
			libwebsocket_callback_on_writable(Context, Wsi);
			libwebsocket_set_timeout(Wsi, NO_PENDING_TIMEOUT, 0);
			break; 
		}
	case LWS_CALLBACK_CLOSED:
		{
			Socket->ErrorCallBack.ExecuteIfBound();
			return -1;
		}
	}

	return 0; 
}


