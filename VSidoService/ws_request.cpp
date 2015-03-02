/*
Copyright (c) 2015, Asratec Corp.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of VSidoConnServer nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "uart_send.hpp"
#include "uart_read.hpp"
#include "ws_request.hpp"
#include "ws_response.hpp"
#include "dispatch.hpp"
#include "vsido_request_parser.hpp"
using namespace VSido;

#include <string>
#include <iostream>
#include <list>
using namespace std;
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h> 

#include "debug.h"



static WSRequest *globalRequest = nullptr;


#include <libwebsockets.h>

mutex mtxWSConnection;
list<struct libwebsocket *> wsCallbacks;


static int callback_http(struct libwebsocket_context * ctx,
                         struct libwebsocket *wsi,
                         enum libwebsocket_callback_reasons reason, void *user,
                         void *in, size_t len)
{
    return 0;
}

static int callback_vsido_cmd(struct libwebsocket_context * ctx,
                                   struct libwebsocket *wsi,
                                   enum libwebsocket_callback_reasons reason,
                                   void *user, void *inMsg, size_t lenMsg)
{
   
    switch (reason)
	{
        case LWS_CALLBACK_ESTABLISHED: // just log message that someone is connecting
    	{
            printf("connection established\n");
    		
    		
    		/// 設定タイムアウト
			struct timeval timeout;      
			timeout.tv_sec = 0;
			timeout.tv_usec = 1000 * 100;
			int sockfd = libwebsocket_get_socket_fd(wsi);
			if(0 > sockfd)
			{
        		perror("libwebsocket_get_socket_fd\n");
			}
			else
			{
				printf("%s,%d,sockfd=<%d>\n",__FILE__,__LINE__,sockfd);
			}
			if(setsockopt (sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
			{
        		perror("setsockopt failed\n");
			}
    		if(nullptr != wsi)
    		{
    			lock_guard<mutex> lock(mtxWSConnection);
    			wsCallbacks.push_back(wsi);
    		}
            break;
    	}
        case LWS_CALLBACK_RECEIVE:
    	{
        	if(nullptr == wsi)
        	{
        		DUMP_VAR(wsi);
        		return 0;
        	}
        	string msgStr((const char*)inMsg,lenMsg);
        	try
        	{
				DUMP_VAR_DETAILS(user);
        		globalRequest->recieve(msgStr,wsi);
        	}
        	catch(...)
        	{
        		printf("LWS_CALLBACK_RECEIVE exception %s:%d\n",__FILE__,__LINE__);
        	}
            break;
        }
    	case LWS_CALLBACK_CLOSED:
    	{
            printf("connection closed\n");
        	DUMP_VAR(wsi);
        	if(nullptr == wsi)
        	{
        		return 0;
        	}
    		{
    			lock_guard<mutex> lock(mtxWSConnection);
    			wsCallbacks.remove(wsi);
    		}
    		break;
    	}
    	default:
            break;
    }
   
   
    return 0;
}

static struct libwebsocket_protocols protocols[] = 
{
    /* first protocol must always be HTTP handler */
    {
        "http-only",   // name
        callback_http, // callback
        0              // per_session_data_size
    },
    {
        "vsido-cmd", // protocol name - very important!
        callback_vsido_cmd,   // callback
        0                          // we don't use any per session data

    },
    {
        NULL, NULL, 0   /* End of list */
    }
};

/** コンストラクタ
* @param dis 分配器
*/
WSRequest::WSRequest(Dispatcher &dis)
:_dis(dis)
{
	globalRequest = this;
}

/** WebSocket要求受付スレッド本体
* @return None
*/
void WSRequest::operator()()
{

	struct libwebsocket_context *context;
	
	lws_context_creation_info info ={0}; 
	info.port = 20080;
	info.protocols = protocols;

	info.gid = -1;
	info.uid = -1;
	
	// create libwebsocket context representing this server
	context = libwebsocket_create_context(&info);

	if (context == NULL)
	{
		fprintf(stderr, "libwebsocket init failed\n");
		return;
	}

	printf("starting server...\n");

	// infinite loop, to end this server send SIGTERM. (CTRL+C)
	while (true)
	{
		libwebsocket_service(context, 100);
	}

	libwebsocket_context_destroy(context);
   
}


/** WebSocket受信
* @param msg websocket message
* @param wsi websocket handler
* @return None
*/
void WSRequest::recieve(const string &msg,struct libwebsocket *wsi)
{
	DUMP_VAR_DETAILS(msg);
	shared_ptr<WSResponse> res(new WSResponse());
	res->setDist(wsi);
	DUMP_VAR_DETAILS(this);
	_dis.addRequest(msg,res);
}

