/*********************************************************
 * Copyright (C) 2011,2014-2016 VMware, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation version 2.1 and no later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the Lesser GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA.
 *
 *********************************************************/

/*********************************************************
 * The contents of this file are subject to the terms of the Common
 * Development and Distribution License (the "License") version 1.0
 * and no later version.  You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at
 *         http://www.opensource.org/licenses/cddl1.php
 *
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 *********************************************************/

#ifndef __ASYNC_SOCKET_VTABLE_H__
#define __ASYNC_SOCKET_VTABLE_H__

#ifdef USE_SSL_DIRECT
#include "sslDirect.h"
#else
#include "ssl.h"
#endif

/*
 * If we change the AsyncSocketVTable, we also need to change the follow files:
 * lib/blastSockets/asyncProxySocket.c
 * lib/asyncsocket/asyncsocket.c
 * lib/asyncsocket/asyncWebSocket.c
 * lib/asyncsocket/asyncNamedPipe.c
 * lib/udpfec/fecAsyncSocket.c
 * lib/udpfec/fecAsyncSslSocket.c
 * devices/vsock/asyncVmciSocket.c
 */
typedef struct AsyncSocketVTable {
   AsyncSocketState (*getState)(AsyncSocket *sock);
   int (*getGenericErrno)(AsyncSocket *s);
   int (*getFd)(AsyncSocket *asock);
   int (*getRemoteIPStr)(AsyncSocket *asock, const char **ipStr);
   int (*getINETIPStr)(AsyncSocket *asock, int socketFamily, char **ipRetStr);
   unsigned int (*getPort)(AsyncSocket *asock);
   int (*useNodelay)(AsyncSocket *asock, Bool nodelay);
   int (*setTCPTimeouts)(AsyncSocket *asock, int keepIdle, int keepIntvl,
                         int keepCnt);
   Bool (*setBufferSizes)(AsyncSocket *asock, int sendSz, int recvSz);
   void (*setSendLowLatencyMode)(AsyncSocket *asock, Bool enable);
   void (*setCloseOptions)(AsyncSocket *asock, int flushEnabledMaxWaitMsec,
                           AsyncSocketCloseFn closeCb);
   Bool (*connectSSL)(AsyncSocket *asock, struct _SSLVerifyParam *verifyParam,
                      void *sslContext);
   void (*startSslConnect)(AsyncSocket *asock,
                           struct _SSLVerifyParam *verifyParam, void *sslCtx,
                           AsyncSocketSslConnectFn sslConnectFn,
                           void *clientData);
   Bool (*acceptSSL)(AsyncSocket *asock, void *sslCtx);
   void (*startSslAccept)(AsyncSocket *asock, void *sslCtx,
                          AsyncSocketSslAcceptFn sslAcceptFn,
                          void *clientData);
   int (*flush)(AsyncSocket *asock, int timeoutMS);
   int (*recv)(AsyncSocket *asock, void *buf, int len, Bool partial, void *cb,
               void *cbData);
   int (*recvPassedFd)(AsyncSocket *asock, void *buf, int len, void *cb,
                       void *cbData);
   int (*getReceivedFd)(AsyncSocket *asock);
   int (*send)(AsyncSocket *asock, void *buf, int len,
               AsyncSocketSendFn sendFn, void *clientData);
   int (*isSendBufferFull)(AsyncSocket *asock);
   int (*getNetworkStats)(AsyncSocket *asock,
                          AsyncSocketNetworkStats *stats);
   int (*close)(AsyncSocket *asock);
   int (*cancelRecv)(AsyncSocket *asock, int *partialRecvd, void **recvBuf,
                     void **recvFn, Bool cancelOnSend);
   void (*cancelCbForClose)(AsyncSocket *asock);
   int (*getLocalVMCIAddress)(AsyncSocket *asock, uint32 *cid, uint32 *port);
   int (*getRemoteVMCIAddress)(AsyncSocket *asock, uint32 *cid, uint32 *port);
   int (*getWebSocketError)(AsyncSocket *asock);
   char *(*getWebSocketURI)(AsyncSocket *asock);
   char *(*getWebSocketCookie)(AsyncSocket *asock);
   uint16 (*getWebSocketCloseStatus)(AsyncSocket *asock);
   const char *(*getWebSocketProtocol)(AsyncSocket *asock);
   int (*setWebSocketCookie)(AsyncSocket *asock, void *clientData, const char *path, const char *sessionId);
   int (*recvBlocking)(AsyncSocket *s, void *buf, int len, int *received,
                      int timeoutMS);
   int (*recvPartialBlocking)(AsyncSocket *s, void *buf, int len,
                              int *received, int timeoutMS);
   int (*sendBlocking)(AsyncSocket *s, void *buf, int len, int *sent,
                       int timeoutMS);
   int (*doOneMsg)(AsyncSocket *s, Bool read, int timeoutMS);
   int (*waitForConnection)(AsyncSocket *s, int timeoutMS);   // IN:


   /*
    * Internal function, called when refcount drops to zero:
    */
   void (*destroy)(AsyncSocket *asock);
} AsyncSocketVTable;


#define VT(x) ((x)->vt)
#define VALID(asock, x) LIKELY(asock && VT(asock)->x)


#endif
