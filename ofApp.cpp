#include "ofApp.h"
#include "ofxWinMenu.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//sound_F2.load("sounds/sample.mp3");
	
	//sound_F2.setMultiPlay(true);
	
	// 기본 설정. frame rate, title, 색, 메뉴 등을 설정해준다.
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
	// menu->AddPopupItem(hPopup, "Violin", false, false);

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
	play_piano = 1;
	// key part
	// 현재 건반의 개수는 흰색 21개, 검은색 15개로 총 36개이다. 왜 검은색이 20개이냐, 그것은 바로 검은 건반은 연속하지 않으므로 구현 시의 효율성을 위해 20개로 했다.
	num_of_white = 21;
	num_of_black = 20;
	// 필요한 자료구조들을 동적 할당 해 준다.
	white_keypressedflag = new int[num_of_white];
	black_keypressedflag = new int[num_of_black];
	white_sustainflag = new int[num_of_white];
	black_sustainflag = new int[num_of_black];
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

	// sound part;
	//white_sound_flag = new int[14];
	//black_sound_flag = new int[13];
	sound_load();
	
	// image part
	//display.load("images/musicimage.jpg");
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

	if (title == "Play Piano") play_piano = 1; // 기본적으로 1로 설정되어 있긴 하다.
	// mode에 따라 추가 예정

} // end appMenuFunction

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	char str[200]; // myfont1에서 화면에 문장을 나타내기 위한 str 버퍼이다
	/*
	// 건반의 길이를 설정
	white_x = (float)((ofGetWidth()-20) / num_of_white); // 좌우 여유 10px
	white_y = (float)((ofGetHeight() - 20) / 2.5); // 하단 여유 20px
	black_x = (float)white_x / 2;
	black_y = (float)white_y * 3 / 5;
	// 눌렸을 때 표시될 원을 그릴 반지름 설정
	white_r = white_x / 3;
	black_r = black_x / 3;

	x1 = 10; 
	x2 = ofGetWidth() - 10;
	y2 = ofGetHeight() - 20;
	y1 = y2 - white_y;
	ofSetColor(ofColor::black);
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofSetLineWidth(5);
	ofNoFill();
	// draw the overall rectangle
	ofDrawRectangle(x1, y1, x2 - x1-10, y2 - y1); // 모양 맞추기 위한 -10
	float tempx = x1 + white_x;
	// draw the white key
	for (int i = 0; i < num_of_white-1; i++) {
		ofDrawLine(tempx, y1, tempx, y2);
		tempx += white_x;
	}
	// draw the black key
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
	*/
	//display.draw(ofGetWidth() / 6, 40, ofGetWidth() * 2 / 3, ofGetHeight() / 2);
	piano_draw();
	// 가장 먼저 피아노를 그려야 한다.
	
	// if key pressed, then the window highlight that key
	/*
	if (white_keypressedflag[F2] == 1) {
		ofSetColor(ofColor::lightSalmon);
		ofDrawCircle(x1 + white_x / 2, y2 - white_y / 6, white_r);
	}
	*/

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


	// print the string in window
	// 글자는 항상 마지막에 draw되어야 사용자가 볼 수 있다.
	
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

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// full screen 에 대한 부분!
	printf("%c\n", key);
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

	/*
	if (key == 'z') {
		if (white_keypressedflag[F2] != 1) {
			white_keypressedflag[F2] = 1;
			white_sound[F2].play();
		}
	}
	if (key == 'x') {
		if (white_keypressedflag[G2] != 1) {
			white_keypressedflag[G2] = 1;
			white_sound[G2].play();
		}
	}

	if (key == 'c') {
		if (white_keypressedflag[A2] != 1) {
			white_keypressedflag[A2] = 1;
			white_sound[A2].play();
		}
	}

	if (key == 'v') {
		if (white_keypressedflag[B2] != 1) {
			white_keypressedflag[B2] = 1;
			white_sound[B2].play();
		}
	}
	// test!!!
	*/

	if (key == ' ') { // sustain pedal이 눌린 경우, 현재 눌린 음을 계속 지속해야 한다. 이를 위해 따로 표시한다.
		if (play_piano) sustain_pedal = 1;
		
	}

	// sustain pedal이 눌린 경우, 이 항목을 실행한다.
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
		// 음 재생의 조건은 현재 play_piano mode이고, 입력으로 들어온 key가 눌리지 않은 상태라면 음을 재생한다.
		// 설정에 따라 달라질 수 있기는 하나 이미 눌린 상태에서 음을 재생 시 중첩되어 귀가 아플 수 있다
		if (play_piano && key == white_keyboard[i] && white_keypressedflag[i] != 1) {
			white_keypressedflag[i] = 1;
			white_sound[i].play();
		}
	}

	for (int i = 0; i < num_of_black; i++) {
		if (i == 3 || i == 6 || i == 10 || i == 13 || i == 17) continue;
		// 검은 건반이 없는 위치에 대해서는 key 입력을 받지 않아야 한다.
		if (play_piano && key == black_keyboard[i] && black_keypressedflag[i] != 1) {
			black_keypressedflag[i] = 1;
			black_sound[i].play();
		}
	}

	

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	/*
	if (key == 'z') {
		if (white_keypressedflag[F2] == 1) {
			white_keypressedflag[F2] = 0;
			white_sound[F2].stop();
		}
	}
	
	if (key == 'x') {
		if (white_keypressedflag[G2] == 1) {
			white_keypressedflag[G2] = 0;
			white_sound[G2].stop();
		}
	}

	if (key == 'c') {
		if (white_keypressedflag[A2] == 1) {
			white_keypressedflag[A2] = 0;
			white_sound[A2].stop();
		}
	}

	if (key == 'v') {
		if (white_keypressedflag[B2] == 1) {
			white_keypressedflag[B2] = 0;
			white_sound[B2].stop();
		}
	}
	*/
	if (key == ' ') {
		// sustain pedal에서 손을 뗀 경우, pedal로 인해 지속 중인 모든 음을 멈춰야 한다.
		sustain_pedal = 0;
		// 스페이스바를 떼면 sustain중이였던 모든 key들이 눌리지 않은 상태로 변한다.
		for (int i = 0; i < num_of_white; i++) {
			if (play_piano && white_sustainflag[i] == 1) {
				white_keypressedflag[i] = 0;
				white_sustainflag[i] = 0;
				// white_sound[i].stop();
				
			}
		}

		for (int i = 0; i < num_of_black; i++) {
			if (i == 3 || i == 6 || i == 10 || i == 13 || i == 17) continue;
			if (play_piano && black_sustainflag[i] == 1) {
				black_keypressedflag[i] = 0;
				black_sustainflag[i] = 0;
				// black_sound[i].stop();
			}
		}
	}
	if (play_piano && sustain_pedal == 0) {
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
	ofSetColor(ofColor::black);
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofSetLineWidth(5);
	ofNoFill();
	// 전체적인 틀 사각형 그림
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

bool ofApp::sound_load() {
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
		//printf("%s\n", temps);
	}

	for (int i = 0; i < num_of_black; i++) {
		strcpy(temps, "sounds/");
		strcat(temps, black_file_name[i]);
		strcat(temps, extension);
		black_sound[i].load(temps);
		//printf("%s\n", temps);
	}
	//white_sound[F2].load("sounds/sample.mp3");
	return true;
}