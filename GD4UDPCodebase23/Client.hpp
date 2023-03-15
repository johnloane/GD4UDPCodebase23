#pragma once

class Client
{
public:
	static void DoServiceLoop(const UDPSocketPtr client_socket);
	static void PrintOptions();
	static void GetChoice(std::string& choice);
	static void SendDataToServer(const UDPSocketPtr client_socket, char* choice);
	static void ReceiveDataFromServer(const UDPSocketPtr client_socket, char* receive_buffer, SocketAddress from_address, int bytes_received, bool& service_running);
	static void ProcessReceivedData(char* receive_buffer, int bytes_received, SocketAddress socket_address, bool& service_running);
	static int ConvertIPToInt(std::string ip_string);
	static void SendPlayerOutputByteStream(UDPSocketPtr client_socket, const Player* player);
	static void SendPlayerOutputBitStream(UDPSocketPtr client_socket, const Player* player);
	static const int32_t kMaxPacketSize = 1300;
	static void ByteSwapTest();
	static void SendWorld(UDPSocketPtr client_socket, Player* player, LinkingContext game_context);
	
	//static void ByteSwapTest();
};
