#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h"
#include <string.h>
#include <time.h>

typedef enum {F2, G2, A2, B2, C3, D3, E3, F3, G3, A3, B3, C4, D4, E4, F4, G4, A4, B4, C5, D5, E5} white_piano_key;
typedef enum {Gb2, Ab2, Bb2, NOT1, Db3, Eb3, NOT2, Gb3, Ab3, Bb3, NOT3, Db4, Eb4, NOT4, Gb4, Ab4, Bb4, NOT5, Db5, Eb5} black_piano_key;
// enum type 구조를 활용해 코드의 가독성을 높이고 구현을 용이하게 하였다.

static char* white_key_name[] = {"F2\n z", "G2\n x", "A2\n c", "B2\n v", "C3\n b", "D3\n n", "E3\n m", "F3\n ,", "G3\n .", "A3\n q", "B3\n w", "C4\n e", "D4\n r", "E4\n t", "F4\n y", "G4\n u", "A4\n i", "B4\n o", "C5\n p", "D5\n [", "E5\n ]"};
static char* black_key_name[] = { "Gb2\n  s", "Ab2\n  d", "Bb2\n  f", "", "Db3\n  h", "Eb3\n  j", "", "Gb3\n  l", "Ab3\n  1", "Bb3\n  2", "", "Db4\n  4", "Eb4\n  5", "", "Gb4\n  7", "Ab4\n  8", "Bb4\n  9", "", "Db5\n  -", "Eb5\n  =" };
// 건반에 직접 출력될 문자열이다. 각 건반마다 해당하는 음과, 눌러야 할 키를 같이 명시했다. 검은 건반의 경우 중간중간 건너뛰어야 하므로 빈 문자열을 집어넣었다.

static char* white_file_name[] = {"F2", "G2", "A2", "B2", "C3", "D3", "E3", "F3", "G3", "A3", "B3", "C4", "D4", "E4", "F4", "G4", "A4", "B4", "C5", "D5", "E5"};
static char* black_file_name[] = { "Gb2", "Ab2", "Bb2", "", "Db3", "Eb3", "", "Gb3", "Ab3", "Bb3", "", "Db4", "Eb4", "", "Gb4", "Ab4", "Bb4", "", "Db5", "Eb5"};
// 각 음에 해당하는 sound file을 불러올 때 코드의 양을 줄이기 위해 선언하였다. enum type을 활용해 (ex: white_file_name[F2] = "F2") 해당하는 음의 파일명을 알 수 있다

static char white_keyboard[] = "zxcvbnm,.qwertyuiop[]";
static char black_keyboard[] = "sdf hj l12 45 789 -=";
// key 입력이 들어왔을 때 건반에 일대일 대응되는 모든 키를 확인해야 하는데, 일일히 for문을 달아주면 코드 양이 끔찍하게 불어날 것이다.
// 이를 위해 미리 각 키들의 문자열 배열을 선언해, 맨 위의 enum type과 함께 (if key == white_keyboard[F2]) 등으로 키 입력을 알 수 있고,
// enum type이므로 for loop로 간결하게 key의 입력을 검사할 수 있다는 장점이 있다.


class Note { // 음표 자료구조
public:
	int x; // 건반 인덱스: 흰, 검은에 따라 달라진다.
	float y; // 음표의 y좌표
	short int len; // 음표 타입 = 4분음표, 8분음표, 16분음표 중 하나이다.
	short int key; // 흰 건반, 검은 건반 중 하나에 배정된다.
	float speed; // 떨어질 속도.
	float acce; // 가속도
	Note* prev;
	Note* next;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		// 추가 선언한 함수들
		void free_Memory(); // 사용된 동적 메모리를 해제하는 함수
		
		void play_piano_init(); // play_piano 시 자료구조 초기화
		void piano_draw(); // 피아노 건반들을 화면에 그려주는 역할.
		void activatedkey_draw();
		void print_window();

		void perfect_timing_init(int level);
		void perfect_timing_end();
		void perfect_timing_draw();
		void calculate_timing();

		void sound_load(); // 필요한 sound file들을 로딩해주는 역할

		void image_load(); // 필요한 image들을 load하는 함수
		void create_note(); // note 생성 후 연결시킴
		void update_note(); // note의 상태를 update

		ofxWinMenu* menu; // Menu object
		void appMenuFunction(string title, bool bChecked); // Menu return function

		

		float windowWidth, windowHeight; 
		HWND hWnd; // Application window
		HWND hWndForeground; // current foreground window

		// Example menu variables
		bool bShowInfo;
		bool bFullscreen;
		bool bTopmost;
		// Example functions
		void doFullScreen(bool bFull);
		void doTopmost(bool bTop);

		// 추가 선언해준 변수들
		ofTrueTypeFont myFont1; // consil... 출력 위한 폰트
		ofTrueTypeFont myFont2; // score 출력
		ofTrueTypeFont myFont_white; // 흰 건반의 음과 눌러야 하는 키보드 버튼을 출력해주기 위한 폰트
		ofTrueTypeFont myFont_black; // 검은 건반의 음과 눌러야 하는 키보드 버튼을 출력해주기 위한 폰트
		ofImage myImage; // image를 넣을 일이 있을 때 사용,  *** 없으면 삭제 예정 ***

		int is_intro; // intro 음이 loading되었는지 알려주는 변수. 1이면 로딩 된 것.
		int play_piano; // play_piano mode인지 알려주는 변수. 1이면 play_piano인 것
		int perfect_timing; // 현재 게임 중임을 나타내는 변수. 1이면 그런 것

		// 건반
		int num_of_white; // 흰 건반의 개수
		int num_of_black; // 검은 건반의 개수
		float white_x; // 흰 건반의 폭
		float white_y; // 흰 건반의 길이
		float black_x; // 검은 폭
		float black_y; // 검은 높이

		float x1, x2, y1, y2; // 총 건반의 사각형 표시, 왼쪽 위와 오른쪽 아래 꼭짓점의 좌효

		int* white_keypressedflag; // key가 눌렸을 때, 화면에 표시하고 소리를 내는 등 flag
		int* black_keypressedflag; // (black key의 경우)
		int white_r; // key가 눌렸을 때 화면에 키가 눌림을 표시해주는 원을 그릴 때의 반지름
		int black_r; // (black key의 경우)
		//int* white_circle_x; 
		//int* black_circle_x;

		int* white_sustainflag; // 현재 sustain중인 흰 건반들을 표시해주는 flag 배열
		int* black_sustainflag; // (black key의 경우)
		int sustain_pedal; // sustain pedal이 눌린 경우 1, 아니면 0


		// Sound
		//int* white_sound_flag; 
		//int* black_sound_flag;
		//ofSoundPlayer sound_F2;
		//ofSoundPlayer  sound_G2;

		
		ofSoundPlayer intro; // intro 음악을 담을 ofSoundPlayer 객체
		ofSoundPlayer* white_sound; // 흰 건반의 음들을 담는 ofSoundPlayer의 배열
		ofSoundPlayer* black_sound; // 검은 건반의 음들을 담는 ofSoundPlayer의 배열

		// 이미지
		//ofImage note_4;
		//ofImage note_8;
		//ofImage note_16;
		ofImage* notes;

		// perfect timing
		int num_of_perfect;
		int num_of_good;
		int num_of_fail;
		Note* score_front; // 음표들이 모이면 악보가 된다. 음표 연결 리스트의 시작 음표를 저장하는 포인터 변수.
		Note* score_rear; // 음표들의 끝을 저장. 
};
