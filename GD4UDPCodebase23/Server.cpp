#include "SocketWrapperPCH.hpp"
#include "Server.hpp"

/* Client can contact the server that offers three services
1. Echo - just repeats back what you sent
2. DateAndTime - current date and time
3. Stats - returns the number of queries that the server has handled
*/

int main()
{
	Player test = Player();
	std::cout << sizeof(test) << std::endl;
	SocketUtil::StaticInit();
	UDPSocketPtr server_socket = SocketUtil::CreateUDPSocket(INET);
	SocketAddress server_address = SocketAddress(Server::ConvertIPToInt("10.102.215.156"), 50005);
	server_socket->Bind(server_address);
	server_socket->SetNonBlockingMode(false);
	Server::DoServiceLoop(server_socket);
	
	return 0;
}

uint32_t Server::ConvertIPToInt(std::string ip_string)
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

void Server::DoServiceLoop(const UDPSocketPtr server_socket)
{
	bool service_running = true;
	char receive_buffer[kMaxPacketSize];
	
	//set the buffer to null
	//The next 3 lines set the buffer to null
	char* begin = receive_buffer;
	char* end = begin + sizeof(receive_buffer);
	std::fill(begin, end, 0);
	
	SocketAddress sender_address;
	int bytes_received = 0;
	int requests = 0;
	
	while (service_running)
	{
		bytes_received = server_socket->ReceiveFrom(receive_buffer, sizeof(receive_buffer), sender_address);
		if (bytes_received > 0)
		{
			requests++;
			ProcessReceivedData(receive_buffer, bytes_received, sender_address, server_socket, requests, service_running);
		}
	}
}

void Server::ProcessReceivedData(char* receive_buffer, int bytes_received, SocketAddress socket_address, const UDPSocketPtr& server_socket, int request, bool service_running)
{
	std::cout << "Got " << bytes_received << " bytes from " << socket_address.ToString() << std::endl;
	std::cout << "Received: " << receive_buffer << std::endl;

	char response_data[kMaxPacketSize] = "";
	int choice = atoi(receive_buffer);
	std::string current_date_time = "";
	std::string requests_string = "";
	std::string quit_string = "QUIT";

	switch (static_cast<Choice>(choice))
	{
	case Choice::ECHO:
		std::cout << "Echoing back to " << socket_address.ToString() << std::endl;
		strcpy_s(response_data, receive_buffer);
		break;
	case Choice::DATEANDTIME:
		std::cout << "DateAndTime request" << std::endl;
		current_date_time = ReturnCurrentDateAndTime();
		current_date_time.copy(response_data, current_date_time.length(), 0);
		break;
	case Choice::STATS:
		std::cout << "Stats request" << std::endl;
		requests_string = std::to_string(request);
		requests_string.copy(response_data, requests_string.length(), 0);
		break;
	case Choice::QUIT:
		std::cout << "Quit request" << std::endl;
		quit_string.copy(response_data, quit_string.length(), 0);
		service_running = false;
		break;
	}
	int bytes_sent = server_socket->SendTo(response_data, strlen(response_data), socket_address);
}

std::string Server::ReturnCurrentDateAndTime()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	
	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
	return ss.str();
}
