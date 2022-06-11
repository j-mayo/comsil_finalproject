#include "ofApp.h"
#include "ofxWinMenu.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//sound_F2.load("sounds/sample.mp3");
	
	//sound_F2.setMultiPlay(true);
	
	// 기본 설정. frame rate, title, 색, 메뉴 등을 설정해준다.
	srand(time(NULL));
	ofSetWindowTitle("Piano Playtime!");
	ofSetFrameRate(30);
	ofBackground(255, 255, 255);

	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	

	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	// new menu object!
	menu = new ofxWinMenu(this, hWnd);

	menu->CreateMenuFunction(&ofApp::appMenuFunction);


	// 앞으로 사용할 font를 load
	myFont1.loadFont("verdana.ttf", 12, true, true);
	myFont2.loadFont("verdana.ttf", 20, true, true);
	myFont_white.loadFont("frabk.ttf", 20, true, true);
	myFont_black.loadFont("frabk.ttf", 15, true, true);

	// window menu 생성
	HMENU hMenu = menu->CreateWindowMenu();

	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");
	// Exit 전에는 구분 줄을 넣어준다
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	// mode 메뉴
	hPopup = menu->AddPopupMenu(hMenu, "Mode");
	menu->AddPopupItem(hPopup, "Play Piano", false, false);
	menu->AddPopupItem(hPopup, "Perfect timing!(easy)", false, false);
	menu->AddPopupItem(hPopup, "Perfect timing!(hard)", false, false);
	menu->AddPopupItem(hPopup, "Perfect timing!(comsil)", false, false);

	// Mode ( if possible )
	/*
	hPopup = menu->AddPopupMenu(hMenu, "Mode");
	menu->AddPopupItem(hPopup, "Play instrument");
	menu->AddPopupItem(hPopup, "Play with a song");
	*/
	// 보기 메뉴
	hPopup = menu->AddPopupMenu(hMenu, "View");
	menu->AddPopupItem(hPopup, "Full screen", false, false);
	
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false);

	menu->SetWindowMenu();
	

	// 처음엔 기본적으로 play_piano 모드로 시작한다.
	play_piano_init();

	// sound part;
	//white_sound_flag = new int[14];
	//black_sound_flag = new int[13];
	sound_load();
	
	// image part
	image_load();
	
}

void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	if (title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	if (title == "Exit") {
		ofExit();
	}
	//
	// Help menu
	//

	if (title == "About") {
		// 설명을 붙인다. 일단 아무 말이나 쓰긴 했는데 그렇게 중요한 부분도 아니니 몇 마디만 썼다.
		ofSystemAlertDialog("Piano Playtime! Enjoy your own keyboard-piano!\nMade by SJY\n\nmofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

	if (title == "Play Piano") {
		// play_piano = 1; // 기본적으로 1로 설정되어 있긴 하다.
		// perfect_timing = 0; // 게임은 멈춘다.
		if (perfect_timing > 0) perfect_timing_end();
		play_piano_init();
	}
	if (title == "Perfect timing!(easy)") {
		if (perfect_timing > 0) perfect_timing_end();
		perfect_timing_init(1);
	}
	if (title == "Perfect timing!(hard)") {
		if (perfect_timing > 0) perfect_timing_end();
		perfect_timing_init(2);
	}
	if (title == "Perfect timing!(comsil)") {
		if (perfect_timing > 0) perfect_timing_end();
		perfect_timing_init(3);
	}
	// mode에 따라 추가 예정

} // end appMenuFunction

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	if (perfect_timing) perfect_timing_draw(); // perfect timing이 진행 중인 경우, 판정을 위해 피아노보다 먼저 그려준다.

	//display.draw(ofGetWidth() / 6, 40, ofGetWidth() * 2 / 3, ofGetHeight() / 2);
	piano_draw();
	// 먼저 피아노를 그려야 한다.
	
	// 다음으로. 눌린 건반을 표시해준다..
	activatedkey_draw();

	// print the string in window
	print_window();
	// 글자는 항상 마지막에 draw되어야 사용자가 볼 수 있다.
	
	

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// full screen 에 대한 부분!
	//printf("%c\n", key);
	if (key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if (bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
			free_Memory();
		}
	}
	/*
	if (key == 'f') {
		bFullscreen = !bFullscreen;
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}
	*/

	if (play_piano == 1) {
		if (key == ' ') { // sustain pedal이 눌린 경우, 현재 눌린 음을 계속 지속해야 한다. 이를 위해 따로 표시한다.
			sustain_pedal = 1;
		}

		// sustain pedal이 눌린 경우, 현재 눌린 건반들에 대해 sustain_flag를 설정해 줘야 하므로 이 부분을 실행.
		if (sustain_pedal == 1) {
			// 현재 눌린 건반들에 대해, sustain_flag를 1로 설정해 눌려있다는 것을 알린다.
			for (int i = 0; i < num_of_white; i++) {
				if (white_keypressedflag[i] == 1) {
					white_sustainflag[i] = 1;
				}
			}
			for (int i = 0; i < num_of_black; i++) {
				// if (i == 3 || i == 6 || i == 10 || i == 13 || i == 17) continue;
				if (black_keypressedflag[i] == 1) {
					black_sustainflag[i] = 1;
				}
			}
		}

		for (int i = 0; i < num_of_white; i++) {
			// 입력으로 들어온 key가 눌리지 않은 상태라면 음을 재생한다.

			//if (play_piano && key == white_keyboard[i] && white_keypressedflag[i] != 1) {
			if (key == white_keyboard[i]) {
				if (white_sustainflag[i] == 1) {
					white_keypressedflag[i] = 0;
					white_sound[i].stop();
				}
				if (white_keypressedflag[i] != 1) {
					white_keypressedflag[i] = 1; // 이미 눌린 상태에서 음을 재생 시 중첩되어 귀가 아플 수 있다
					white_sound[i].play();
				}
				if (sustain_pedal == 1) white_sustainflag[i] = 1;
				// pedal을 먼저 누른 뒤, 음을 누를 경우 꾹 누르는 키 입력이 끊긴다.
				// 이는 아무래도 openFramework key 인식 또는 컴퓨터 키보드 상의 구조적 문제로 보인다.
				// 따라서 pedal을 누른 뒤 눌린 상태에서 또 다른 음을 누른 경우, 페달 중이라고 표시해 줘야 하므로 sustain_flag[i]를 1로 설정. 밑의 검은 건반에서도 마찬가지다.
			}
		}

		for (int i = 0; i < num_of_black; i++) {
			if (i == 3 || i == 6 || i == 10 || i == 13 || i == 17) continue;
			// 검은 건반이 없는 위치에 대해서는 key 입력을 받지 않아야 한다.
			//if (play_piano && key == black_keyboard[i] && black_keypressedflag[i] != 1) {
			if (key == black_keyboard[i]) {
				if (black_sustainflag[i] == 1) {
					black_keypressedflag[i] = 0;
					black_sound[i].stop();
				}
				if (black_keypressedflag[i] != 1) { // 이미 눌린 상태에서 음을 재생 시 중첩되어 귀가 아플 수 있다
					black_keypressedflag[i] = 1;
					black_sound[i].play();
				}
				if (sustain_pedal == 1) black_sustainflag[i] = 1;
			}
		}
	}
	
	if (perfect_timing > 0) { // perfect timing 게임에선 sustain pedal을 활성화하지 않는다.
		for (int i = 0; i < num_of_white; i++) {
			if (key == white_keyboard[i]) {
				if (white_keypressedflag[i] != 1) {
					white_keypressedflag[i] = 1; 
					white_sound[i].play();
				}
				calculate_timing(0, i); // 역시 위와 같이 재생은 하고, 현재 음표들이 내려오는 것과 비교해준다.

			}
		}

		for (int i = 0; i < num_of_black; i++) {
			if (i == 3 || i == 6 || i == 10 || i == 13 || i == 17) continue;
			if (key == black_keyboard[i]) {
				if (black_keypressedflag[i] != 1) { 
					black_keypressedflag[i] = 1;
					black_sound[i].play();
				}
				calculate_timing(1, i); // 역시 위와 같이 재생은 하고, 현재 음표들이 내려오는 것과 비교해준다.
			}
		}

	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	if (play_piano) {
		if (key == ' ') {
			// sustain pedal에서 손을 뗀 경우, pedal로 인해 지속 중인 모든 음을 멈춰야 한다.
			sustain_pedal = 0;
			// 스페이스바를 떼면 sustain중이였던 모든 key들이 눌리지 않은 상태로 변한다.
			for (int i = 0; i < num_of_white; i++) {
				if (white_sustainflag[i] == 1) {
					white_keypressedflag[i] = 0;
					white_sustainflag[i] = 0;
					white_sound[i].stop();

				}
			}

			for (int i = 0; i < num_of_black; i++) {
				if (i == 3 || i == 6 || i == 10 || i == 13 || i == 17) continue;
				if (black_sustainflag[i] == 1) {
					black_keypressedflag[i] = 0;
					black_sustainflag[i] = 0;
					black_sound[i].stop();
				}
			}
		}
		if (sustain_pedal == 0) {
			// sustain_pedal이 눌린 상태라면 음을 종료해서는 안 되므로 조건에 추가한다.
			for (int i = 0; i < num_of_white; i++) {
				if (key == white_keyboard[i] && white_keypressedflag[i] == 1) {
					white_keypressedflag[i] = 0;
					white_sound[i].stop();
				}
			}

			for (int i = 0; i < num_of_black; i++) {
				if (i == 3 || i == 6 || i == 10 || i == 13 || i == 17) continue;
				if (key == black_keyboard[i] && black_keypressedflag[i] == 1) {
					black_keypressedflag[i] = 0;
					black_sound[i].stop();
				}
			}
		}
	}

	if (perfect_timing > 0) { // perfect timing 게임에선 sustain pedal을 활성화하지 않는다.
		// 여기선 음을 바로 끊지 않고, 그냥 keypressedflag만 0으로 설정해 그림은 안 그려지되, 음이 자연스럽게 끊기도록 한다.
		for (int i = 0; i < num_of_white; i++) {
			if (key == white_keyboard[i]) {
				if (white_keypressedflag[i] == 1) {
					white_keypressedflag[i] = 0;
				}
			}
		}

		for (int i = 0; i < num_of_black; i++) {
			if (i == 3 || i == 6 || i == 10 || i == 13 || i == 17) continue;
			if (key == black_keyboard[i]) {
				if (black_keypressedflag[i] == 1) {
					black_keypressedflag[i] = 0;
				}
			}
		}

	}
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if (bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else {
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU));
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth() - ofGetWidth()) / 2, (ofGetScreenHeight() - ofGetHeight()) / 2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if (bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if (bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if (GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
} // end doTopmost


void ofApp::free_Memory() {
	// free the allocated memory
	delete[] white_keypressedflag;
	delete[] black_keypressedflag;
	delete[] white_sustainflag;
	delete[] black_sustainflag;
	delete[] white_sound;
	delete[] black_sound;
	delete[] notes;
	if (score_front) {
		Note* temp = score_front->next;
		Note* delnote;
		for (; temp != score_rear; ) {
			delnote = temp;
			temp = temp->next;
			delnote->prev->next = temp;
			temp->prev = delnote->prev;
			delete delnote;
		}

		delete score_front;
		delete score_rear;
	}
}

void ofApp::play_piano_init() {
	play_piano = 1;
	perfect_timing = 0;
	// key part
	// 현재 건반의 개수는 흰색 21개, 검은색 15개로 총 36개이다. 왜 검은색이 20개이냐, 그것은 바로 검은 건반은 연속하지 않으므로 구현 시의 효율성을 위해 20개로 했다.
	num_of_white = 21;
	num_of_black = 20;
	// 필요한 자료구조들을 동적 할당 해 준다.
	if (!white_keypressedflag) white_keypressedflag = new int[num_of_white];
	if (!black_keypressedflag) black_keypressedflag = new int[num_of_black];
	if (!white_sustainflag) white_sustainflag = new int[num_of_white];
	if (!black_sustainflag) black_sustainflag = new int[num_of_black];
	
	// 자료구조를 초기화한다.
	for (int i = 0; i < num_of_white; i++) {
		white_keypressedflag[i] = 0;
		white_sustainflag[i] = 0;
	}
	for (int i = 0; i < num_of_black; i++) {
		black_keypressedflag[i] = 0;
		black_sustainflag[i] = 0;
	}
	// sustain_pedal은 초기에 눌리지 않으므로 0으로 초기화.
	sustain_pedal = 0;
}

void ofApp::piano_draw() {
	char str[200];

	// 화면에 나올 건반 길이를 설정
	white_x = (float)((ofGetWidth() - 20) / num_of_white); // 좌우 여유 10px
	white_y = (float)((ofGetHeight() - 20) / 2.5); // 하단 여유 20px
	black_x = (float)white_x / 2;
	black_y = (float)white_y * 3 / 5;
	// 눌렸을 때 표시될 원을 그릴 반지름 설정
	white_r = white_x / 3;
	black_r = black_x / 3;

	// 피아노 전체 사각형 꼭짓점 설정
	x1 = 10;
	x2 = ofGetWidth() - 10;
	y2 = ofGetHeight() - 100 - 20;
	y1 = y2 - white_y;
	ofSetColor(ofColor::white);
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofSetLineWidth(5);
	// 전체적인 틀 사각형 그림
	ofFill();
	ofDrawRectangle(x1, y1, x2 - x1 - 10, y2 - y1); // 여기서 그리는 건, perfect timiing 게임 시 음표가 피아노 밑으로 가는 걸 가리기 위함.
	ofSetColor(ofColor::black);
	ofNoFill();
	ofDrawRectangle(x1, y1, x2 - x1 - 10, y2 - y1); // 모양 맞추기 위한 -10
	float tempx = x1 + white_x;
	// 흰 건반 그림
	for (int i = 0; i < num_of_white - 1; i++) {
		ofDrawLine(tempx, y1, tempx, y2);
		tempx += white_x;
	}
	// 검은 건반 그림
	tempx = x1 + white_x * 3 / 4;
	ofFill();
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 7; j++) {
			if (j == 3 || j == 6) {
				tempx += white_x;
				continue;
			}
			ofDrawRectangle(tempx, y1, black_x, black_y);
			tempx += white_x;
		}
	}
	// draw the space bar(sustain pedal을 그린 것)
	ofNoFill();
	ofDrawRectangle(ofGetWidth() / 2 - 3 * white_x, y2 + 20, 6 * white_x, 70);
}

void ofApp::activatedkey_draw() {
	// if key activatied, then the window highlight that key

	// 눌린 건반에 색을 칠해 사용자가 눌렸음을 알 수 있게 한다.
	ofFill();
	ofSetColor(ofColor::lightSalmon);
	for (int i = 0; i < num_of_white; i++) { // 흰 건반에 대함
		if (white_keypressedflag[i] == 1) {
			ofDrawCircle(x1 + white_x / 2 + i * white_x, y2 - white_y / 6, white_r); // 적절히 좌표를 계산해 보기 좋게 출력한다.
		}
	}

	for (int i = 0; i < num_of_black; i++) { // 검은 건반에 대함
		if (black_keypressedflag[i] == 1) { // black_key의 경우 중간중간 없는 건반이 있지만 이것들은 절대 눌릴 염려가 없어 따로 처리하지 않았다.
			ofDrawCircle(x1 + white_x * (i + 1), y2 - white_y / 2 - white_y / 60, black_r); // 적절히 좌표를 계산해 보기 좋게 출력한다.
		}
	}
	if (sustain_pedal) ofDrawRectangle(ofGetWidth() / 2 - 2 * white_x, y2 + 25, 4 * white_x, 50); // sustain pedal의 경우 사각형을 표시했다ㅏ.
}

void ofApp::print_window() {
	char str[200]; // myfont1에서 화면에 문장을 나타내기 위한 str 버퍼이다

	ofSetColor(ofColor::darkGray);
	sprintf(str, " 20181277_ShinJaeyo_comsil_final_project ");
	myFont1.drawString(str, 15, ofGetHeight() - 5);

	// 흰 건반에 글자를 출력
	ofSetColor(ofColor::black);
	// print the white key name
	for (int i = 0; i < num_of_white; i++) {
		//sprintf(str, white_key_name[i]);
		myFont_white.drawString(white_key_name[i], x1 + white_x * 1 / 3 + i * white_x, y2 - white_y / 7);
		// 헤더에서 선언한 white_key_name으로 for문을 사용해 효율적으로 글자를 print한다.
	}
	myFont_white.drawString("Sustain pedal(Space bar)", ofGetWidth() / 2 - 1.5 * white_x, ofGetHeight() - 60);
	ofSetColor(ofColor::white);
	// print the black key name
	for (int i = 0; i < num_of_black; i++) {
		//sprintf(str, black_key_name[i]);
		myFont_black.drawString(black_key_name[i], x1 + white_x * 13 / 16 + i * white_x, y2 - white_y / 2);
		// 헤더에서 선언한 black_key_name으로 for문을 사용해 효율적으로 글자를 print한다.
		// 검은 건반이 없는 위치의 black_key_name은 빈 문자열이니 아무런 문제가 없다. 하나의 for문으로 깔끔하게 구현이 가능했다.
	}

	if (perfect_timing > 0) {
		ofSetColor(ofColor::forestGreen);
		sprintf(str, "PERFECT: %d / GOOD; %d / FAIL: %d", num_of_perfect, num_of_good, num_of_fail);
		myFont2.drawString(str, ofGetWidth() / 2 + 4 * white_x, ofGetHeight() - 50);
	}
}
void ofApp::sound_load() {
	// 필요한 sound file을 load하는 함수
	is_intro = intro.load("sounds/intro.mp3"); // loading 성공 시 1, 아니면 0
	intro.play();


	// 배열 할당
	white_sound = new ofSoundPlayer[num_of_white];
	black_sound = new ofSoundPlayer[num_of_black];
	// 파일 확장자 설정. 필요에 의해 바꿀 수 있다.
	char extension[] = ".wav";
	// 건반 이름과 키를 화면에 출력할 때 쓰는 문자열 버퍼
	char* temps = (char*)malloc(sizeof(int) * 20);
	
	// 헤더에서 선언한 white_file_name, black_file_name과 strcpy, strcat 함수로 경로를 인자로 전달해 wav 파일을 load한다.
	for (int i = 0; i < num_of_white; i++) {
		strcpy(temps, "sounds/");
		strcat(temps, white_file_name[i]);
		strcat(temps, extension);
		white_sound[i].load(temps);
		white_sound[i].setMultiPlay(false); // 키를 꾹 누를 경우 음이 반복 재생되는 일이 없도록 setMultiplay(false) 처리한다.
		//printf("%s\n", temps);
	}

	for (int i = 0; i < num_of_black; i++) {
		strcpy(temps, "sounds/");
		strcat(temps, black_file_name[i]);
		strcat(temps, extension);
		black_sound[i].load(temps);
		black_sound[i].setMultiPlay(false);
		//printf("%s\n", temps);
	}
	//white_sound[F2].load("sounds/sample.mp3");
}

void ofApp::image_load() {
	notes = new ofImage[3];
	notes[0].load("images/Note_4.PNG");
	notes[1].load("images/Note_8.PNG");
	notes[2].load("images/Note_16.PNG");
}

void ofApp::perfect_timing_init(int level) {
	play_piano = 0;
	perfect_timing = 1;
	num_of_perfect = 0;
	num_of_good = 0;
	num_of_fail = 0;
	if (!score_front) {
		score_front = new Note; // dummy note
		score_rear = new Note; // dummy note
		score_front->next = score_rear;
		score_rear->prev = score_front;
		
	}
	for (int i=0; i<level; i++) create_note(); // level에 따라 떨어지는 음의 개수가 늘어남.

}

void ofApp::perfect_timing_end() {
	// 끝날 시 일시적으로 할당된 메모리를 해제. free memory와 구분한 이유는 free-memory에서 해제하는 메모리는 
	// 프로그램 전체의 메모리이다. play_piano에서 할당한 메모리의 경우 처음에만 할당하고 따로 재할당할 필요가 없으나(피아노를 쳐도 메모리 구조는 안 변함).
	// perfect_timing 시에는 연결 리스트를 사용하기 때문에 note 생성 시마다 연결하며 메모리가 변하므로,
	// 실행할 때 초기화를 하지 않으면 꼬일 수 있다. 따라서  초기 상태로 메모리를 만들어야 하므로, 
	// 끝날 시 메모리 누수가 없게 할당한 것을 해제.
	if (score_front) {
		Note* temp = score_front->next;
		Note* delnote;
		for (; temp != score_rear; ) {
			delnote = temp;
			temp = temp->next;
			delnote->prev->next = temp;
			temp->prev = delnote->prev;
			delete delnote;
		}
	}
	perfect_timing = 0;
}

void ofApp::create_note() {
	// note를 생성해서 연결 리스트에 넣는다.
	Note* temp = new Note;
	temp->key = rand() % 2;
	int t;
	t = rand() % 3;
	temp->len = t;
	temp->speed = 4;
	temp->acce = (0.1 + (float)(rand() % 10) / 100) * (t + 1);
	if (temp->key == 0) { // 흰 건반이 0
		temp->x = rand() % 21;
	}
	else {
		while (1) {
			t = rand() % 20;
			if (t == 3 || t == 6 || t == 10 || t == 13 || t == 17) continue; // 검은 건반은 빈 곳이 있다!
			temp->x = t;
			break;
		}
	}
	temp->y = 0;
	temp->is_calculated = 0;
	temp->prev = score_rear->prev;
	temp->next = score_rear;
	score_rear->prev->next = temp;
	score_rear->prev = temp;
}

void ofApp::update_note() {

	Note* temp = score_front->next; // front는 dummy
	Note* delnote;
	for (temp; temp != score_rear; ) {
		temp->y += temp->speed;
		temp->speed += temp->acce;
		int ty;
		if (temp->y > y1 + (temp->speed * 6)) { //y1이 현재 속력 기준으로 6프레임 후 이동거리보다 밑에 있을 시 그 노트는 사라진다.
			delnote = temp;
			if (temp->is_calculated == 0) num_of_fail++; // 무시된 음표의 경우 fail로 결정
			create_note();
			temp = temp->next;
			if (delnote->prev) delnote->prev->next = temp;
			temp->prev = delnote->prev;
			delete delnote;
			//cout << temp->x << endl;
		}
		else temp = temp->next;
	}

}

void ofApp::perfect_timing_draw() {
	//ofSetColor(255, 255, 255, 0);
	//cout << score_front->x << " / " << score_front->y << endl;
	//cout << score_front->key << " len: " << score_front->len << endl;
	//ofSetColor(ofColor::grey);
	Note* temp = score_front->next;
	for (temp; temp != score_rear; temp = temp->next) {
		if (temp->key == 0) { //흰 건반일 때
			ofNoFill();
			ofSetColor(ofColor::black);
			ofDrawRectangle(temp->x * white_x + x1 + black_x / 2, temp->y, black_x, 50);
			ofFill();
			ofSetColor(ofColor::white);
			notes[temp->len].draw(temp->x * white_x + x1 + black_x / 2, temp->y, black_x, 50);
			//ofDrawCircle(score_front->x * white_x + x1 + black_x, score_front->y, black_x);
			//update_note();
		}
		else {
			ofSetColor(ofColor::grey);
			ofFill(); // 검은 건반은 회색!
			//notes[score_front->len].draw(0, 0);
			notes[temp->len].draw(temp->x * white_x + x1 + white_x * 3 / 4, temp->y, black_x, 50);
			//ofDrawCircle(score_front->x * white_x + x1 + white_x * 7 / 8, score_front->y, black_x);
			//update_note();
		}
	}
	update_note();

}

void ofApp::calculate_timing(int key_type, int key_num) {
	Note* temp = score_front->next;
	int is_fail = 0; // input key가 현재 내려오는 노트들 중 어느 것에도 맞지 않는다면 잘못 누른 것 -> 그 경우 fail을 하나 올리기로 했다.
	for (int i = 0; i < perfect_timing; i++, temp = temp->next) {
		if (temp->is_calculated == 1) continue; // 이미 눌려서 점수가 갱신되었다면 이건 패스
		if (key_type == temp->key) { // 건반 색깔 유무를 맞춰야 함.
			if (key_num == temp->x) {
				is_fail = 1;
				temp->is_calculated = 1;
				// 판정의 기준: 평균 속력을 내서 프레임별 이동 거리 계산, 2프레임 내부일 시 perfect, 6프레임 내부일 시 good, 그 이하 fail
				float perfect_range = (temp->speed + temp->acce) * 3;
				float good_range = (temp->speed + 3 * temp->acce) * 6;
				// y1은 흰 건반의 높이, 즉 음표의 판정 부분과 같다!
				if (abs(temp->y + 50 - y1) < perfect_range) num_of_perfect++; // 50은 내려오는 음표 박스의 길이임.
				else if (abs(temp->y + 50 - y1) < good_range) num_of_good++;
				else num_of_fail++;
				break; // 한 번에 하나씩만 처리해야.
			}
		}
	}
	if (!is_fail) num_of_fail++; // 입력과 현재 떨어지는 음표들이 맞지 않은 경우: fail++
}