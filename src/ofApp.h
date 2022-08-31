#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h"
#include <string.h>
#include <time.h>

//typedef enum {F2, G2, A2, B2, C3, D3, E3, F3, G3, A3, B3, C4, D4, E4, F4, G4, A4, B4, C5, D5, E5} white_piano_key;
//typedef enum {Gb2, Ab2, Bb2, NOT1, Db3, Eb3, NOT2, Gb3, Ab3, Bb3, NOT3, Db4, Eb4, NOT4, Gb4, Ab4, Bb4, NOT5, Db5, Eb5} black_piano_key;
// ���� �ǹ�, �� �ǹݵ��� ������� ������ ������Ų �����̴�. ���� �ǹ��� NOTx����, 
// �ǾƳ뿡�� �� �ǹ��� �����ϳ� ���� �ǹ��� ���� ���� �����Ƿ� �� ���� ������ �� ���̴�.
// enum type ������ Ȱ���� �ڵ��� �������� ���̰� ������ �����ϰ� �Ͽ���.
// �ٵ� ��� ���ص� �ɵ�

static char* white_key_name[] = {"F2\n z", "G2\n x", "A2\n c", "B2\n v", "C3\n b", "D3\n n", "E3\n m", "F3\n ,", "G3\n .", "A3\n q", "B3\n w", "C4\n e", "D4\n r", "E4\n t", "F4\n y", "G4\n u", "A4\n i", "B4\n o", "C5\n p", "D5\n [", "E5\n ]"};
static char* black_key_name[] = { "Gb2\n  s", "Ab2\n  d", "Bb2\n  f", "", "Db3\n  h", "Eb3\n  j", "", "Gb3\n  l", "Ab3\n  1", "Bb3\n  2", "", "Db4\n  4", "Eb4\n  5", "", "Gb4\n  7", "Ab4\n  8", "Bb4\n  9", "", "Db5\n  -", "Eb5\n  =" };
// �ǹݿ� ���� ��µ� ���ڿ��̴�. �� �ǹݸ��� �ش��ϴ� ����, ������ �� Ű�� ���� �����ߴ�. ���� �ǹ��� ��� �߰��߰� �ǳʶپ�� �ϹǷ� �� ���ڿ��� ����־���.

static char* white_file_name[] = {"F2", "G2", "A2", "B2", "C3", "D3", "E3", "F3", "G3", "A3", "B3", "C4", "D4", "E4", "F4", "G4", "A4", "B4", "C5", "D5", "E5"};
static char* black_file_name[] = { "Gb2", "Ab2", "Bb2", "", "Db3", "Eb3", "", "Gb3", "Ab3", "Bb3", "", "Db4", "Eb4", "", "Gb4", "Ab4", "Bb4", "", "Db5", "Eb5"};
// �� ���� �ش��ϴ� sound file�� �ҷ��� �� �ڵ��� ���� ���̱� ���� �����Ͽ���. enum type�� Ȱ���� (ex: white_file_name[F2] = "F2") �ش��ϴ� ���� ���ϸ��� �� �� �ִ�

static char white_keyboard[] = "zxcvbnm,.qwertyuiop[]";
static char black_keyboard[] = "sdf hj l12 45 789 -=";
// key �Է��� ������ �� �ǹݿ� �ϴ��� �����Ǵ� ��� Ű�� Ȯ���ؾ� �ϴµ�, ������ for���� �޾��ָ� �ڵ� ���� �����ϰ� �Ҿ ���̴�.
// �̸� ���� �̸� �� Ű���� ���ڿ� �迭�� ������, 
// for loop�� �����ϰ� key�� �Է��� �˻��� �� �ִٴ� ������ �ִ�.


class Note { // ��ǥ �ڷᱸ��
public:
	int x; // �ǹ� �ε���: ��, ������ ���� �޶�����.
	float y; // ��ǥ�� y��ǥ
	short int len; // ��ǥ Ÿ�� = 4����ǥ, 8����ǥ, 16����ǥ �� �ϳ��̴�.
	short int key; // �� �ǹ�, ���� �ǹ� �� �ϳ��� �����ȴ�. ���� 0, ���� 1
	float speed; // ������ �ӵ�.
	float acce; // ���ӵ�
	short int is_calculated; // �� ��ǥ�� �����Ǿ����� ���θ� �Ǵ�. �Ǿ����� 1
	Note* prev; // ���� ��ǥ�� ����Ų��
	Note* next; // ���� ��ǥ�� ����Ų��
};
class result {
public:
	int perfect;
	int good;
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

		// ������ �Լ���
		void free_Memory(); // ���� ���� �޸𸮸� �����ϴ� �Լ�
		
		void play_piano_init(); // play_piano �� �ڷᱸ�� �ʱ�ȭ
		void piano_draw(); // �ǾƳ� �ǹݵ��� ȭ�鿡 �׷��ִ� ����.
		void activatedkey_draw(); // ���� ������ Ű���� ȭ�鿡 �׷���
		void print_window(); // ���α׷��� ���̴� ���ڿ��� ȭ�鿡 ���

		void sound_load(); // �ʿ��� sound file���� �ε����ִ� ����

		void image_load(); // �ʿ��� image���� load�ϴ� �Լ�

		void perfect_timing_init(int level); // perfect_timing ���� �ʱ�ȭ
		void perfect_timing_end(); // ���� ���� �� ���� �޸� ����
		void perfect_timing_gameover(); // ���ӿ��� �� ����Ǵ� �Լ�, ��� ���� �� ���Ͽ� ��� ���� ����
		void perfect_timing_gameover_draw(); // ���ӿ��� ȭ���� �׷��ִ� �Լ�
		void perfect_timing_draw(); // ������ ��ǥ���� �׷��ִ� �Լ�
		void calculate_timing(int key_type, int key_num); // ��ǥ�� �°� �󸶳� �� �������� Ÿ�̹��� �˻��ϴ� �Լ�

		
		void create_note(); // note ���� �� �����Ŵ
		void update_note(); // note�� ���¸� update
		void push_maxheap(result tempresult); // maxheap�� ���� ����
		result pop_maxheap(); // maxheap���� �ִ� ���� ��ȯ

		ofxWinMenu* menu; // Menu object
		void appMenuFunction(string title, bool bChecked); // Menu return function

		float windowWidth, windowHeight; 
		HWND hWnd; // Application window
		HWND hWndForeground; // current foreground window

		// Example menu variables
		//bool bShowInfo;
		bool bFullscreen;
		bool bTopmost;
		// Example functions
		void doFullScreen(bool bFull);
		void doTopmost(bool bTop);


		// �������� ������
		ofTrueTypeFont myFont1; // consil... ��� ���� ��Ʈ
		ofTrueTypeFont myFont2; // ���� ��Ʈ ó�� ��� ���
		ofTrueTypeFont myFont_white; // �� �ǹ��� ���� ������ �ϴ� Ű���� ��ư�� ������ֱ� ���� ��Ʈ
		ofTrueTypeFont myFont_black; // ���� �ǹ��� ���� ������ �ϴ� Ű���� ��ư�� ������ֱ� ���� ��Ʈ

		//int is_intro; // intro ���� loading�Ǿ����� �˷��ִ� ����. 1�̸� �ε� �� ��.
		int play_piano; // play_piano mode���� �˷��ִ� ����. 1�̸� play_piano�� ��
		int perfect_timing; // ���� ���� ������ ��Ÿ���� ����. 1�̸� �׷� ��
		int is_gameover; // ���ӿ��� ���θ� �˷��ִ� ����. 1�̸� ���ӿ���

		// �ǹ�
		int num_of_white; // �� �ǹ��� ����
		int num_of_black; // ���� �ǹ� + �� ������ ����
		float white_x; // �� �ǹ��� ��
		float white_y; // �� �ǹ��� ����
		float black_x; // ���� ��
		float black_y; // ���� ����
		float x1, x2, y1, y2; // �� �ǹ��� �簢�� ǥ��, ���� ���� ������ �Ʒ� �������� ��ǥ

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

		
		ofSoundPlayer intro; // intro ������ ���� ofSoundPlayer class
		ofSoundPlayer* white_sound; // �� �ǹ��� ������ ��� ofSoundPlayer�� �迭
		ofSoundPlayer* black_sound; // ���� �ǹ��� ������ ��� ofSoundPlayer�� �迭
		ofSoundPlayer gamestart; // perfect timing ���� �� ���� ȿ����
		ofSoundPlayer gameover; // perfect timing ���ӿ��� �� ������ ���� �������

		// �̹���
		//ofImage note_4;
		//ofImage note_8;
		//ofImage note_16;
		ofImage* notes; // ��ǥ �̹������� ��� �迭

		// perfect timing
		int num_of_perfect; // ���� perfect ������ ����
		int num_of_good; // ���� good ������ ����
		int num_of_fail; // ���� fail ������ ����
		int max_num_of_fail; // ���Ǵ� fail�� ����, �� ���� 0�� �Ǹ� ���ӿ���.
		Note* score_front; //  ��ǥ ���� ����Ʈ�� dummy front.
		Note* score_rear; // ���� ����Ʈ�� dummy rear. 
		result* maxheap; // ����� �ε��� ������ �����ϴ� �迭 ��� max heap
		result result_print[5]; // ���� ���� �� 5���� ���� ����� ȭ�鿡 draw �� ���
		int maxheap_len; // heap ����
};