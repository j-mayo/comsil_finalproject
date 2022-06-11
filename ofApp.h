#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h"
#include <string.h>
#include <time.h>

typedef enum {F2, G2, A2, B2, C3, D3, E3, F3, G3, A3, B3, C4, D4, E4, F4, G4, A4, B4, C5, D5, E5} white_piano_key;
typedef enum {Gb2, Ab2, Bb2, NOT1, Db3, Eb3, NOT2, Gb3, Ab3, Bb3, NOT3, Db4, Eb4, NOT4, Gb4, Ab4, Bb4, NOT5, Db5, Eb5} black_piano_key;
// enum type ������ Ȱ���� �ڵ��� �������� ���̰� ������ �����ϰ� �Ͽ���.

static char* white_key_name[] = {"F2\n z", "G2\n x", "A2\n c", "B2\n v", "C3\n b", "D3\n n", "E3\n m", "F3\n ,", "G3\n .", "A3\n q", "B3\n w", "C4\n e", "D4\n r", "E4\n t", "F4\n y", "G4\n u", "A4\n i", "B4\n o", "C5\n p", "D5\n [", "E5\n ]"};
static char* black_key_name[] = { "Gb2\n  s", "Ab2\n  d", "Bb2\n  f", "", "Db3\n  h", "Eb3\n  j", "", "Gb3\n  l", "Ab3\n  1", "Bb3\n  2", "", "Db4\n  4", "Eb4\n  5", "", "Gb4\n  7", "Ab4\n  8", "Bb4\n  9", "", "Db5\n  -", "Eb5\n  =" };
// �ǹݿ� ���� ��µ� ���ڿ��̴�. �� �ǹݸ��� �ش��ϴ� ����, ������ �� Ű�� ���� ����ߴ�. ���� �ǹ��� ��� �߰��߰� �ǳʶپ�� �ϹǷ� �� ���ڿ��� ����־���.

static char* white_file_name[] = {"F2", "G2", "A2", "B2", "C3", "D3", "E3", "F3", "G3", "A3", "B3", "C4", "D4", "E4", "F4", "G4", "A4", "B4", "C5", "D5", "E5"};
static char* black_file_name[] = { "Gb2", "Ab2", "Bb2", "", "Db3", "Eb3", "", "Gb3", "Ab3", "Bb3", "", "Db4", "Eb4", "", "Gb4", "Ab4", "Bb4", "", "Db5", "Eb5"};
// �� ���� �ش��ϴ� sound file�� �ҷ��� �� �ڵ��� ���� ���̱� ���� �����Ͽ���. enum type�� Ȱ���� (ex: white_file_name[F2] = "F2") �ش��ϴ� ���� ���ϸ��� �� �� �ִ�

static char white_keyboard[] = "zxcvbnm,.qwertyuiop[]";
static char black_keyboard[] = "sdf hj l12 45 789 -=";
// key �Է��� ������ �� �ǹݿ� �ϴ��� �����Ǵ� ��� Ű�� Ȯ���ؾ� �ϴµ�, ������ for���� �޾��ָ� �ڵ� ���� �����ϰ� �Ҿ ���̴�.
// �̸� ���� �̸� �� Ű���� ���ڿ� �迭�� ������, �� ���� enum type�� �Բ� (if key == white_keyboard[F2]) ������ Ű �Է��� �� �� �ְ�,
// enum type�̹Ƿ� for loop�� �����ϰ� key�� �Է��� �˻��� �� �ִٴ� ������ �ִ�.


class Note { // ��ǥ �ڷᱸ��
public:
	int x; // �ǹ� �ε���: ��, ������ ���� �޶�����.
	float y; // ��ǥ�� y��ǥ
	short int len; // ��ǥ Ÿ�� = 4����ǥ, 8����ǥ, 16����ǥ �� �ϳ��̴�.
	short int key; // �� �ǹ�, ���� �ǹ� �� �ϳ��� �����ȴ�.
	float speed; // ������ �ӵ�.
	float acce; // ���ӵ�
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

		// �߰� ������ �Լ���
		void free_Memory(); // ���� ���� �޸𸮸� �����ϴ� �Լ�
		
		void play_piano_init(); // play_piano �� �ڷᱸ�� �ʱ�ȭ
		void piano_draw(); // �ǾƳ� �ǹݵ��� ȭ�鿡 �׷��ִ� ����.
		void activatedkey_draw();
		void print_window();

		void perfect_timing_init(int level);
		void perfect_timing_end();
		void perfect_timing_draw();
		void calculate_timing();

		void sound_load(); // �ʿ��� sound file���� �ε����ִ� ����

		void image_load(); // �ʿ��� image���� load�ϴ� �Լ�
		void create_note(); // note ���� �� �����Ŵ
		void update_note(); // note�� ���¸� update

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

		// �߰� �������� ������
		ofTrueTypeFont myFont1; // consil... ��� ���� ��Ʈ
		ofTrueTypeFont myFont2; // score ���
		ofTrueTypeFont myFont_white; // �� �ǹ��� ���� ������ �ϴ� Ű���� ��ư�� ������ֱ� ���� ��Ʈ
		ofTrueTypeFont myFont_black; // ���� �ǹ��� ���� ������ �ϴ� Ű���� ��ư�� ������ֱ� ���� ��Ʈ
		ofImage myImage; // image�� ���� ���� ���� �� ���,  *** ������ ���� ���� ***

		int is_intro; // intro ���� loading�Ǿ����� �˷��ִ� ����. 1�̸� �ε� �� ��.
		int play_piano; // play_piano mode���� �˷��ִ� ����. 1�̸� play_piano�� ��
		int perfect_timing; // ���� ���� ������ ��Ÿ���� ����. 1�̸� �׷� ��

		// �ǹ�
		int num_of_white; // �� �ǹ��� ����
		int num_of_black; // ���� �ǹ��� ����
		float white_x; // �� �ǹ��� ��
		float white_y; // �� �ǹ��� ����
		float black_x; // ���� ��
		float black_y; // ���� ����

		float x1, x2, y1, y2; // �� �ǹ��� �簢�� ǥ��, ���� ���� ������ �Ʒ� �������� ��ȿ

		int* white_keypressedflag; // key�� ������ ��, ȭ�鿡 ǥ���ϰ� �Ҹ��� ���� �� flag
		int* black_keypressedflag; // (black key�� ���)
		int white_r; // key�� ������ �� ȭ�鿡 Ű�� ������ ǥ�����ִ� ���� �׸� ���� ������
		int black_r; // (black key�� ���)
		//int* white_circle_x; 
		//int* black_circle_x;

		int* white_sustainflag; // ���� sustain���� �� �ǹݵ��� ǥ�����ִ� flag �迭
		int* black_sustainflag; // (black key�� ���)
		int sustain_pedal; // sustain pedal�� ���� ��� 1, �ƴϸ� 0


		// Sound
		//int* white_sound_flag; 
		//int* black_sound_flag;
		//ofSoundPlayer sound_F2;
		//ofSoundPlayer  sound_G2;

		
		ofSoundPlayer intro; // intro ������ ���� ofSoundPlayer ��ü
		ofSoundPlayer* white_sound; // �� �ǹ��� ������ ��� ofSoundPlayer�� �迭
		ofSoundPlayer* black_sound; // ���� �ǹ��� ������ ��� ofSoundPlayer�� �迭

		// �̹���
		//ofImage note_4;
		//ofImage note_8;
		//ofImage note_16;
		ofImage* notes;

		// perfect timing
		int num_of_perfect;
		int num_of_good;
		int num_of_fail;
		Note* score_front; // ��ǥ���� ���̸� �Ǻ��� �ȴ�. ��ǥ ���� ����Ʈ�� ���� ��ǥ�� �����ϴ� ������ ����.
		Note* score_rear; // ��ǥ���� ���� ����. 
};
