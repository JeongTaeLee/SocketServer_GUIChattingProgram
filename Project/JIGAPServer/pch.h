
#ifndef PCH_H
#define PCH_h
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include <WinSock2.h>
#include <Windows.h>

#include <iostream>
#include <string>
#include <cstring>
#include <cstdarg>
#include <thread>
#include <functional>
#include <queue>
#include <list>
#include <map>
#include <set>
#include <crtdbg.h>
#include <atlconv.h>
#include <mysql.h>
#include <hash_map>

#define SAFE_DELETE(s) { if (s) { delete s; s = nullptr; } }
#define SAFE_DELETE_ARRAY(s) { if (s) { delete[] s; s = nullptr;} } 

#define StartMutex(handler) WaitForSingleObject(handler, INFINITE)
#define EndMutex(handler) ReleaseMutex(handler)

#include "JIGAPPacket.pb.h"
#include "JIGAPStructure.h"
#include "TCPSocket.h"

#include "ObjectPool.h"
#include "PacketHandler.h"

#include "DBManager.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libmysql.lib")


#ifdef _DEBUG
#define DEBUG_LOG(s) std::cout << __FILE__ << " : " << __LINE__ << s << std::endl

#pragma comment(lib, "libprotobufd.lib")
#pragma comment(lib, "libprotobuf-lited.lib")
#else
#define DEBUG_LOG(s) 

#pragma comment(lib, "libprotobuf.lib")
#pragma comment(lib, "libprotobuf-lite.lib")
#endif

#endif PCH_H