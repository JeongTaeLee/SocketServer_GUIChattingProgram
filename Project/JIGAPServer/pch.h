
#ifndef PCH_H
#define PCH_h
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <WinSock2.h>
#include <Windows.h>

#include <iostream>
#include <string>
#include <cstring>
#include <cstdarg>
#include <thread>
#include <functional>
#include <fstream>
#include <queue>
#include <list>
#include <map>
#include <set>
#include <crtdbg.h>
#include <atlconv.h>
#include <exception>
#include <functional>
#include <mysql.h>
#include <hash_map>
#include <io.h>

#define SAFE_DELETE(s) { if (s) { delete s; s = nullptr; } }
#define SAFE_DELETE_ARRAY(s) { if (s) { delete[] s; s = nullptr;} } 

#define StartMutex(handler) WaitForSingleObject(handler, INFINITE)
#define EndMutex(handler) ReleaseMutex(handler)

#ifdef _DEBUG
#define DEBUG_LOG(s) std::cout << __FILE__ << " : " << __LINE__ << " " <<  s << std::endl

#pragma comment(lib, "libprotobufd.lib")
#pragma comment(lib, "libprotobuf-lited.lib")
#else
#define DEBUG_LOG(s) 

#pragma comment(lib, "libprotobuf.lib")
#pragma comment(lib, "libprotobuf-lite.lib")
#endif

#include "CustomException.h"

#include "JIGAPPacket.pb.h"
#include "JIGAPStructure.h"
#include "TCPSocket.h"

#include "ObjectPool.h"
#include "PacketHandler.h"

#include "iniReader.h"
#include "iniWriter.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libmysql.lib")
#pragma comment(lib , "winmm.lib")


#endif PCH_H