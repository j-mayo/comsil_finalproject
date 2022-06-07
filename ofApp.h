#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h"

typedef enum {F2, G2, A2, B2, C3, D3, E3, F3, G3, A3, B3, C4, D4, E4} white_piano_key;
typedef enum {Gb2, Ab2, Bb2, NOT1, Db3, Eb3, NOT2, Gb3, Ab3, Bb3, NOT3, Db4, Eb4} black_piano_key;

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
		
		ofxWinMenu* menu; // Menu object
		void appMenuFunction(string title, bool bChecked); // Menu return function

		// Used by example app
		ofTrueTypeFont myFont1;
		ofTrueTypeFont myFont2;
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

		// 건반
		float white_x; // 흰 건반의 폭
		float white_y; // 흰 건반의 길이
		float black_x; // 검은 폭
		float black_y; // 검은 높이

		int* white_keypressedflag; // key가 눌렸을 때, 화면에 표시하고 소리를 내는 등 flag
		int* black_keypressedflag;
		int white_r;
		int black_r;
		int* white_circle_x;
		int* black_circle_x;


		// Sound
		int* white_sound_flag;
		int* black_sound_flag;
		ofSoundPlayer sound_F2;
		ofSoundPlayer  beats;
		ofSoundPlayer  sound_G2;
		ofSoundPlayer  vocals;
};
