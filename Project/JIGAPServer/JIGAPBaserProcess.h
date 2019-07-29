#pragma once

class JIGAPServer;
class TCPSocket;

class JIGAPBaserProcess
{
	protected:
	JIGAPServer* lpJIGAPServer = nullptr;

private:
	JIGAPBaserProcess() {} 
public:
	JIGAPBaserProcess(JIGAPServer* inLpJIGAPServer) { lpJIGAPServer = inLpJIGAPServer; };
	virtual ~JIGAPBaserProcess() {};

	virtual void OnInitialize() PURE;
	virtual void OnRelease()						 PURE;
	virtual void OnConnect(TCPSocket * lpInSocket)	 PURE;
	virtual void OnDisconnect(TCPSocket* lpInSocket) PURE;
	virtual void OnProcess(TCPSocket * lpInTCPSocket, PacketHandler* lpHandler) PURE;

};

