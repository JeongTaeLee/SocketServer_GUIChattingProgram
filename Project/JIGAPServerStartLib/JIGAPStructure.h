#pragma once
#define MAXBUFFERSIZE 2048 // 

#define MAXSTRSIZE 512
#define MAXNAMESIZE 32
#define MAXROOMNAMESIZE 32

// 서버 통신에서 패킷 판별을 위한 4글자 리터럴 입니다.
//const uint32_t requestLoginLiteral = 'RLIL';
const uint32_t requestLoginLiteral = 'RLIL'; // 클라에서 로그인 요청시 필요한 메시지 입니다.
const uint32_t answerLoginLiteral = 'ALIL';

// 룸 목록 요청과 답변시 필요한 리터럴 입니다.
const uint32_t requestRoomListLiteral = 'RRLL'; 
const uint32_t answerRoomListLiteral = 'ARLL';

// 룸 생성 요청과 답변시 필요한 리터럴 입니다.
const uint32_t requestCreateRoomLiteral = 'RCRL';
const uint32_t answerCreateRoomLiteral = 'ACRL';

// 룸 참가 요청과 답션시 필요한 리터럴 입니다.
const uint32_t requestJoinedRoomLiteral = 'RJRL'; 
const uint32_t answerJoinedRoomLiteral = 'AJRL';

// 룸 탈퇴 요청과 답변시 필요한 리터럴 입니다.
const uint32_t requestExitRoomLiteral = 'RERL'; 
const uint32_t answerExitRoomLiteral = 'AERL';

const uint32_t requestChattingLiteral = 'RCTL'; // 클라에서 채팅 요청시 필요한 메시지입니다.
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
