/* Copyright (c) Microsoft Corporation. All rights reserved.
   Licensed under the MIT License. */

#pragma once
#include <sys/types.h>
#include <stdbool.h>

#include "message_protocol_public.h"
#include "exitcodes.h"

typedef ssize_t (*Transport_ReadFunctionType)(char *buffer, size_t amount);

typedef ssize_t (*Transport_WriteFunctionType)(const char *buffer, size_t amount);

/// <summary>
///     Initialize the message protocol and UART.
/// </summary>
/// <param name="epollFd">epoll file descriptor to use for event polling.</param>
/// <param name="readFunction">Function to call to read data from the transport.</param>
/// <param name="writeFunction">Function to call to write data to the transport.</param>
/// <returns>An <see cref="ExitCode" /> indicating success or failure.</returns>
ExitCode MessageProtocol_Initialize(EventLoop *el, Transport_ReadFunctionType readFunction,
                                    Transport_WriteFunctionType writeFunction);

/// <summary>
///     Clean up the message protocol callback handlers.
/// </summary>
void MessageProtocol_Cleanup(void);

/// <summary>
///     Call this function to cause the message protocol to attempt to read a message from the
///     transport (and call the relevant handlers)
/// </summary>
void MessageProtocol_HandleReceivedMessage(void);

typedef void (*MessageProtocol_EventHandlerType)(MessageProtocol_CategoryId categoryId,
                                                 MessageProtocol_EventId eventId);

/// <summary>
///     Register a callback handler for incoming message protocol event messages.
/// </summary>
/// <param name="categoryId">The message protocol category ID.</param>
/// <param name="eventId">The message protocol event ID.</param>
/// <param name="handler">The callback handler to register.</param>
void MessageProtocol_RegisterEventHandler(MessageProtocol_CategoryId categoryId,
                                          MessageProtocol_EventId eventId,
                                          MessageProtocol_EventHandlerType handler);

typedef void (*MessageProtocol_IdleHandlerType)(void);

/// <summary>
///     Register a callback handler for the idle event.
/// </summary>
/// <param name="handler">The callback handler to register.</param>
void MessageProtocol_RegisterIdleHandler(MessageProtocol_IdleHandlerType handler);

typedef void (*MessageProtocol_ResponseHandlerType)(MessageProtocol_CategoryId categoryId,
                                                    MessageProtocol_RequestId requestId,
                                                    const uint8_t *data, size_t dataSize,
                                                    MessageProtocol_ResponseResult result,
                                                    bool timedOut);

/// <summary>
///     Send a request using the message protocol.
/// </summary>
/// <param name="categoryId">The message protocol category ID.</param>
/// <param name="requestId">The message protocol request ID.</param>
/// <param name="body">The body of the message.</param>
/// <param name="bodyLength">The length of the message body in bytes.</param>
/// <param name="responseHandler">The callback handler for the response message.</param>
void MessageProtocol_SendRequest(MessageProtocol_CategoryId categoryId,
                                 MessageProtocol_RequestId requestId, const uint8_t *body,
                                 size_t bodyLength,
                                 MessageProtocol_ResponseHandlerType responseHandler);

/// <summary>
///     Query whether the message protocol is currently idle.
/// </summary>
/// <returns>True if the message protocol is currently idle; false if it is busy.</returns>
bool MessageProtocol_IsIdle(void);
