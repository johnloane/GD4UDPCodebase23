enum SocketAddressFamily
{
	INET = AF_INET,
	INET6 = AF_INET6
};

class SocketUtil
{
public:

	static bool			StaticInit();
	static void			CleanUp();

	static void			ReportError( const char* inOperationDesc );
	static int			GetLastError();
	static int ConvertIPToInt(std::string ipString);

	

	static UDPSocketPtr	CreateUDPSocket( SocketAddressFamily inFamily );

private:

};