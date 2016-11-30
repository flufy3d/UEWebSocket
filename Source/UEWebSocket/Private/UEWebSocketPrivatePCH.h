// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "IUEWebSocket.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.

#include "Core.h"
#include "Engine.h"


#include "UEWebSocketBlueprintFunctionLibrary.h"
#include "WebSocketWrap.h"


class FWebSocket;
class FWebSocketServer;

typedef struct libwebsocket_context WebSocketInternalContext;
typedef struct libwebsocket WebSocketInternal;
typedef struct libwebsocket_protocols WebSocketInternalProtocol;

DECLARE_DELEGATE_TwoParams(FWebsocketPacketRecievedCallBack, void* /*Data*/, int32 /*Data Size*/);
DECLARE_DELEGATE_OneParam(FWebsocketClientConnectedCallBack, FWebSocket* /*Socket*/);
DECLARE_DELEGATE(FWebsocketInfoCallBack);

DECLARE_LOG_CATEGORY_EXTERN(LogUEWebSocket, Warning, All);

