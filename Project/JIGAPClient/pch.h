// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <Windows.h>

#include <iostream>
#include <string>
#include <cstring>
#include <cstdarg>
#include <thread>

#include <queue>
#include <list>
#include <functional>
#include <crtdbg.h>
#include <atlstr.h>

#include "../JIGAPServerStartLib/JIGAPStructure.h"
#include "../JIGAPServerStartLib/SocketAddress.h"
#include "../JIGAPServerStartLib/TCPIOData.h"
#include "../JIGAPServerStartLib/TCPSocket.h"
#include "../JIGAPServerStartLib/JIGAPPacket.pb.h"
#include "../JIGAPServerStartLib/PacketHandler.h"

#pragma comment(lib, "ws2_32.lib")
#ifdef _DEBUG
#pragma comment(lib, "libprotobufd.lib")
#pragma comment(lib, "libprotobuf-lited.lib")
#pragma comment(lib, "JIGAPServerStartLib.lib")

#define DEBUG_LOG(s) std::cout << s << std::endl;

#else
#pragma comment(lib, "libprotobuf.lib")
#pragma comment(lib, "libprotobuf-lite.lib")
#pragma comment(lib, "JIGAPServerStartLib.lib")

#define DEBUG_LOG(s) 

#endif

#endif //PCH_H
