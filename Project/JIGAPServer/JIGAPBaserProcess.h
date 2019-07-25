#pragma once

class JIGAPServer;
class TCPSocket;

class JIGAPBaserProcess
{
public:
	JIGAPBaserProcess() {};
	virtual ~JIGAPBaserProcess() {};

	virtual void OnInitialize() PURE;
	virtual void OnRelease()						 PURE;
	virtual void OnConnect(TCPSocket * lpInSocket)	 PURE;
	virtual void OnDisconnect(TCPSocket* lpInSocket) PURE;
	virtual void OnProcess(TCPSocket * lpInTCPSocket, PacketHandler* lpHandler) PURE;
};

