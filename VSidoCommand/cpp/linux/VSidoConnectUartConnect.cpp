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

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <stdio.h>
#include <string>
#include <thread>
#include <iostream>
#include <map>
#include <fstream>
#include <streambuf>
using namespace std;

#include "VSidoConnectUartConnect.hpp"
using namespace VSido;

#include "debug.h"


using namespace std;




extern string execShell(string cmd);
void dumpUartFlags(termios term);



/** コンストラクタ
* @param dev VSidoコネクトと繋ぐデバイス
*/
UARTConnect::UARTConnect(const string &dev)
:_dev(dev)
,_baudrate(B115200)
{
	openSPP();
    DUMP_VAR(_uart);
}


/** コンストラクタ
* @param dev VSidoコネクトと繋ぐデバイス
*/
UARTConnect::UARTConnect(const string &dev,int baud)
:_dev(dev)
,_baudrate(baud)
{
	openSPP();
    DUMP_VAR(_uart);
}


/** URATを変更する。
* @return UART
*/
UARTConnect::operator int() const
{
	return _uart;
}


static struct termios oldtio;
static struct termios newtio;

#ifndef N_HCI
#define N_HCI	15
#endif


/** SPPを開く
* @return None
*/
void UARTConnect::openSPP()
{
    _uart = open( _dev.c_str(), O_RDWR | O_NOCTTY|O_NDELAY | O_NONBLOCK );
    if( _uart < 0 )
    {
        perror( _dev.c_str() );
    	return;
    }
	auto tcret = tcflush(_uart,TCIOFLUSH);
	if (tcret == -1) 
	{
        perror( _dev.c_str() );
	}

	tcgetattr( _uart, &oldtio );

	/// ビットレート設定など、シリアル通信の属性設定
    newtio = oldtio;
	
	dumpUartFlags(newtio);

	newtio.c_iflag |=  IGNBRK | IGNPAR;
	newtio.c_iflag &= ~(ICRNL | IXON);
//	newtio.c_iflag &= ~(ICRNL);
//	newtio.c_iflag |= IXOFF;
	

	newtio.c_cflag =  CS8 | CLOCAL | CREAD;
	newtio.c_cflag |=  _baudrate;
	
    newtio.c_lflag = 0;

	
    newtio.c_ispeed = _baudrate;
    newtio.c_ospeed = _baudrate;
	
	newtio.c_cc[VMIN] = 1;
	newtio.c_cc[VTIME] = 1;
	
	newtio.c_oflag &= ~(ONLCR);



	dumpUartFlags(newtio);

    tcsetattr(_uart, TCSANOW, &newtio);
    DUMP_VAR(_uart);


}

/** SPPを閉じる
* @return None
*/
void UARTConnect::closeSPP()
{
    if( _uart < 0 )
    {
    	return;
    }
    tcsetattr(_uart, TCSANOW, &oldtio);
    close(_uart);
}




#define DUMP_FLAGS(x,y) \
{\
	if(x & y)\
	{\
		cout << "DUMP_FLAGS:" << __func__ << #x << ":" << #y << " is ON "<< endl; \
	}\
}

#define DUMP_CC_FLAGS(x,y) \
{\
	cout << "DUMP_FLAGS:" << __func__ << #x << "["  << #y << "] is =<" << x[y] << ">" << endl; \
}


void dumpUartFlags(termios term)
{
    FATAL_VAR(&term);

    FATAL_VAR(cfgetispeed(&term));
    FATAL_VAR(cfgetospeed(&term));
//    FATAL_VAR(cfgetspeed(&term));
	
	DUMP_FLAGS(term.c_iflag,IGNBRK);
	DUMP_FLAGS(term.c_iflag,BRKINT);
	DUMP_FLAGS(term.c_iflag,IGNPAR);
	DUMP_FLAGS(term.c_iflag,PARMRK);
	DUMP_FLAGS(term.c_iflag,INPCK);
	DUMP_FLAGS(term.c_iflag,ISTRIP);
	DUMP_FLAGS(term.c_iflag,INLCR);
	DUMP_FLAGS(term.c_iflag,IGNCR);
	DUMP_FLAGS(term.c_iflag,ICRNL);
	DUMP_FLAGS(term.c_iflag,IUCLC);
	DUMP_FLAGS(term.c_iflag,IXON);
	DUMP_FLAGS(term.c_iflag,IXANY);
	DUMP_FLAGS(term.c_iflag,IXOFF);
	DUMP_FLAGS(term.c_iflag,IMAXBEL);
	DUMP_FLAGS(term.c_iflag,IUTF8);
	
	DUMP_FLAGS(term.c_oflag,OPOST);
	DUMP_FLAGS(term.c_oflag,OLCUC);
	DUMP_FLAGS(term.c_oflag,ONLCR);
	DUMP_FLAGS(term.c_oflag,OCRNL);
	DUMP_FLAGS(term.c_oflag,ONOCR);
	DUMP_FLAGS(term.c_oflag,ONLRET);
	DUMP_FLAGS(term.c_oflag,OFILL);
	DUMP_FLAGS(term.c_oflag,OFDEL);
	DUMP_FLAGS(term.c_oflag,NLDLY);
	DUMP_FLAGS(term.c_oflag,CRDLY);
	DUMP_FLAGS(term.c_oflag,TABDLY);
	DUMP_FLAGS(term.c_oflag,BSDLY);
	DUMP_FLAGS(term.c_oflag,VTDLY);
	DUMP_FLAGS(term.c_oflag,FFDLY);


	DUMP_FLAGS(term.c_cflag,CBAUD);
	DUMP_FLAGS(term.c_cflag,CBAUDEX);
	DUMP_FLAGS(term.c_cflag,CSIZE);
	DUMP_FLAGS(term.c_cflag,CSTOPB);
	DUMP_FLAGS(term.c_cflag,CREAD);
	DUMP_FLAGS(term.c_cflag,PARENB);
	DUMP_FLAGS(term.c_cflag,PARODD);
	DUMP_FLAGS(term.c_cflag,HUPCL);
	DUMP_FLAGS(term.c_cflag,CLOCAL);
//	DUMP_FLAGS(term.c_cflag,LOBLK);
	DUMP_FLAGS(term.c_cflag,CIBAUD);
	DUMP_FLAGS(term.c_cflag,CMSPAR);
	DUMP_FLAGS(term.c_cflag,CRTSCTS);
	

	DUMP_FLAGS(term.c_lflag,ISIG);
	DUMP_FLAGS(term.c_lflag,ICANON);
	DUMP_FLAGS(term.c_lflag,XCASE);
	DUMP_FLAGS(term.c_lflag,ECHO);
	DUMP_FLAGS(term.c_lflag,ECHOE);
	DUMP_FLAGS(term.c_lflag,ECHOK);
	DUMP_FLAGS(term.c_lflag,ECHONL);
	DUMP_FLAGS(term.c_lflag,ECHOCTL);
	DUMP_FLAGS(term.c_lflag,ECHOPRT);
	DUMP_FLAGS(term.c_lflag,ECHOKE);
//	DUMP_FLAGS(term.c_lflag,DEFECHO);
	DUMP_FLAGS(term.c_lflag,FLUSHO);
	DUMP_FLAGS(term.c_lflag,NOFLSH);
	DUMP_FLAGS(term.c_lflag,TOSTOP);
	DUMP_FLAGS(term.c_lflag,PENDIN);
	DUMP_FLAGS(term.c_lflag,IEXTEN);
	
	DUMP_CC_FLAGS(term.c_cc ,VDISCARD);
//	DUMP_CC_FLAGS(term.c_cc ,VDSUSP);
	DUMP_CC_FLAGS(term.c_cc ,VEOF);
	DUMP_CC_FLAGS(term.c_cc ,VEOL);
	DUMP_CC_FLAGS(term.c_cc ,VEOL2);
	DUMP_CC_FLAGS(term.c_cc ,VERASE);
	DUMP_CC_FLAGS(term.c_cc ,VINTR);
	DUMP_CC_FLAGS(term.c_cc ,VKILL);
	DUMP_CC_FLAGS(term.c_cc ,VLNEXT);
	DUMP_CC_FLAGS(term.c_cc ,VMIN);
	DUMP_CC_FLAGS(term.c_cc ,VQUIT);
	DUMP_CC_FLAGS(term.c_cc ,VREPRINT);
	DUMP_CC_FLAGS(term.c_cc ,VSTART);
//	DUMP_CC_FLAGS(term.c_cc ,VSTATUS);
	DUMP_CC_FLAGS(term.c_cc ,VSTOP);
	DUMP_CC_FLAGS(term.c_cc ,VSUSP);
//	DUMP_CC_FLAGS(term.c_cc ,VSWTCH);
	DUMP_CC_FLAGS(term.c_cc ,VTIME);
	DUMP_CC_FLAGS(term.c_cc ,VWERASE);

}




