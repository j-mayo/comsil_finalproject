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

	// 현재 건반의 개수는 흰색 21개, 검은색 15개로 총 36개이다. 왜 검은색이 20개이냐, 그것은 바로 검은 건반은 연속하지 않으므로 구현 시의 효율성을 위해 20개로 했다.
	num_of_white = 21;
	num_of_black = 20;

	// 화면에 나올 건반 길이를 설정
	white_x = (float)((ofGetWidth() - 20) / num_of_white); // 좌우 여유 10px
	white_y = (float)((ofGetHeight() - 20) / 2.5); // 하단 여유 20px
	black_x = (float)white_x / 2;
	black_y = (float)white_y * 3 / 5;
	// 눌렸을 때 표시될 원을 그릴 반지름 설정
	white_r = white_x / 3;
	black_r = black_x / 3;

	// 필요한 자료구조들을 동적 할당
	white_keypressedflag = new int[num_of_white];
	black_keypressedflag = new int[num_of_black];
	white_sustainflag = new int[num_of_white];
	black_sustainflag = new int[num_of_black];
	white_sound = new ofSoundPlayer[num_of_white];
	black_sound = new ofSoundPlayer[num_of_black];
	notes = new ofImage[3];

	score_front = new Note; // dummy note
	score_rear = new Note; // dummy note
	score_front->next = score_rear;
	score_rear->prev = score_front;


	// 앞으로 사용할 font를 load
	myFont1.loadFont("verdana.ttf", 12, true, true);
	myFont2.loadFont("frabk.ttf", 20, true, true);
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
	
	// sound part;
	//white_sound_flag = new int[14];
	//black_sound_flag = new int[13];
	sound_load();
	intro.play();
	// image part
	image_load();
	
	// 처음엔 기본적으로 play_piano 모드로 시작한다.
	play_piano_init();
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
		if (perfect_timing > 0) perfect_timing_end(); // 게임 중 종료 시 메모리 해제
		free_Memory();
		ofExit();
		
	}
	//
	// Help menu
	//

	if (title == "About") {
		// 설명을 붙인다. 
		ofSystemAlertDialog("Piano Playtime! Enjoy your own keyboard-piano!\nMade by SJY\n\nmofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

	if (title == "Play Piano") {
		// play_piano = 1; // 기본적으로 1로 설정되어 있긴 하다.
		// perfect_timing = 0; // 게임은 멈춘다.
		if (perfect_timing > 0) perfect_timing_end();
		play_piano_init();
	}
	// 각 난이도에 따른 게임을 선택 시, 난이도에 따라 게임 init 함수 실행
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
	// 현재 게임 중이고 게임오버가 되지 않았다면 update_note() 함수를 실행해
	// 떨어지는 음표들을 갱신
	if (perfect_timing > 0 && !is_gameover) {
		update_note();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	//cout << 0 << endl;
	if (perfect_timing && !is_gameover) perfect_timing_draw(); 
	// perfect timing이 진행 중인 경우, 판정을 위해 피아노보다 먼저 그려준다.
	// piano의 사각형 범위는 OfFill인 상태로 그려지므로, 음표들이 피아노에 흡수되는 효과를 낼 수 있다.
	
	//display.draw(ofGetWidth() / 6, 40, ofGetWidth() * 2 / 3, ofGetHeight() / 2);
	piano_draw();
	// 먼저 피아노를 그려야 한다.
	
	// 다음으로. 눌린 건반을 표시해준다..
	activatedkey_draw();

	// print the string in window
	print_window();
	// 글자는 항상 마지막에 draw되어야 사용자가 볼 수 있다.
	
	// 게임 중 게임오버인 경우, 게임오버 화면을 그려주는 함수를 실행.
	if (is_gameover == 1) {
		perfect_timing_gameover_draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// full screen 에 대한 부분!
	
	if (key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if (bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			if (perfect_timing > 0) perfect_timing_end(); // 게임 중 종료 시 메모리 해제
			free_Memory();
			ofExit();
			
		}
	}

	if (is_gameover == 1) { // 게임오버 시, esc를 제외한 아무 키나 누른다면 게임을 처음부터 시작!
		if (key != VK_ESCAPE) {
			//is_gameover = 0;
			gameover.stop();
			int temp = perfect_timing; // gameover 후 이어할 시 난이도는 보존된다.
			perfect_timing_end(); // 랭킹 저장을 위한 end
			perfect_timing_init(temp); // 게임 시작
			//perfect_timing_init(max_num_of_fail / 15); // 게임 다시 시작, 게임오버 당시의 레벨로 다시 시작
			return;
		}
	}
	if (play_piano == 1) {
		if (key == ' ') { // sustain pedal이 눌린 경우, 현재 눌린 음을 계속 지속해야 한다. 이를 위해 따로 표시한다.
			sustain_pedal = 1;
		}

		// sustain pedal이 눌린 경우, 현재 눌린 건반들에 대해 sustain_flag를 설정해 줘야 하므로 이 부분을 실행.
		if (sustain_pedal == 1) {
			// for문으로 모든 건반에 대해 검사
			// "현재 눌린 상태인 건반들"에 대해, sustain_flag를 1로 설정해 눌려있다는 것을 알린다.
			for (int i = 0; i < num_of_white; i++) {
				if (white_keypressedflag[i] == 1) {
					white_sustainflag[i] = 1;
				}
			}
			for (int i = 0; i < num_of_black; i++) {
				if (i == 3 || i == 6 || i == 10 || i == 13 || i == 17) continue;
				// 검은 건반이 빈 위치의 인덱스는 확인하지 않는다.
				// 사실 해당 위치에는 sound 파일이 없기 떄문에 이 문이 없어도 무방하다.
				if (black_keypressedflag[i] == 1) {
					black_sustainflag[i] = 1;
				}
			}
		}
		// for문을 통해 모든 건반에 대해 입력 키와 비교하며 검사
		for (int i = 0; i < num_of_white; i++) {
			// 입력으로 들어온 key가 눌리지 않은 상태라면 음을 재생한다.

			//if (play_piano && key == white_keyboard[i] && white_keypressedflag[i] != 1) {
			if (key == white_keyboard[i]) {
				if (white_sustainflag[i] == 1 && white_keypressedflag[i] != 1) {
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
				if (black_sustainflag[i] == 1 && black_keypressedflag[i] != 1) {
					//black_keypressedflag[i] = 0;
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
		// perfect timing 게임 중 반복문으로 모든 건반을 검사해서, 입력 키와 맞는 건반이 있다면 음을 재생하고 calculate_timing 함수를 호출한다
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
void ofApp::keyReleased(int key) {

	if (play_piano == 1) {
		if (key == ' ') {
			// sustain pedal에서 손을 뗀 경우, pedal로 인해 지속 중인 모든 음을 멈춰야 한다.
			sustain_pedal = 0;
			// 스페이스바를 떼면 sustain중이였던 모든 key들이 눌리지 않은 상태로 변한다.
			// 반복문을 통해 모든 건반을 검사해서 sustain 중이고 누르고 있지 않은 건반들의 flag를 설정하고 음을 멈춰준다.
			for (int i = 0; i < num_of_white; i++) {
				if (white_sustainflag[i] == 1 && white_keypressedflag[i] != 1) {
					//white_keypressedflag[i] = 0;
					white_sustainflag[i] = 0;
					white_sound[i].stop();

				}
			}

			for (int i = 0; i < num_of_black; i++) {
				if (i == 3 || i == 6 || i == 10 || i == 13 || i == 17) continue;
				if (black_sustainflag[i] == 1 && black_keypressedflag[i] != 1) {
					//black_keypressedflag[i] = 0;
					black_sustainflag[i] = 0;
					black_sound[i].stop();
				}
			}
		}
		// 반복문을 통해 모든 건반을 검사해서, released된 키보드에 대응하는 건반이 있고
		// 그 건반이 눌린 상태일 경우 건반의 flag를 설정하고, 페달을 누르지 않았을 경우 음을 멈춘다.
		for (int i = 0; i < num_of_white; i++) {
			//if (key == white_keyboard[i] && white_keypressedflag[i] == 1) {
			if (key == white_keyboard[i] && white_keypressedflag[i] == 1) {
				white_keypressedflag[i] = 0;
				if (sustain_pedal == 0) white_sound[i].stop();
			}
		}

		for (int i = 0; i < num_of_black; i++) {
			if (i == 3 || i == 6 || i == 10 || i == 13 || i == 17) continue;
			if (key == black_keyboard[i] && black_keypressedflag[i] == 1) {
				black_keypressedflag[i] = 0;
				if (sustain_pedal == 0) black_sound[i].stop();
			}
		}
	}


		if (perfect_timing > 0) { // perfect timing 게임에선 sustain pedal을 활성화하지 않는다.
			// 반복문을 통해 모든 건반을 검사해서 떼어진 건반을 찾는다.
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
		// double linked list에 저장된 모든 음표 객체의 메모리를 해제한다.
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
	// sustain_pedal은 초기에 눌리지 않으므로 0으로 초기화.
	sustain_pedal = 0;
	// key part
	
	// 자료구조를 초기화한다.
	for (int i = 0; i < num_of_white; i++) {
		white_keypressedflag[i] = 0;
		white_sustainflag[i] = 0;
	}
	for (int i = 0; i < num_of_black; i++) {
		black_keypressedflag[i] = 0;
		black_sustainflag[i] = 0;
	}
	
}

void ofApp::piano_draw() {
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
	// 흰 건반 그림 - loop 사용
	for (int i = 0; i < num_of_white - 1; i++) {
		ofDrawLine(tempx, y1, tempx, y2);
		tempx += white_x;
	}
	// 검은 건반 그림 - loop 사용
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
	for (int i = 0; i < num_of_white; i++) { // 흰 건반에 대해서 반복문을 사용해 그려줌
		//if (white_keypressedflag[i] == 1 || white_sustainflag[i] == 1) {
		if (white_keypressedflag[i] == 1) {
			ofDrawCircle(x1 + white_x / 2 + i * white_x, y2 - white_y / 6, white_r); // 적절히 좌표를 계산해 보기 좋게 출력한다.
		}
	}

	for (int i = 0; i < num_of_black; i++) { // 검은 건반에 대해 반복문을 사용해 그려줌
		// if (black_keypressedflag[i] == 1 || black_sustainflag[i] == 1) { // black_key의 경우 중간중간 없는 건반이 있지만 이것들은 절대 눌릴 염려가 없어 따로 처리하지 않았다.
		if (black_keypressedflag[i] == 1){
			ofDrawCircle(x1 + white_x * (i + 1), y2 - white_y / 2 - white_y / 60, black_r); // 적절히 좌표를 계산해 보기 좋게 출력한다.
		}
	}
	if (sustain_pedal) ofDrawRectangle(ofGetWidth() / 2 - 2 * white_x, y2 + 25, 4 * white_x, 50); // sustain pedal의 경우 사각형을 표시했다ㅏ.
}

void ofApp::print_window() { // string을 화면에 띄워주는 함수
	char str[200]; // myfont1에서 화면에 문장을 나타내기 위한 str 버퍼이다

	ofSetColor(ofColor::darkGray);
	sprintf(str, " 20181277_ShinJaeyo_comsil_final_project ");
	myFont1.drawString(str, 15, ofGetHeight() - 5);

	// 반복문을 사용해 흰 건반에 글자를 출력
	ofSetColor(ofColor::black);
	for (int i = 0; i < num_of_white; i++) {
		//sprintf(str, white_key_name[i]);
		myFont_white.drawString(white_key_name[i], x1 + white_x * 1 / 3 + i * white_x, y2 - white_y / 7);
		// 헤더에서 선언한 white_key_name으로 for문을 사용해 효율적으로 글자를 print한다.
	}
	myFont_white.drawString("Sustain pedal(Space bar)", ofGetWidth() / 2 - 1.5 * white_x, ofGetHeight() - 60);
	ofSetColor(ofColor::white);
	// 반복문을 사용해 검은 건반에 글자를 출력
	for (int i = 0; i < num_of_black; i++) {
		//sprintf(str, black_key_name[i]);
		myFont_black.drawString(black_key_name[i], x1 + white_x * 13 / 16 + i * white_x, y2 - white_y / 2);
		// 헤더에서 선언한 black_key_name으로 for문을 사용해 효율적으로 글자를 print한다.
		// 검은 건반이 없는 위치의 black_key_name은 빈 문자열이니 아무런 문제가 없다. 하나의 for문으로 깔끔하게 구현이 가능했다.
	}

	if (perfect_timing > 0) {
		ofSetColor(ofColor::forestGreen);
		sprintf(str, "PERFECT: %d / GOOD; %d / FAIL: %d\nCURRENT HP: %d", num_of_perfect, num_of_good, num_of_fail, max_num_of_fail - num_of_fail);
		// 현재 기록과 남은 hp를 화면에 출력해준다.
		myFont2.drawString(str, ofGetWidth() / 2 + 4 * white_x, ofGetHeight() - 70);
	}
}
void ofApp::sound_load() {
	// 필요한 sound file을 load하는 함수
	intro.load("sounds/intro.mp3"); // loading 성공 시 1, 아니면 0
	
	
	// 파일 확장자 설정. 필요에 의해 바꿀 수 있다.
	char extension[] = ".wav";
	// 건반 이름과 키를 화면에 출력할 때 쓰는 문자열 버퍼
	char* temps = (char*)malloc(sizeof(int) * 20);
	
	// 헤더에서 선언한 white_file_name, black_file_name과 strcpy, strcat 함수로 경로를 인자로 전달해
	// 반복문을 사용해 각 건반들에 대해 wav 파일을 load한다.
	for (int i = 0; i < num_of_white; i++) {
		strcpy(temps, "sounds/");
		strcat(temps, white_file_name[i]);
		strcat(temps, extension);
		white_sound[i].load(temps);
		white_sound[i].setMultiPlay(false); // 키를 꾹 누를 경우 음이 반복 재생되는 일이 없도록 setMultiplay(false) 처리한다.
		//printf("%s\n", temps);
	}

	for (int i = 0; i < num_of_black; i++) {
		if (i == 3 || i == 6 || i == 10 || i == 13 || i == 17) continue;
		strcpy(temps, "sounds/");
		strcat(temps, black_file_name[i]);
		strcat(temps, extension);
		black_sound[i].load(temps);
		black_sound[i].setMultiPlay(false);
		//printf("%s\n", temps);
	}
	//white_sound[F2].load("sounds/sample.mp3");
	gamestart.load("sounds/gamestart.ogg");
	gameover.load("sounds/gameover.mp3"); 
}

void ofApp::image_load() {
	
	notes[0].load("images/Note_4.PNG");
	notes[1].load("images/Note_8.PNG");
	notes[2].load("images/Note_16.PNG");
}

void ofApp::perfect_timing_init(int level) {
	intro.stop(); // intro가 재생 중이라면 멈춰준다.
	gamestart.play(); // 게임 시작 효과음을 재생한다
	// 게임 초기 설정
	play_piano = 0;
	perfect_timing = level;
	num_of_perfect = 0;
	num_of_good = 0;
	num_of_fail = 0;
	max_num_of_fail = level * 15;
	is_gameover = 0;
	

	for (int i = 0; i < level; i++) create_note();
	// level에 따라 떨어지는 음의 개수가 늘어남. 난이도별로 다른 개수를 생성하기 위해 반복문 사용

	

	// result를 파일에서 불러오기
	FILE* fp;
	switch (level) {
	case 1:
		fp = fopen("data/result_easy.txt", "r");
		break;
	case 2:
		fp = fopen("data/result_hard.txt", "r");
		break;
	case 3:
		fp = fopen("data/result_comsil.txt", "r");
		break;
	default:
		return;
	}
	if (fp) { // 파일이 있다면
		int len;
		fscanf(fp, "%d", &len); // 기록의 개수를 받는다
		maxheap = new result[len + 2]; // 기록을 저장할 max heap을 생성
		maxheap_len = 0;
		// 파일에 저장된 랭킹의 길이만큼 파일을 읽어서, max heap에 저장
		for (int i = 1; i <= len; i++) {
			result tempresult;
			fscanf(fp, "%d %d", &tempresult.perfect, &tempresult.good);

			// max heap에 element 삽입하는 부분(push)
			push_maxheap(tempresult);
		}
		fclose(fp);
	}
	else {
		maxheap_len = 0;
		maxheap = new result[maxheap_len+2];
	}
	
	
}

void ofApp::perfect_timing_end() {
	// 끝날 시 일시적으로 할당된 Note들의 메모리를 해제. 
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
	
	delete[] maxheap; // heap의 경우 게임 init, end마다 메모리를 다르게 할당하므로 해제
	maxheap_len = 0;
	perfect_timing = 0;
}

void ofApp::perfect_timing_gameover() {
	is_gameover = 1; // 게임오버 상태임을 나타낸다
	gameover.play();
	
	// 게임이 종료될 때 눌려있던 건반들을 떼 주기 위해 반복문 사용.
	for (int i = 0; i < num_of_white; i++) {
		white_keypressedflag[i] = 0;
	}
	for (int j = 0; j < num_of_black; j++) {
		black_keypressedflag[j] = 0;
	}
	
	// 현재 게임의 기록을 result에 저장하기 위해 maxheap에 push
	result tempresult;
	tempresult.perfect = num_of_perfect;
	tempresult.good = num_of_good;
	push_maxheap(tempresult);

	int i;

	// Top 5에 사용할 result들을 maxheap에서 pop하는 과정을 위해 반복문 사용
	for (i = 0; i < 5; i++) {
		if (maxheap_len < 1) break; // 최대 힙에 원소가 없다면 break
		
		// maxheap에서 원소 pop 과정
		result_print[i] = pop_maxheap();
	}

	// 만약 maxheap에 5개보다 적은 개수가 있었다면, draw 할 때를 생각해서 구별해준다.
	if (i != 5) {
		result_print[i].perfect = -1;
		// 이후 draw 시 maxheap[i].perfect가 -1일 경우 그 부분부터는 화면에 출력하지 않는다.
	}

	// 저장된 모든 result들을 파일에 출력
	FILE* fp;
	switch (perfect_timing) {
	case 1:
		fp = fopen("data/result_easy.txt", "w");
		break;
	case 2:
		fp = fopen("data/result_hard.txt", "w");
		break;
	case 3:
		fp = fopen("data/result_comsil.txt", "w");
		break;
	default:
		fp = fopen("data/something_wrong.txt", "w");
		// 게임 모드일 때 perfect_timing은 반드시 1, 2, 3중 하나이니 이 값들 중 하나가 아니라면 문제가 생겼다는 뜻이다.
	}
	fprintf(fp, "%d\n", i + maxheap_len); // 파일에 개수 저장
	//cout << t + maxheap_len << endl;
	
	// 앞서 화면 출력 용으로 따로 저장해둔 (최대) 5개의 기록을 먼저 파일에 쓴다.
	for (int j = 0; j < i; j++) {
		fprintf(fp, "%d %d\n", result_print[j].perfect, result_print[j].good);
		//cout << result_print[i].perfect << " " << result_print[i].good << endl;
	}
	// maxheap 안의 result를 랭킹 순으로 파일에 출력하기 위해 반복문을 사용한다.
	int iter = maxheap_len; // 반복할 횟수
	for (int i = 0; i < iter; i++) {
		if (maxheap_len < 1) break; // 혹시나 원소가 없다면 break

		// maxheap에서 원소 pop 과정, 반복적으로 pop을 해 fprintf를 하는 것으로 txt 파일에 정렬된 result들을 출력.
		fprintf(fp, "%d %d\n", maxheap[1].perfect, maxheap[1].good); 
		// pop할 element를 파일에 출력한다. binary complete tree를 이용했으니 1 인덱스에 가장 높은 result가 있으니
		// 사실상 이 line에서 pop한 것이나 다름없다. 따라서 인덱스 참조를 통해 파일에 출력하고 pop 함수의 반환값은 무시했다.
		pop_maxheap();
	}

	fclose(fp); // FP 닫음
}

void ofApp::perfect_timing_gameover_draw() {
	// 게임오버 시 화면을 출력한다.

	char str[200]; // 버퍼
	ofFill();
	ofSetColor(ofColor::white);
	ofDrawRectangle(ofGetWidth() / 4, ofGetHeight() / 5, ofGetWidth() / 2, ofGetHeight() / 1.75);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectangle(ofGetWidth() / 4, ofGetHeight() / 5, ofGetWidth() / 2, ofGetHeight() / 1.75); // 게임오버 결과창 그릴 흰 네모박스
	
	// 결과 창의 string 출력
	sprintf(str, "Game-Ooooooover!");
	myFont2.drawString(str, ofGetWidth() / 2 - 115, ofGetHeight() / 4 + 20);
	sprintf(str, "Result: PERFECT: %d GOOD: %d FAIL: %d", num_of_perfect, num_of_good, num_of_fail);
	myFont2.drawString(str, ofGetWidth() / 2 - 225, ofGetHeight() / 4 + 60);
	sprintf(str, "Press any key but ESC to continue");
	myFont2.drawString(str, ofGetWidth() / 2 - 195, ofGetHeight() / 4 + 100);

	myFont2.drawString("-----Top 5 results-----", ofGetWidth() / 2 - 130, ofGetHeight() * 4 / 9);
	// 최대 5개의 상위 결과를 출력하기 위해 반복문 사용
	for (int i = 0; i < 5; i++) {
		if (result_print[i].perfect == -1) break;
		sprintf(str, "%d) PERFECT: %3d | GOOD: %3d", i + 1, result_print[i].perfect, result_print[i].good);
		myFont2.drawString(str, ofGetWidth() / 2 - 180, ofGetHeight() / 2 + 40 * (i + 1));
	}

}

void ofApp::create_note() {
	// note를 생성해서 연결 리스트에 넣는다.
	Note* temp = new Note;
	// 멤버 변수 초기화
	temp->key = rand() % 2;
	int t;
	t = rand() % 3; // 음표의 종류 지정
	temp->len = t; //  음표의 종류 지정
	temp->speed = 3.5 + 0.5 * (perfect_timing - 1); // 현재 난이도에 따른 속도 설정
	temp->acce = (0.1 + (float)(rand() % 10) / 100) * (t + 1); // 음표의 종류에 따르지만 무작위 요소도 함께 가속도 설정
	if (temp->key == 0) { // 흰 건반이 0, 흰 건반일 시 떨어지는 건반 설정
		temp->x = rand() % num_of_white;
	}
	else {
		while (1) {
			// 검은 건반인 경우, 나오면 안 되는 x값이 있으니 그 값을 제외하고 찾기 위해 while 사용
			t = rand() % num_of_black;
			if (t == 3 || t == 6 || t == 10 || t == 13 || t == 17) continue; // 검은 건반은 빈 곳이 있다!
			temp->x = t;
			break;
		}
	}
	temp->y = 0;
	temp->is_calculated = 0;
	// double linked list에 연결
	temp->prev = score_rear->prev;
	temp->next = score_rear;
	score_rear->prev->next = temp;
	score_rear->prev = temp;
}

void ofApp::update_note() {
	// update() 함수 내부에서 게임 중일 때 실행되는 함수로, 떨어지는 음표들의 정보를 업데이트한다.
	Note* temp = score_front->next; // front는 dummy
	Note* delnote;
	// 현재 떨어지는 중인 모든 음표들을 확인하기 위해 반복문 사용
	for (temp; temp != score_rear; ) {
		// y, 속도 갱신
		temp->y += temp->speed;
		temp->speed += temp->acce;
		//int ty;
		if (temp->is_calculated == 1) { // 이미 처리된 노트(음표)의 경우 제거해 줘야 한다
			delnote = temp;
			create_note(); // 사라진 노트의 자리를 채울 새 노트를 만들어서 리스트에 연결
			temp = temp->next;
			if (delnote->prev) delnote->prev->next = temp;
			temp->prev = delnote->prev;
			delete delnote;
		}
		else if (temp->y > y1 + (temp->speed * 6)) { //y1이 현재 속력 기준으로 6프레임 후 이동거리보다 밑에 있을 시 그 노트는 사라진다.
			delnote = temp;
			num_of_fail++; // 무시된 음표의 경우 fail로 결정
			create_note(); // 사라진 노트의 자리를 채울 새 노트를 만들어서 리스트에 연결
			temp = temp->next;
			if (delnote->prev) delnote->prev->next = temp;
			temp->prev = delnote->prev;
			delete delnote;
			//cout << temp->x << endl;
		}
		else temp = temp->next;

		// 게임오버 조건
		if (!is_gameover && num_of_fail >= max_num_of_fail) { // gameover 함수는 한 번만 호출되어야 하므로, 아직 호출되지 않았으면서 게임오버 조건 충족 시 호출
			perfect_timing_gameover(); // max_num_of_fail보다 많이 틀리면 gameover
		}
	}

}

void ofApp::perfect_timing_draw() {
	//ofSetColor(255, 255, 255, 0);
	//cout << score_front->x << " / " << score_front->y << endl;
	//cout << score_front->key << " len: " << score_front->len << endl;
	//ofSetColor(ofColor::grey);

	Note* temp = score_front->next;
	// 현재 떨어지는 음표들을 그리기 위해 반복문 사용
	for (temp; temp != score_rear; temp = temp->next) {
		if (temp->key == 0) { //흰 건반일 때, 그에 맞게 좌표를 계산해서 음표를 그려준다
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
			ofFill(); // 검은 건반은 회색으로 배경을 넣어 알아보기 쉽게 한다.
			//notes[score_front->len].draw(0, 0);
			notes[temp->len].draw(temp->x * white_x + x1 + white_x * 3 / 4, temp->y, black_x, 50);
			//ofDrawCircle(score_front->x * white_x + x1 + white_x * 7 / 8, score_front->y, black_x);
			//update_note();
		}
	}
	

}

void ofApp::calculate_timing(int key_type, int key_num) {
	Note* temp = score_front->next;
	int is_fail = 0; // input key가 현재 내려오는 노트들 중 어느 것에도 맞지 않는다면 잘못 누른 것 -> 그 경우 fail을 하나 올리기로 했다.
	if (num_of_fail < max_num_of_fail) {
		// update_note에서 처리되지 않은 음표가 있을 경우 fail을 1 올린다. update_note에 gameover 조건을 넣야 한다.
		// 그러나 update_note는 draw 후에 이뤄지기에 key 입력 -> calculate_timing -> draw -> update 사이클에서 
		// calculate_timing에서 num_of_fail이 너무 커졌는데도 draw 함수 전이라 gameover가 되지 않는 현상이 있었다.
		// calculate_timing 중 num_of_fail이 max_num_of_fail보다 커질 수 있으므로 여기서도 gameover 조건을 넣어야 한다.

		// 현재 떨어지고 있는 모든 음표들에 대해 입력 키가 대응하는지 확인하기 위해 반복문 사용
		for (int i = 0; i < perfect_timing; i++, temp = temp->next) {
			if (temp->is_calculated == 1) continue; // 이미 눌려서 점수가 갱신되었다면 이건 패스
			if (key_type == temp->key) { // 건반 색깔 유무를 맞춰야 함.
				if (key_num == temp->x) {
					is_fail = 1;
					temp->is_calculated = 1;
					// 판정의 기준: 평균 속력을 내서 프레임별 이동 거리 계산, 3프레임 내부일 시 perfect, 6프레임 내부일 시 good, 그 이하 fail
					float perfect_range = (temp->speed + 1.5 * temp->acce) * 3;
					float good_range = (temp->speed + 3 * temp->acce) * 6;
					// y1은 흰 건반의 높이, 즉 음표의 판정 부분과 같다!
					if (abs(temp->y + 50 - y1) < perfect_range) {
						num_of_perfect++; // 50은 내려오는 음표 박스의 길이임.
						max_num_of_fail++; // perfect한 timing을 맞출 경우, HP가 1 증가!
					}
					else if (abs(temp->y + 50 - y1) < good_range) num_of_good++; // good 판정
					else num_of_fail++; // fail 판정
					break; // 한 번에 하나씩만 처리해야 한다. 같은 자리에 2개가 떨어지고 있는 경우가 생길 수 있다.
				}
			}
		}
		if (!is_fail) num_of_fail++; // 입력과 현재 떨어지는 음표들이 맞지 않은 경우: fail++
	}
	else if (!is_gameover) perfect_timing_gameover(); // gameover 조건에 맞으면서 아직 함수가 호출되지 않은 경우 호출
}

void ofApp::push_maxheap(result tempresult) {
	int temp = ++maxheap_len;
	// complete binary tree로 봤을 때 tempresult를 가장 밑의 가장 오른쪽에 넣고, parent와 비교해가며 자신의 위치를 찾기 위해 반복문  사용.
	while (temp != 1) {
		// result 간 대소 비교는, perfect의 대소를 먼저 비교하고 같다면 good의 대소를 비교해 판별한다.
		if (tempresult.perfect > maxheap[temp / 2].perfect || (tempresult.perfect == maxheap[temp / 2].perfect && tempresult.good > maxheap[temp / 2].good)) {
			maxheap[temp] = maxheap[temp / 2];
			temp /= 2;
		}
		
		else break;
	}
	maxheap[temp] = tempresult;
}

result ofApp::pop_maxheap() {
	result returnresult = maxheap[1]; // pop할 element
	result tempresult = maxheap[maxheap_len--]; // complete binary tree 기준으로 가장 낮은 level의 마지막 element
	//cout << maxheap_len << endl;
	int anc = 1;
	int des = 2;
	// 가장 끝의 원소가 인덱스 1에 있다고 가정하고, child와 비교해가며 자신의 자리를 찾기 위해
	// 반복문 사용
	while (des <= maxheap_len) {
		if (des < maxheap_len && (maxheap[des + 1].perfect > maxheap[des].perfect || (maxheap[des + 1].perfect == maxheap[des].perfect && maxheap[des + 1].good > maxheap[des].good))) des++;
		// maxheap(max complete binary tree)에서 des와 des+1 중 우선순위가 더 큰 것을 골라야 하니, des+1의 우선순의가 더 높다면 des에 1 더해줌
		if (tempresult.perfect > maxheap[des].perfect || (tempresult.perfect == maxheap[des].perfect && tempresult.good >= maxheap[des].good)) break;
		// tempresult가 자신 밑 level의 두 element보다 더 크므로, 이 경우 tempresult는 현재 anc 자리에 있으면 된다.

		maxheap[anc] = maxheap[des]; // 그렇지 않다면 maxheap[des]을 한 단계 올려야 한다.
		anc = des; // 다음 층 탐색
		des *= 2;
	}
	maxheap[anc] = tempresult; // pop 후 heap 다시 정렬.

	return returnresult;
}