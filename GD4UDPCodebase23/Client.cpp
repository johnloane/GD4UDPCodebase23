#include "SocketWrapperPCH.hpp"
#include "Client.hpp"


/*
	Client needs to offer the user a menu where they can select
	1. Echo
	2. DateAndTime
	3. Stats
	4. Quit
	Take the input from the user, send it to the server, and then receive the response from the server.
*/

int main()
{
	SocketUtil::StaticInit();
	UDPSocketPtr client_socket = SocketUtil::CreateUDPSocket(INET);
	client_socket->SetNonBlockingMode(false);
	//Client::ByteSwapTest();
	//Client::DoServiceLoop(client_socket);
	Player* john = new Player();
	//Client::SendPlayerOutputByteStream(client_socket, john);
	Client::SendPlayerOutputBitStream(client_socket, john);
	system("pause");
	return 0;
}

void Client::DoServiceLoop(const UDPSocketPtr client_socket)
{
	bool service_running = true;
	string choice;
	char receive_buffer[kMaxPacketSize];
	//The next 3 lines set the buffer to null
	char* begin = receive_buffer;
	char* end = begin + sizeof(receive_buffer);
	std::fill(begin, end, 0);
	SocketAddress sender_address;
	int bytes_received=0;

	while (service_running)
	{
		PrintOptions();
		GetChoice(choice);
		SendDataToServer(client_socket, (char*) choice.c_str());
		ReceiveDataFromServer(client_socket, receive_buffer, sender_address, bytes_received, service_running);
	}
	
}

void Client::PrintOptions()
{
	std::cout << "Please enter: " << std::endl;
	std::cout << "1. Echo" << std::endl;
	std::cout << "2. DateAndTime" << std::endl;
	std::cout << "3. Stats" << std::endl;
	std::cout << "4. Quit" << std::endl;
}

void Client::GetChoice(std::string& choice)
{
	std::getline(std::cin, choice);
}

void Client::SendDataToServer(const UDPSocketPtr client_socket, char* choice)
{
	SocketAddress server_address = SocketAddress(Client::ConvertIPToInt("10.102.212.100"), 50005);
	int bytes_sent = client_socket->SendTo(choice, sizeof(choice), server_address);
}

void Client::ReceiveDataFromServer(const UDPSocketPtr client_socket, char* receive_buffer, SocketAddress from_address, int bytes_received, bool& service_running)
{
	bytes_received = client_socket->ReceiveFrom(receive_buffer, kMaxPacketSize, from_address);
	if (bytes_received > 0)
	{
		ProcessReceivedData(receive_buffer, bytes_received, from_address, service_running);
	}
}

void Client::ProcessReceivedData(char* receive_buffer, int bytes_received, SocketAddress socket_address, bool& service_running)
{
	char key[] = "quit";
	
	if (strcmp(receive_buffer, key) == 0)
	{
		std::cout << "Quitting" << std::endl;
		service_running = false;
	}
	else
	{
		std::cout << "Got " << bytes_received << " bytes from " << socket_address.ToString() << std::endl;
		std::cout << receive_buffer << std::endl;
	}
}

int Client::ConvertIPToInt(std::string ip_string)
{
	int ip_int = 0;
	for (int i = 0; i < ip_string.length(); i++)
	{
		if (ip_string[i] == '.')
		{
			ip_string[i] = ' ';
		}
	}
	vector<int> array_tokens;
	std::stringstream ss(ip_string);
	int temp;
	while (ss >> temp)
	{
		array_tokens.emplace_back(temp);
	}

	for (int i = 0; i < array_tokens.size(); i++)
	{
		ip_int += array_tokens[i] << ((3 - i) * 8);
	}
	return ip_int;
	
}

void Client::SendPlayerOutputByteStream(UDPSocketPtr client_socket, const Player* player)
{
	SocketAddress server_address = SocketAddress(Client::ConvertIPToInt("127.0.0.1"), 50005);
	OutputMemoryStream output_stream;
	player->Write(output_stream);
	int bytes_sent = client_socket->SendTo(output_stream.GetBufferPtr(), output_stream.GetLength(), server_address);
	std::cout << "Sent: " << bytes_sent << std::endl;
	std::cin.ignore();
	
}

void Client::SendPlayerOutputBitStream(UDPSocketPtr client_socket, const Player* player)
{
	SocketAddress server_address = SocketAddress(Client::ConvertIPToInt("127.0.0.1"), 50005);
	OutputMemoryBitStream output_bit_stream;
	player->WriteBits(output_bit_stream);
	int bytes_sent = client_socket->SendTo(output_bit_stream.GetBufferPtr(), output_bit_stream.GetByteLength(), server_address);
	std::cout << "Sent: " << bytes_sent << std::endl;
}

void Client::ByteSwapTest()
{
	uint16_t value = 16;
	uint16_t swapped_value = ByteSwap(value);
	std::cout << value << " " << swapped_value << std::endl;
}

void Client::SendWorld(UDPSocketPtr client_socket, Player* player, LinkingContext game_context)
{
	SocketAddress server_address = SocketAddress(Client::ConvertIPToInt("127.0.0.1"), 50005);
	OutputMemoryBitStream output_bit_stream;
	//First send the type of packet
	PacketType or_packet = PacketType::PT_ReplicationData;
	output_bit_stream.WriteBits(&or_packet, GetRequiredBits<PacketType::PT_MAX>::Value);
	//Send the id of the player
	uint32_t player_id = game_context.GetNetworkId(player, true);
	std::cout << "Id is " << player_id << std::endl;
	output_bit_stream.Write(player_id);
	player->WriteBits(output_bit_stream);
	int bytes_sent = client_socket->SendTo(output_bit_stream.GetBufferPtr(), output_bit_stream.GetByteLength(), server_address);
	std::cout << "Sent: " << bytesSent << std::endl;
}

//void Client::ByteSwapTest()
//{
//	uint16_t value = 16;
//	uint16_t swapped_value = ByteSwap(value);
//	std::cout << value << " " << swapped_value << std::endl;
//}
