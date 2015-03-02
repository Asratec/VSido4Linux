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
#ifndef __VISIDO_REQUEST_IK_HPP__
#define __VISIDO_REQUEST_IK_HPP__


#include <list>
#include <string>
using namespace std;
#include "vsido_request_common.hpp"
#include "picojson.h"

namespace VSido
{
/**
* IK設定
*/
class RequestIK : public Request
{
public:
	/** コンストラクタ
	* @param[in] raw httpサーバーからのJson要求
	*  {
	*	 "cmd" : "ik",
    *    "ikf": {
    *      "cur" :{"torq":false,"pos":false,"rot":false},
    *      "dist"    :{"torq":false,"pos":true,"rot":false},
    *    },
    *    "kdts":[
    *      {
    *        "kid":1~15,
    *        "kdt": {
    *          "pos"    :{"x"  :-100~100,"y"  :-100~100,"z" :-100~100},
    *          "rot"    :{"Rx" :-100~100,"Ry" :-100~100,"Rz":-100~100},
    *          "torq"      :{"Tx" :-100~100,"Ty" :-100~100,"Tz":-100~100},
    *        }
    *      }
    *    ]
	*  }
	*/
    RequestIK(picojson::object &raw);
	
	/** Json要求からVSidoパケットへ変換する
	* @param None
	* @return VSidoパケット
	*/
    virtual list<unsigned char> conv(void);
private:
    RequestIK(void);
	bool parseIKF(void);
	void parseKDT(void);
	void parseKID(void);
private:
    picojson::object &_raw;
	bool _cur_pos;
	bool _cur_torq;
	bool _cur_rot;
	bool _dist_pos;
	bool _dist_torq;
	bool _dist_rot;
};
} // namespace VSido

#endif //__VISIDO_REQUEST_IK_HPP__