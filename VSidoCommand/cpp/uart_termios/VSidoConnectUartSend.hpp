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
#ifndef __UART_SEND__
#define __UART_SEND__


//#include <string>
#include <list>
using namespace std;

namespace VSido
{

/**
 * VSidoと繋がるUARTの送信
 */

class UARTSend
{
public:
	/** コンストラクタ
	*/
    UARTSend();

	/** ファイルデスクリプタを設定する
	* @param fd VSidoと繋がるURATファイルデスクリプタ
	* @return None
	*/
	void setFD(int fd){_fd = fd;}
	
	/** VSidoと繋がるURATにコマンド送信
	* @param data VSido制御コマンド
	* @return None
	*/
	void send(const list<unsigned char> &data);
    
private:
    void write(const list<unsigned char> &data);
    void tryReconnect(void);
private:
    int _fd;
    list<list<unsigned char>> _cmds;

};
} // namespace VSido

#endif //__UART_SEND__
