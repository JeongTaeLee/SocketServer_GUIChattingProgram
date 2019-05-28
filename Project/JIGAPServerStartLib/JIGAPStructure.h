#pragma once
#define MAXBUFFERSIZE 2048 // 

#define MAXSTRSIZE 512
#define MAXNAMESIZE 32
#define MAXROOMNAMESIZE 32

// ���� ��ſ��� ��Ŷ �Ǻ��� ���� 4���� ���ͷ� �Դϴ�.
//const uint32_t requestLoginLiteral = 'RLIL';
const uint32_t requestLoginLiteral = 'RLIL'; // Ŭ�󿡼� �α��� ��û�� �ʿ��� �޽��� �Դϴ�.
const uint32_t answerLoginLiteral = 'ALIL';

// �� ��� ��û�� �亯�� �ʿ��� ���ͷ� �Դϴ�.
const uint32_t requestRoomListLiteral = 'RRLL'; 
const uint32_t answerRoomListLiteral = 'ARLL';

// �� ���� ��û�� �亯�� �ʿ��� ���ͷ� �Դϴ�.
const uint32_t requestCreateRoomLiteral = 'RCRL';
const uint32_t answerCreateRoomLiteral = 'ACRL';

// �� ���� ��û�� ��ǽ� �ʿ��� ���ͷ� �Դϴ�.
const uint32_t requestJoinedRoomLiteral = 'RJRL'; 
const uint32_t answerJoinedRoomLiteral = 'AJRL';

// �� Ż�� ��û�� �亯�� �ʿ��� ���ͷ� �Դϴ�.
const uint32_t requestExitRoomLiteral = 'RERL'; 
const uint32_t answerExitRoomLiteral = 'AERL';

const uint32_t requestChattingLiteral = 'RCTL'; // Ŭ�󿡼� ä�� ��û�� �ʿ��� �޽����Դϴ�.
const uint32_t anwserChattingLiteral = 'ACTL'; 

enum JIGAPSTATE
{
	E_NOTROOM,
	E_REQUESTROOM,
	E_ONROOM,
};

#include "SocketAddress.h"
#include "TCPIOData.h"
#include "TCPSocket.h"
#include "SerializeObject.h"
#include "Room.h"
