/**
 * @file socket.h
 * @brief Functions for opening sockets for wireless communication.
 */

#pragma once

#include <sys/socket.h>
#include <nn/types.h>

namespace nn {
namespace socket {
struct InAddr {
    u32 addr;
};

Result Initialize(void* pool, ulong poolSize, ulong allocPoolSize, int concurLimit);
Result Finalize();
s32 SetSockOpt(s32 socket, s32 socketLevel, s32 option, void const*, u64 len);
u64 Send(s32 socket, void const* buffer, u64 bufferLength, s32 flags);
s32 Socket(s32 domain, s32 type, s32 proto);
u16 InetHtons(u16);
u32 InetAton(const char* str, InAddr*);
u32 Connect(s32 socket, const sockaddr* addr, u32 addrLen);
};  // namespace socket
};  // namespace nn