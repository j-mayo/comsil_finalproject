#include "ofApp.h"
#include "ofxWinMenu.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//sound_F2.load("sounds/sample.mp3");
	
	//sound_F2.setMultiPlay(true);
	
	// �⺻ ����. frame rate, title, ��, �޴� ���� �������ش�.
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


	// ������ ����� font�� load
	myFont1.loadFont("verdana.ttf", 12, true, true);
	myFont2.loadFont("verdana.ttf", 20, true, true);
	myFont_white.loadFont("frabk.ttf", 20, true, true);
	myFont_black.loadFont("frabk.ttf", 15, true, true);

	// window menu ����
	HMENU hMenu = menu->CreateWindowMenu();

	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");
	// Exit ������ ���� ���� �־��ش�
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	// mode �޴�
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
	// ���� �޴�
	hPopup = menu->AddPopupMenu(hMenu, "View");
	menu->AddPopupItem(hPopup, "Full screen", false, false);
	
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false);

	menu->SetWindowMenu();
	

	// ó���� �⺻������ play_piano ���� �����Ѵ�.
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
		// ������ ���δ�. �ϴ� �ƹ� ���̳� ���� �ߴµ� �׷��� �߿��� �κе� �ƴϴ� �� ���� ���.
		ofSystemAlertDialog("Piano Playtime! Enjoy your own keyboard-piano!\nMade by SJY\n\nmofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

	if (title == "Play Piano") {
		// play_piano = 1; // �⺻������ 1�� �����Ǿ� �ֱ� �ϴ�.
		// perfect_timing = 0; // ������ �����.
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
	// mode�� ���� �߰� ����

} // end appMenuFunction

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	if (perfect_timing) perfect_timing_draw(); // perfect timing�� ���� ���� ���, ������ ���� �ǾƳ뺸�� ���� �׷��ش�.

	//display.draw(ofGetWidth() / 6, 40, ofGetWidth() * 2 / 3, ofGetHeight() / 2);
	piano_draw();
	// ���� �ǾƳ븦 �׷��� �Ѵ�.
	
	// ��������. ���� �ǹ��� ǥ�����ش�..
	activatedkey_draw();

	// print the string in window
	print_window();
	// ���ڴ� �׻� �������� draw�Ǿ�� ����ڰ� �� �� �ִ�.
	
	

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// full screen �� ���� �κ�!
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
		if (key == ' ') { // sustain pedal�� ���� ���, ���� ���� ���� ��� �����ؾ� �Ѵ�. �̸� ���� ���� ǥ���Ѵ�.
			sustain_pedal = 1;
		}

		// sustain pedal�� ���� ���, ���� ���� �ǹݵ鿡 ���� sustain_flag�� ������ ��� �ϹǷ� �� �κ��� ����.
		if (sustain_pedal == 1) {
			// ���� ���� �ǹݵ鿡 ����, sustain_flag�� 1�� ������ �����ִٴ� ���� �˸���.
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
			// �Է����� ���� key�� ������ ���� ���¶�� ���� ����Ѵ�.

			//if (play_piano && key == white_keyboard[i] && white_keypressedflag[i] != 1) {
			if (key == white_keyboard[i]) {
				if (white_sustainflag[i] == 1) {
					white_keypressedflag[i] = 0;
					white_sound[i].stop();
				}
				if (white_keypressedflag[i] != 1) {
					white_keypressedflag[i] = 1; // �̹� ���� ���¿��� ���� ��� �� ��ø�Ǿ� �Ͱ� ���� �� �ִ�
					white_sound[i].play();
				}
				if (sustain_pedal == 1) white_sustainflag[i] = 1;
				// pedal�� ���� ���� ��, ���� ���� ��� �� ������ Ű �Է��� �����.
				// �̴� �ƹ����� openFramework key �ν� �Ǵ� ��ǻ�� Ű���� ���� ������ ������ ���δ�.
				// ���� pedal�� ���� �� ���� ���¿��� �� �ٸ� ���� ���� ���, ��� ���̶�� ǥ���� ��� �ϹǷ� sustain_flag[i]�� 1�� ����. ���� ���� �ǹݿ����� ����������.
			}
		}

		for (int i = 0; i < num_of_black; i++) {
			if (i == 3 || i == 6 || i == 10 || i == 13 || i == 17) continue;
			// ���� �ǹ��� ���� ��ġ�� ���ؼ��� key �Է��� ���� �ʾƾ� �Ѵ�.
			//if (play_piano && key == black_keyboard[i] && black_keypressedflag[i] != 1) {
			if (key == black_keyboard[i]) {
				if (black_sustainflag[i] == 1) {
					black_keypressedflag[i] = 0;
					black_sound[i].stop();
				}
				if (black_keypressedflag[i] != 1) { // �̹� ���� ���¿��� ���� ��� �� ��ø�Ǿ� �Ͱ� ���� �� �ִ�
					black_keypressedflag[i] = 1;
					black_sound[i].play();
				}
				if (sustain_pedal == 1) black_sustainflag[i] = 1;
			}
		}
	}
	
	if (perfect_timing > 0) { // perfect timing ���ӿ��� sustain pedal�� Ȱ��ȭ���� �ʴ´�.
		for (int i = 0; i < num_of_white; i++) {
			if (key == white_keyboard[i]) {
				if (white_keypressedflag[i] != 1) {
					white_keypressedflag[i] = 1; 
					white_sound[i].play();
				}
				calculate_timing(0, i); // ���� ���� ���� ����� �ϰ�, ���� ��ǥ���� �������� �Ͱ� �����ش�.

			}
		}

		for (int i = 0; i < num_of_black; i++) {
			if (i == 3 || i == 6 || i == 10 || i == 13 || i == 17) continue;
			if (key == black_keyboard[i]) {
				if (black_keypressedflag[i] != 1) { 
					black_keypressedflag[i] = 1;
					black_sound[i].play();
				}
				calculate_timing(1, i); // ���� ���� ���� ����� �ϰ�, ���� ��ǥ���� �������� �Ͱ� �����ش�.
			}
		}

	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	if (play_piano) {
		if (key == ' ') {
			// sustain pedal���� ���� �� ���, pedal�� ���� ���� ���� ��� ���� ����� �Ѵ�.
			sustain_pedal = 0;
			// �����̽��ٸ� ���� sustain���̿��� ��� key���� ������ ���� ���·� ���Ѵ�.
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
			// sustain_pedal�� ���� ���¶�� ���� �����ؼ��� �� �ǹǷ� ���ǿ� �߰��Ѵ�.
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

	if (perfect_timing > 0) { // perfect timing ���ӿ��� sustain pedal�� Ȱ��ȭ���� �ʴ´�.
		// ���⼱ ���� �ٷ� ���� �ʰ�, �׳� keypressedflag�� 0���� ������ �׸��� �� �׷�����, ���� �ڿ������� ���⵵�� �Ѵ�.
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
	// ���� �ǹ��� ������ ��� 21��, ������ 15���� �� 36���̴�. �� �������� 20���̳�, �װ��� �ٷ� ���� �ǹ��� �������� �����Ƿ� ���� ���� ȿ������ ���� 20���� �ߴ�.
	num_of_white = 21;
	num_of_black = 20;
	// �ʿ��� �ڷᱸ������ ���� �Ҵ� �� �ش�.
	if (!white_keypressedflag) white_keypressedflag = new int[num_of_white];
	if (!black_keypressedflag) black_keypressedflag = new int[num_of_black];
	if (!white_sustainflag) white_sustainflag = new int[num_of_white];
	if (!black_sustainflag) black_sustainflag = new int[num_of_black];
	
	// �ڷᱸ���� �ʱ�ȭ�Ѵ�.
	for (int i = 0; i < num_of_white; i++) {
		white_keypressedflag[i] = 0;
		white_sustainflag[i] = 0;
	}
	for (int i = 0; i < num_of_black; i++) {
		black_keypressedflag[i] = 0;
		black_sustainflag[i] = 0;
	}
	// sustain_pedal�� �ʱ⿡ ������ �����Ƿ� 0���� �ʱ�ȭ.
	sustain_pedal = 0;
}

void ofApp::piano_draw() {
	char str[200];

	// ȭ�鿡 ���� �ǹ� ���̸� ����
	white_x = (float)((ofGetWidth() - 20) / num_of_white); // �¿� ���� 10px
	white_y = (float)((ofGetHeight() - 20) / 2.5); // �ϴ� ���� 20px
	black_x = (float)white_x / 2;
	black_y = (float)white_y * 3 / 5;
	// ������ �� ǥ�õ� ���� �׸� ������ ����
	white_r = white_x / 3;
	black_r = black_x / 3;

	// �ǾƳ� ��ü �簢�� ������ ����
	x1 = 10;
	x2 = ofGetWidth() - 10;
	y2 = ofGetHeight() - 100 - 20;
	y1 = y2 - white_y;
	ofSetColor(ofColor::white);
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofSetLineWidth(5);
	// ��ü���� Ʋ �簢�� �׸�
	ofFill();
	ofDrawRectangle(x1, y1, x2 - x1 - 10, y2 - y1); // ���⼭ �׸��� ��, perfect timiing ���� �� ��ǥ�� �ǾƳ� ������ ���� �� ������ ����.
	ofSetColor(ofColor::black);
	ofNoFill();
	ofDrawRectangle(x1, y1, x2 - x1 - 10, y2 - y1); // ��� ���߱� ���� -10
	float tempx = x1 + white_x;
	// �� �ǹ� �׸�
	for (int i = 0; i < num_of_white - 1; i++) {
		ofDrawLine(tempx, y1, tempx, y2);
		tempx += white_x;
	}
	// ���� �ǹ� �׸�
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
	// draw the space bar(sustain pedal�� �׸� ��)
	ofNoFill();
	ofDrawRectangle(ofGetWidth() / 2 - 3 * white_x, y2 + 20, 6 * white_x, 70);
}

void ofApp::activatedkey_draw() {
	// if key activatied, then the window highlight that key

	// ���� �ǹݿ� ���� ĥ�� ����ڰ� �������� �� �� �ְ� �Ѵ�.
	ofFill();
	ofSetColor(ofColor::lightSalmon);
	for (int i = 0; i < num_of_white; i++) { // �� �ǹݿ� ����
		if (white_keypressedflag[i] == 1) {
			ofDrawCircle(x1 + white_x / 2 + i * white_x, y2 - white_y / 6, white_r); // ������ ��ǥ�� ����� ���� ���� ����Ѵ�.
		}
	}

	for (int i = 0; i < num_of_black; i++) { // ���� �ǹݿ� ����
		if (black_keypressedflag[i] == 1) { // black_key�� ��� �߰��߰� ���� �ǹ��� ������ �̰͵��� ���� ���� ������ ���� ���� ó������ �ʾҴ�.
			ofDrawCircle(x1 + white_x * (i + 1), y2 - white_y / 2 - white_y / 60, black_r); // ������ ��ǥ�� ����� ���� ���� ����Ѵ�.
		}
	}
	if (sustain_pedal) ofDrawRectangle(ofGetWidth() / 2 - 2 * white_x, y2 + 25, 4 * white_x, 50); // sustain pedal�� ��� �簢���� ǥ���ߴ٤�.
}

void ofApp::print_window() {
	char str[200]; // myfont1���� ȭ�鿡 ������ ��Ÿ���� ���� str �����̴�

	ofSetColor(ofColor::darkGray);
	sprintf(str, " 20181277_ShinJaeyo_comsil_final_project ");
	myFont1.drawString(str, 15, ofGetHeight() - 5);

	// �� �ǹݿ� ���ڸ� ���
	ofSetColor(ofColor::black);
	// print the white key name
	for (int i = 0; i < num_of_white; i++) {
		//sprintf(str, white_key_name[i]);
		myFont_white.drawString(white_key_name[i], x1 + white_x * 1 / 3 + i * white_x, y2 - white_y / 7);
		// ������� ������ white_key_name���� for���� ����� ȿ�������� ���ڸ� print�Ѵ�.
	}
	myFont_white.drawString("Sustain pedal(Space bar)", ofGetWidth() / 2 - 1.5 * white_x, ofGetHeight() - 60);
	ofSetColor(ofColor::white);
	// print the black key name
	for (int i = 0; i < num_of_black; i++) {
		//sprintf(str, black_key_name[i]);
		myFont_black.drawString(black_key_name[i], x1 + white_x * 13 / 16 + i * white_x, y2 - white_y / 2);
		// ������� ������ black_key_name���� for���� ����� ȿ�������� ���ڸ� print�Ѵ�.
		// ���� �ǹ��� ���� ��ġ�� black_key_name�� �� ���ڿ��̴� �ƹ��� ������ ����. �ϳ��� for������ ����ϰ� ������ �����ߴ�.
	}

	if (perfect_timing > 0) {
		ofSetColor(ofColor::forestGreen);
		sprintf(str, "PERFECT: %d / GOOD; %d / FAIL: %d", num_of_perfect, num_of_good, num_of_fail);
		myFont2.drawString(str, ofGetWidth() / 2 + 4 * white_x, ofGetHeight() - 50);
	}
}
void ofApp::sound_load() {
	// �ʿ��� sound file�� load�ϴ� �Լ�
	is_intro = intro.load("sounds/intro.mp3"); // loading ���� �� 1, �ƴϸ� 0
	intro.play();


	// �迭 �Ҵ�
	white_sound = new ofSoundPlayer[num_of_white];
	black_sound = new ofSoundPlayer[num_of_black];
	// ���� Ȯ���� ����. �ʿ信 ���� �ٲ� �� �ִ�.
	char extension[] = ".wav";
	// �ǹ� �̸��� Ű�� ȭ�鿡 ����� �� ���� ���ڿ� ����
	char* temps = (char*)malloc(sizeof(int) * 20);
	
	// ������� ������ white_file_name, black_file_name�� strcpy, strcat �Լ��� ��θ� ���ڷ� ������ wav ������ load�Ѵ�.
	for (int i = 0; i < num_of_white; i++) {
		strcpy(temps, "sounds/");
		strcat(temps, white_file_name[i]);
		strcat(temps, extension);
		white_sound[i].load(temps);
		white_sound[i].setMultiPlay(false); // Ű�� �� ���� ��� ���� �ݺ� ����Ǵ� ���� ������ setMultiplay(false) ó���Ѵ�.
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
	for (int i=0; i<level; i++) create_note(); // level�� ���� �������� ���� ������ �þ.

}

void ofApp::perfect_timing_end() {
	// ���� �� �Ͻ������� �Ҵ�� �޸𸮸� ����. free memory�� ������ ������ free-memory���� �����ϴ� �޸𸮴� 
	// ���α׷� ��ü�� �޸��̴�. play_piano���� �Ҵ��� �޸��� ��� ó������ �Ҵ��ϰ� ���� ���Ҵ��� �ʿ䰡 ������(�ǾƳ븦 �ĵ� �޸� ������ �� ����).
	// perfect_timing �ÿ��� ���� ����Ʈ�� ����ϱ� ������ note ���� �ø��� �����ϸ� �޸𸮰� ���ϹǷ�,
	// ������ �� �ʱ�ȭ�� ���� ������ ���� �� �ִ�. ����  �ʱ� ���·� �޸𸮸� ������ �ϹǷ�, 
	// ���� �� �޸� ������ ���� �Ҵ��� ���� ����.
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
	// note�� �����ؼ� ���� ����Ʈ�� �ִ´�.
	Note* temp = new Note;
	temp->key = rand() % 2;
	int t;
	t = rand() % 3;
	temp->len = t;
	temp->speed = 4;
	temp->acce = (0.1 + (float)(rand() % 10) / 100) * (t + 1);
	if (temp->key == 0) { // �� �ǹ��� 0
		temp->x = rand() % 21;
	}
	else {
		while (1) {
			t = rand() % 20;
			if (t == 3 || t == 6 || t == 10 || t == 13 || t == 17) continue; // ���� �ǹ��� �� ���� �ִ�!
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

	Note* temp = score_front->next; // front�� dummy
	Note* delnote;
	for (temp; temp != score_rear; ) {
		temp->y += temp->speed;
		temp->speed += temp->acce;
		int ty;
		if (temp->y > y1 + (temp->speed * 6)) { //y1�� ���� �ӷ� �������� 6������ �� �̵��Ÿ����� �ؿ� ���� �� �� ��Ʈ�� �������.
			delnote = temp;
			if (temp->is_calculated == 0) num_of_fail++; // ���õ� ��ǥ�� ��� fail�� ����
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
		if (temp->key == 0) { //�� �ǹ��� ��
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
			ofFill(); // ���� �ǹ��� ȸ��!
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
	int is_fail = 0; // input key�� ���� �������� ��Ʈ�� �� ��� �Ϳ��� ���� �ʴ´ٸ� �߸� ���� �� -> �� ��� fail�� �ϳ� �ø���� �ߴ�.
	for (int i = 0; i < perfect_timing; i++, temp = temp->next) {
		if (temp->is_calculated == 1) continue; // �̹� ������ ������ ���ŵǾ��ٸ� �̰� �н�
		if (key_type == temp->key) { // �ǹ� ���� ������ ����� ��.
			if (key_num == temp->x) {
				is_fail = 1;
				temp->is_calculated = 1;
				// ������ ����: ��� �ӷ��� ���� �����Ӻ� �̵� �Ÿ� ���, 2������ ������ �� perfect, 6������ ������ �� good, �� ���� fail
				float perfect_range = (temp->speed + temp->acce) * 3;
				float good_range = (temp->speed + 3 * temp->acce) * 6;
				// y1�� �� �ǹ��� ����, �� ��ǥ�� ���� �κа� ����!
				if (abs(temp->y + 50 - y1) < perfect_range) num_of_perfect++; // 50�� �������� ��ǥ �ڽ��� ������.
				else if (abs(temp->y + 50 - y1) < good_range) num_of_good++;
				else num_of_fail++;
				break; // �� ���� �ϳ����� ó���ؾ�.
			}
		}
	}
	if (!is_fail) num_of_fail++; // �Է°� ���� �������� ��ǥ���� ���� ���� ���: fail++
}