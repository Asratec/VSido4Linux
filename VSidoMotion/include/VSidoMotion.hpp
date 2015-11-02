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
#pragma once
#include <string>
#include <map>
#include <vector>
using namespace std;

namespace VSido
{
	class Motion
	{
	public:
		/** ���[�V�����f�[�^�����[�h����
		* @param name ���[�V�����t�@�C����
		* @return None
		*/
		void loadXml(const string &name);

		/** �����̃��[�V�����f�[�^�����ׂă��[�h����
		* @return None
		*/
		void loadXmlAll(void);

		/** �J�n����
		* @param id ���[�V����id(�t�@�C����)
		* @param loop true �J��Ԃ� false ���Đ�
		* @return None
		*/
		void start(const string &id,bool loop);
		
		/** ��~����
		* @return None
		*/
		void stop(void);
		
		/** ���[�V����id�̈ꗗ���擾����
		* @return ���[�V����id(�t�@�C����)
		*/
		vector<string> list(void);
		
		/** ���[�V�����t�@�C�����A�b�v���[�h����B
		* @param id ���[�V����id(�t�@�C����)
		* @param data ���[�V�����f�[�^
		* @return None
		*/
		void upload(const string &id,const string &data);

		/** ���[�V�����Đ��X���b�h�{��
		* @return None
		*/
    	void operator()();
	private:
		void loopMotion(double tic);
	private:	
    	int maxTick_ = -1;
		map <int,double> previousJiontAngle_;

	};
}
