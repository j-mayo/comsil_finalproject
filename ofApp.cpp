#include "ofApp.h"
#include "ofxWinMenu.h"

//--------------------------------------------------------------
void ofApp::setup(){
	sound_F2.load("sounds/test.mp3");
	sound_G2.load("sounds/ralo.mp3");
	
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
	myFont2.loadFont("frabk.ttf", 12, true, true);

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

	white_keypressedflag = new int[14];
	black_keypressedflag = new int[13];

	// sound part;
	//white_sound_flag = new int[14];
	//black_sound_flag = new int[13];
	
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
	// 건반의 길이를 설정
	white_x = (float)((ofGetWidth()-20) / 21); // 좌우 여유 10px
	white_y = (float)((ofGetHeight() - 20) / 2.5); // 하단 여유 20px
	black_x = (float)white_x / 2;
	black_y = (float)white_y * 3 / 5;
	// 눌렸을 때 표시될 원을 그릴 반지름 설정
	white_r = white_x / 3;
	black_r = black_x / 3;

	float x1 = 10; 
	float x2 = ofGetWidth() - 10;
	float y2 = ofGetHeight() - 20;
	float y1 = y2 - white_y;
	ofSetColor(ofColor::black);
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofSetLineWidth(5);
	ofNoFill();
	ofDrawRectangle(x1, y1, x2 - x1-10, y2 - y1); // 모양 맞추기 위한 -10
	float tempx = x1 + white_x;
	// white key
	for (int i = 0; i < 20; i++) {
		ofDrawLine(tempx, y1, tempx, y2);
		tempx += white_x;
	}

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

	if (white_keypressedflag[F2] == 1) {
		ofSetColor(ofColor::lightSalmon);
		ofDrawCircle(x1 + white_x / 2, y2 - white_y / 5, white_r);
	}

	ofSetColor(ofColor::gray);
	sprintf(str, " 20181277_ShinJaeyo_comsil_final_project ");
	myFont1.drawString(str, 15, ofGetHeight() - 5);

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

	if (key == 'z' || key == 'Z') {
		if (white_keypressedflag[F2] != 1) {
			white_keypressedflag[F2] = 1;
			sound_F2.play();
		}
	}
	if (key == 'x' || key == 'X') {
		if (white_keypressedflag[G2] != 1) {
			white_keypressedflag[G2] = 1;
			sound_G2.play();
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
	if (key == 'z' || key == 'Z') {
		if (white_keypressedflag[F2] == 1) {
			white_keypressedflag[F2] = 0;
			sound_F2.stop();
		}
	}
	
	if (key == 'x' || key == 'X') {
		if (white_keypressedflag[G2] == 1) {
			white_keypressedflag[G2] = 0;
			sound_G2.stop();
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