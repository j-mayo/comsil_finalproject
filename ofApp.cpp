#include "ofApp.h"
#include "ofxWinMenu.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//sound_F2.load("sounds/sample.mp3");
	
	//sound_F2.setMultiPlay(true);
	

	ofSetWindowTitle("Piano Playtime!");
	ofSetFrameRate(15);
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

	myFont1.loadFont("verdana.ttf", 12, true, true);
	myFont_white.loadFont("frabk.ttf", 20, true, true);
	myFont_black.loadFont("frabk.ttf", 15, true, true);

	// window menu 생성
	HMENU hMenu = menu->CreateWindowMenu();

	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");
	// Exit 전에는 구분 줄을 넣어준다
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	// 악기 메뉴
	hPopup = menu->AddPopupMenu(hMenu, "Instrument");
	menu->AddPopupItem(hPopup, "Piano", false, false);
	menu->AddPopupItem(hPopup, "Violin", false, false);

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

	// key part
	num_of_white = 21;
	num_of_black = 20;
	white_keypressedflag = new int[num_of_white];
	black_keypressedflag = new int[num_of_black];
	for (int i = 0; i < num_of_white; i++) {
		white_keypressedflag[i] = 0;
	}
	for (int i = 0; i < num_of_black; i++) {
		black_keypressedflag[i] = 0;
	}

	// sound part;
	//white_sound_flag = new int[14];
	//black_sound_flag = new int[13];
	sound_load();
	
	
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
		ofSystemAlertDialog("Piano Playtime! Enjoy your own keyboard-piano!\nMade by SJY\n\nmofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	char str[200];
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

	if (is_intro) piano_draw();
	
	// if key pressed, then the window highlight that key
	if (white_keypressedflag[F2] == 1) {
		ofSetColor(ofColor::lightSalmon);
		ofDrawCircle(x1 + white_x / 2, y2 - white_y / 6, white_r);
	}
	ofSetColor(ofColor::lightSalmon);
	for (int i = 0; i < num_of_white; i++) {
		if (white_keypressedflag[i] == 1) {
			ofDrawCircle(x1 + white_x / 2 + i * white_x, y2 - white_y / 6, white_r);
		}
	}

	for (int i = 0; i < num_of_black; i++) {
		if (black_keypressedflag[i] == 1) {
			ofDrawCircle(x1 + white_x * (i + 1), y2 - white_y / 2 - white_y / 60, black_r);
		}
	}

	ofSetColor(ofColor::darkGray);
	sprintf(str, " 20181277_ShinJaeyo_comsil_final_project ");
	myFont1.drawString(str, 15, ofGetHeight() - 5);

	ofSetColor(ofColor::black);
	// print the white key name
	for (int i = 0; i < num_of_white; i++) {
		//sprintf(str, white_key_name[i]);
		myFont_white.drawString(white_key_name[i], x1 + white_x * 1 / 3 + i * white_x, y2 - white_y / 7);
	}
	ofSetColor(ofColor::white);
	// print the black key name
	for (int i = 0; i < num_of_black; i++) {
		//sprintf(str, black_key_name[i]);
		myFont_black.drawString(black_key_name[i], x1 + white_x * 13 / 16 + i * white_x, y2 - white_y / 2);
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
	*/
	for (int i = 0; i < num_of_white; i++) {
		if (key == white_keyboard[i] && white_keypressedflag[i] != 1) {
			white_keypressedflag[i] = 1;
			white_sound[i].play();
		}
	}

	for (int i = 0; i < num_of_black; i++) {
		if (i == 3 || i == 6 || i == 10 || i == 13 || i == 17) continue;
		if (key == black_keyboard[i] && black_keypressedflag[i] != 1) {
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
	delete[] white_sound;
	delete[] black_sound;
}

void ofApp::piano_draw() {
	char str[200];
	// 건반의 길이를 설정
	white_x = (float)((ofGetWidth() - 20) / num_of_white); // 좌우 여유 10px
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
	ofDrawRectangle(x1, y1, x2 - x1 - 10, y2 - y1); // 모양 맞추기 위한 -10
	float tempx = x1 + white_x;
	// draw the white key
	for (int i = 0; i < num_of_white - 1; i++) {
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
}

bool ofApp::sound_load() {
	intro.load("sounds/intro.mp3");
	intro.play();
	is_intro = 1;
	white_sound = new ofSoundPlayer[num_of_white];
	black_sound = new ofSoundPlayer[num_of_black];
	char extension[] = ".mp3";
	char* temps = (char*)malloc(sizeof(int) * 20);
	
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