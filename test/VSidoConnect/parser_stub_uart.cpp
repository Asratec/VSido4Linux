#include <list>
#include <map>
#include <chrono>
#include <tuple>
using namespace std;



list<unsigned char> sendBuffer;
list<unsigned char> readBuffer;
map<int,tuple<list<unsigned char>,chrono::milliseconds>> ut_uart_read;


namespace VSido
{
/** ���M����
* @param data ���M�f�[�^
* @return None
*/
void sendUart(list<unsigned char> data)
{
	sendBuffer = data ;
}

/** UART����M����B 
* @return ��M�f�[�^
*/
map<int,tuple<list<unsigned char>,chrono::milliseconds>> readUart()
{
	return ut_uart_read;
}


} // namespace VSido

