#include "SocketWrapperPCH.hpp"



bool SocketUtil::StaticInit()
{
#if _WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if ( iResult != NO_ERROR )
	{
		ReportError ("Starting Up");
		return false;
	}
#endif
	return true;
}

void SocketUtil::CleanUp()
{
#if _WIN32
	WSACleanup();
#endif
}


void SocketUtil::ReportError( const char* inOperationDesc )
{
#if _WIN32
	LPVOID lpMsgBuf;
	DWORD errorNum = GetLastError();
	
	FormatMessage(
				  FORMAT_MESSAGE_ALLOCATE_BUFFER |
				  FORMAT_MESSAGE_FROM_SYSTEM |
				  FORMAT_MESSAGE_IGNORE_INSERTS,
				  NULL,
				  errorNum,
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				  (LPTSTR) &lpMsgBuf,
				  0, NULL );
	
	
	LOG( "Error %s: %d- %s", inOperationDesc, errorNum, lpMsgBuf );
#else
	LOG( "Error: %hs", inOperationDesc );
#endif
}

int SocketUtil::GetLastError()
{
#if _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
	
}

UDPSocketPtr SocketUtil::CreateUDPSocket( SocketAddressFamily inFamily )
{
	SOCKET s = socket( inFamily, SOCK_DGRAM, IPPROTO_UDP );
	
	if( s != INVALID_SOCKET )
	{
		return UDPSocketPtr( new UDPSocket( s ) );
	}
	else
	{
		ReportError( "SocketUtil::CreateUDPSocket" );
		return nullptr;
	}
}


int SocketUtil::ConvertIPToInt(std::string ipString)
{
	int intIP = 0;
	for (int i = 0; i < ipString.length(); ++i)
	{
		if (ipString[i] == '.')
		{
			ipString[i] = ' ';
		}
	}
	vector<int> arrayTokens;
	std::stringstream ss(ipString);
	int temp;
	while (ss >> temp)
	{
		arrayTokens.emplace_back(temp);
	}
	for (int i = 0; i < arrayTokens.size(); ++i)
	{
		intIP += (arrayTokens[i] << ((3 - i) * 8));
	}
	return intIP;
}
