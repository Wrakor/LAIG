#pragma once
#include <string>
#include <iostream>


class Socket {
private:
	std::string ipaddr;
	unsigned int port;
	unsigned int __w64 m_socket; //fix (couldn't include winsock2 in this header, it's included in the cpp instead; SOCKET = unsigned int __w64)
public:
	Socket(std::string ipaddr, unsigned int port);
	~Socket();
	bool socketConnect();
	void envia(const char *s, int len);
	void recebe(char *ans);
	void quit();
	bool connected;
};