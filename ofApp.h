#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h"
#include "string.h"

typedef enum {F2, G2, A2, B2, C3, D3, E3, F3, G3, A3, B3, C4, D4, E4, F4, G4, A4, B4, C5, D5, E5} white_piano_key;
typedef enum {Gb2, Ab2, Bb2, NOT1, Db3, Eb3, NOT2, Gb3, Ab3, Bb3, NOT3, Db4, Eb4, NOT4, Gb4, Ab4, Bb4, NOT5, Db5, Eb5} black_piano_key;

static char* white_key_name[] = {"F2\n z", "G2\n x", "A2\n c", "B2\n v", "C3\n b", "D3\n n", "E3\n m", "F3\n ,", "G3\n .", "A3\n q", "B3\n w", "C4\n e", "D4\n r", "E4\n t", "F4\n y", "G4\n u", "A4\n i", "B4\n o", "C5\n p", "D5\n [", "E5\n ]"};
static char* black_key_name[] = { "Gb2\n  s", "Ab2\n  d", "Bb2\n  f", "", "Db3\n  h", "Eb3\n  j", "", "Gb3\n  l", "Ab3\n  1", "Bb3\n  2", "", "Db4\n  4", "Eb4\n  5", "", "Gb4\n  7", "Ab4\n  8", "Bb4\n  9", "", "Db5\n  -", "Eb5\n  =" };


static char* white_file_name[] = {"F2", "G2", "A2", "B2", "C3", "D3", "E3", "F3", "G3", "A3", "B3", "C4", "D4", "E4", "F4", "G4", "A4", "B4", "C5", "D5", "E5"};
static char* black_file_name[] = { "Gb2", "Ab2", "Bb2", "", "Db3", "Eb3", "", "Gb3", "Ab3", "Bb3", "", "Db4", "Eb4", "", "Gb4", "Ab4", "Bb4", "", "Db5", "Eb5"};


static char white_keyboard[] = "zxcvbnm,.qwertyuiop[]";
static char black_keyboard[] = "sdf hj l12 45 789 -=";

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
		void free_Memory();
		
		void piano_draw();

		ofxWinMenu* menu; // Menu object
		void appMenuFunction(string title, bool bChecked); // Menu return function

		// Used by example app
		ofTrueTypeFont myFont1;
		ofTrueTypeFont myFont_white;
		ofTrueTypeFont myFont_black;
		ofImage myImage;
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

		int is_intro;

		// 건반
		int num_of_white;
		int num_of_black;
		float white_x; // 흰 건반의 폭
		float white_y; // 흰 건반의 길이
		float black_x; // 검은 폭
		float black_y; // 검은 높이

		float x1, x2, y1, y2; // 총 건반의 사각형 표시

		int* white_keypressedflag; // key가 눌렸을 때, 화면에 표시하고 소리를 내는 등 flag
		int* black_keypressedflag;
		int white_r;
		int black_r;
		int* white_circle_x;
		int* black_circle_x;


		// Sound
		int* white_sound_flag;
		int* black_sound_flag;
		//ofSoundPlayer sound_F2;
		//ofSoundPlayer  sound_G2;

		bool sound_load();
		ofSoundPlayer intro;
		ofSoundPlayer* white_sound;
		ofSoundPlayer* black_sound;
};
