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

	// ���� �ǹ��� ������ ��� 21��, ������ 15���� �� 36���̴�. �� �������� 20���̳�, �װ��� �ٷ� ���� �ǹ��� �������� �����Ƿ� ���� ���� ȿ������ ���� 20���� �ߴ�.
	num_of_white = 21;
	num_of_black = 20;

	// ȭ�鿡 ���� �ǹ� ���̸� ����
	white_x = (float)((ofGetWidth() - 20) / num_of_white); // �¿� ���� 10px
	white_y = (float)((ofGetHeight() - 20) / 2.5); // �ϴ� ���� 20px
	black_x = (float)white_x / 2;
	black_y = (float)white_y * 3 / 5;
	// ������ �� ǥ�õ� ���� �׸� ������ ����
	white_r = white_x / 3;
	black_r = black_x / 3;

	// �ʿ��� �ڷᱸ������ ���� �Ҵ�
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


	// ������ ����� font�� load
	myFont1.loadFont("verdana.ttf", 12, true, true);
	myFont2.loadFont("frabk.ttf", 20, true, true);
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
	
	// sound part;
	//white_sound_flag = new int[14];
	//black_sound_flag = new int[13];
	sound_load();
	intro.play();
	// image part
	image_load();
	
	// ó���� �⺻������ play_piano ���� �����Ѵ�.
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
		if (perfect_timing > 0) perfect_timing_end(); // ���� �� ���� �� �޸� ����
		free_Memory();
		ofExit();
		
	}
	//
	// Help menu
	//

	if (title == "About") {
		// ������ ���δ�. 
		ofSystemAlertDialog("Piano Playtime! Enjoy your own keyboard-piano!\nMade by SJY\n\nmofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

	if (title == "Play Piano") {
		// play_piano = 1; // �⺻������ 1�� �����Ǿ� �ֱ� �ϴ�.
		// perfect_timing = 0; // ������ �����.
		if (perfect_timing > 0) perfect_timing_end();
		play_piano_init();
	}
	// �� ���̵��� ���� ������ ���� ��, ���̵��� ���� ���� init �Լ� ����
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
	// ���� ���� ���̰� ���ӿ����� ���� �ʾҴٸ� update_note() �Լ��� ������
	// �������� ��ǥ���� ����
	if (perfect_timing > 0 && !is_gameover) {
		update_note();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	//cout << 0 << endl;
	if (perfect_timing && !is_gameover) perfect_timing_draw(); 
	// perfect timing�� ���� ���� ���, ������ ���� �ǾƳ뺸�� ���� �׷��ش�.
	// piano�� �簢�� ������ OfFill�� ���·� �׷����Ƿ�, ��ǥ���� �ǾƳ뿡 ����Ǵ� ȿ���� �� �� �ִ�.
	
	//display.draw(ofGetWidth() / 6, 40, ofGetWidth() * 2 / 3, ofGetHeight() / 2);
	piano_draw();
	// ���� �ǾƳ븦 �׷��� �Ѵ�.
	
	// ��������. ���� �ǹ��� ǥ�����ش�..
	activatedkey_draw();

	// print the string in window
	print_window();
	// ���ڴ� �׻� �������� draw�Ǿ�� ����ڰ� �� �� �ִ�.
	
	// ���� �� ���ӿ����� ���, ���ӿ��� ȭ���� �׷��ִ� �Լ��� ����.
	if (is_gameover == 1) {
		perfect_timing_gameover_draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// full screen �� ���� �κ�!
	
	if (key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if (bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			if (perfect_timing > 0) perfect_timing_end(); // ���� �� ���� �� �޸� ����
			free_Memory();
			ofExit();
			
		}
	}

	if (is_gameover == 1) { // ���ӿ��� ��, esc�� ������ �ƹ� Ű�� �����ٸ� ������ ó������ ����!
		if (key != VK_ESCAPE) {
			//is_gameover = 0;
			gameover.stop();
			int temp = perfect_timing; // gameover �� �̾��� �� ���̵��� �����ȴ�.
			perfect_timing_end(); // ��ŷ ������ ���� end
			perfect_timing_init(temp); // ���� ����
			//perfect_timing_init(max_num_of_fail / 15); // ���� �ٽ� ����, ���ӿ��� ����� ������ �ٽ� ����
			return;
		}
	}
	if (play_piano == 1) {
		if (key == ' ') { // sustain pedal�� ���� ���, ���� ���� ���� ��� �����ؾ� �Ѵ�. �̸� ���� ���� ǥ���Ѵ�.
			sustain_pedal = 1;
		}

		// sustain pedal�� ���� ���, ���� ���� �ǹݵ鿡 ���� sustain_flag�� ������ ��� �ϹǷ� �� �κ��� ����.
		if (sustain_pedal == 1) {
			// for������ ��� �ǹݿ� ���� �˻�
			// "���� ���� ������ �ǹݵ�"�� ����, sustain_flag�� 1�� ������ �����ִٴ� ���� �˸���.
			for (int i = 0; i < num_of_white; i++) {
				if (white_keypressedflag[i] == 1) {
					white_sustainflag[i] = 1;
				}
			}
			for (int i = 0; i < num_of_black; i++) {
				if (i == 3 || i == 6 || i == 10 || i == 13 || i == 17) continue;
				// ���� �ǹ��� �� ��ġ�� �ε����� Ȯ������ �ʴ´�.
				// ��� �ش� ��ġ���� sound ������ ���� ������ �� ���� ��� �����ϴ�.
				if (black_keypressedflag[i] == 1) {
					black_sustainflag[i] = 1;
				}
			}
		}
		// for���� ���� ��� �ǹݿ� ���� �Է� Ű�� ���ϸ� �˻�
		for (int i = 0; i < num_of_white; i++) {
			// �Է����� ���� key�� ������ ���� ���¶�� ���� ����Ѵ�.

			//if (play_piano && key == white_keyboard[i] && white_keypressedflag[i] != 1) {
			if (key == white_keyboard[i]) {
				if (white_sustainflag[i] == 1 && white_keypressedflag[i] != 1) {
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
				if (black_sustainflag[i] == 1 && black_keypressedflag[i] != 1) {
					//black_keypressedflag[i] = 0;
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
		// perfect timing ���� �� �ݺ������� ��� �ǹ��� �˻��ؼ�, �Է� Ű�� �´� �ǹ��� �ִٸ� ���� ����ϰ� calculate_timing �Լ��� ȣ���Ѵ�
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
void ofApp::keyReleased(int key) {

	if (play_piano == 1) {
		if (key == ' ') {
			// sustain pedal���� ���� �� ���, pedal�� ���� ���� ���� ��� ���� ����� �Ѵ�.
			sustain_pedal = 0;
			// �����̽��ٸ� ���� sustain���̿��� ��� key���� ������ ���� ���·� ���Ѵ�.
			// �ݺ����� ���� ��� �ǹ��� �˻��ؼ� sustain ���̰� ������ ���� ���� �ǹݵ��� flag�� �����ϰ� ���� �����ش�.
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
		// �ݺ����� ���� ��� �ǹ��� �˻��ؼ�, released�� Ű���忡 �����ϴ� �ǹ��� �ְ�
		// �� �ǹ��� ���� ������ ��� �ǹ��� flag�� �����ϰ�, ����� ������ �ʾ��� ��� ���� �����.
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


		if (perfect_timing > 0) { // perfect timing ���ӿ��� sustain pedal�� Ȱ��ȭ���� �ʴ´�.
			// �ݺ����� ���� ��� �ǹ��� �˻��ؼ� ������ �ǹ��� ã�´�.
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
		// double linked list�� ����� ��� ��ǥ ��ü�� �޸𸮸� �����Ѵ�.
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
	// sustain_pedal�� �ʱ⿡ ������ �����Ƿ� 0���� �ʱ�ȭ.
	sustain_pedal = 0;
	// key part
	
	// �ڷᱸ���� �ʱ�ȭ�Ѵ�.
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
	// �� �ǹ� �׸� - loop ���
	for (int i = 0; i < num_of_white - 1; i++) {
		ofDrawLine(tempx, y1, tempx, y2);
		tempx += white_x;
	}
	// ���� �ǹ� �׸� - loop ���
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
	for (int i = 0; i < num_of_white; i++) { // �� �ǹݿ� ���ؼ� �ݺ����� ����� �׷���
		//if (white_keypressedflag[i] == 1 || white_sustainflag[i] == 1) {
		if (white_keypressedflag[i] == 1) {
			ofDrawCircle(x1 + white_x / 2 + i * white_x, y2 - white_y / 6, white_r); // ������ ��ǥ�� ����� ���� ���� ����Ѵ�.
		}
	}

	for (int i = 0; i < num_of_black; i++) { // ���� �ǹݿ� ���� �ݺ����� ����� �׷���
		// if (black_keypressedflag[i] == 1 || black_sustainflag[i] == 1) { // black_key�� ��� �߰��߰� ���� �ǹ��� ������ �̰͵��� ���� ���� ������ ���� ���� ó������ �ʾҴ�.
		if (black_keypressedflag[i] == 1){
			ofDrawCircle(x1 + white_x * (i + 1), y2 - white_y / 2 - white_y / 60, black_r); // ������ ��ǥ�� ����� ���� ���� ����Ѵ�.
		}
	}
	if (sustain_pedal) ofDrawRectangle(ofGetWidth() / 2 - 2 * white_x, y2 + 25, 4 * white_x, 50); // sustain pedal�� ��� �簢���� ǥ���ߴ٤�.
}

void ofApp::print_window() { // string�� ȭ�鿡 ����ִ� �Լ�
	char str[200]; // myfont1���� ȭ�鿡 ������ ��Ÿ���� ���� str �����̴�

	ofSetColor(ofColor::darkGray);
	sprintf(str, " 20181277_ShinJaeyo_comsil_final_project ");
	myFont1.drawString(str, 15, ofGetHeight() - 5);

	// �ݺ����� ����� �� �ǹݿ� ���ڸ� ���
	ofSetColor(ofColor::black);
	for (int i = 0; i < num_of_white; i++) {
		//sprintf(str, white_key_name[i]);
		myFont_white.drawString(white_key_name[i], x1 + white_x * 1 / 3 + i * white_x, y2 - white_y / 7);
		// ������� ������ white_key_name���� for���� ����� ȿ�������� ���ڸ� print�Ѵ�.
	}
	myFont_white.drawString("Sustain pedal(Space bar)", ofGetWidth() / 2 - 1.5 * white_x, ofGetHeight() - 60);
	ofSetColor(ofColor::white);
	// �ݺ����� ����� ���� �ǹݿ� ���ڸ� ���
	for (int i = 0; i < num_of_black; i++) {
		//sprintf(str, black_key_name[i]);
		myFont_black.drawString(black_key_name[i], x1 + white_x * 13 / 16 + i * white_x, y2 - white_y / 2);
		// ������� ������ black_key_name���� for���� ����� ȿ�������� ���ڸ� print�Ѵ�.
		// ���� �ǹ��� ���� ��ġ�� black_key_name�� �� ���ڿ��̴� �ƹ��� ������ ����. �ϳ��� for������ ����ϰ� ������ �����ߴ�.
	}

	if (perfect_timing > 0) {
		ofSetColor(ofColor::forestGreen);
		sprintf(str, "PERFECT: %d / GOOD; %d / FAIL: %d\nCURRENT HP: %d", num_of_perfect, num_of_good, num_of_fail, max_num_of_fail - num_of_fail);
		// ���� ��ϰ� ���� hp�� ȭ�鿡 ������ش�.
		myFont2.drawString(str, ofGetWidth() / 2 + 4 * white_x, ofGetHeight() - 70);
	}
}
void ofApp::sound_load() {
	// �ʿ��� sound file�� load�ϴ� �Լ�
	intro.load("sounds/intro.mp3"); // loading ���� �� 1, �ƴϸ� 0
	
	
	// ���� Ȯ���� ����. �ʿ信 ���� �ٲ� �� �ִ�.
	char extension[] = ".wav";
	// �ǹ� �̸��� Ű�� ȭ�鿡 ����� �� ���� ���ڿ� ����
	char* temps = (char*)malloc(sizeof(int) * 20);
	
	// ������� ������ white_file_name, black_file_name�� strcpy, strcat �Լ��� ��θ� ���ڷ� ������
	// �ݺ����� ����� �� �ǹݵ鿡 ���� wav ������ load�Ѵ�.
	for (int i = 0; i < num_of_white; i++) {
		strcpy(temps, "sounds/");
		strcat(temps, white_file_name[i]);
		strcat(temps, extension);
		white_sound[i].load(temps);
		white_sound[i].setMultiPlay(false); // Ű�� �� ���� ��� ���� �ݺ� ����Ǵ� ���� ������ setMultiplay(false) ó���Ѵ�.
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
	intro.stop(); // intro�� ��� ���̶�� �����ش�.
	gamestart.play(); // ���� ���� ȿ������ ����Ѵ�
	// ���� �ʱ� ����
	play_piano = 0;
	perfect_timing = level;
	num_of_perfect = 0;
	num_of_good = 0;
	num_of_fail = 0;
	max_num_of_fail = level * 15;
	is_gameover = 0;
	

	for (int i = 0; i < level; i++) create_note();
	// level�� ���� �������� ���� ������ �þ. ���̵����� �ٸ� ������ �����ϱ� ���� �ݺ��� ���

	

	// result�� ���Ͽ��� �ҷ�����
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
	if (fp) { // ������ �ִٸ�
		int len;
		fscanf(fp, "%d", &len); // ����� ������ �޴´�
		maxheap = new result[len + 2]; // ����� ������ max heap�� ����
		maxheap_len = 0;
		// ���Ͽ� ����� ��ŷ�� ���̸�ŭ ������ �о, max heap�� ����
		for (int i = 1; i <= len; i++) {
			result tempresult;
			fscanf(fp, "%d %d", &tempresult.perfect, &tempresult.good);

			// max heap�� element �����ϴ� �κ�(push)
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
	// ���� �� �Ͻ������� �Ҵ�� Note���� �޸𸮸� ����. 
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
	
	delete[] maxheap; // heap�� ��� ���� init, end���� �޸𸮸� �ٸ��� �Ҵ��ϹǷ� ����
	maxheap_len = 0;
	perfect_timing = 0;
}

void ofApp::perfect_timing_gameover() {
	is_gameover = 1; // ���ӿ��� �������� ��Ÿ����
	gameover.play();
	
	// ������ ����� �� �����ִ� �ǹݵ��� �� �ֱ� ���� �ݺ��� ���.
	for (int i = 0; i < num_of_white; i++) {
		white_keypressedflag[i] = 0;
	}
	for (int j = 0; j < num_of_black; j++) {
		black_keypressedflag[j] = 0;
	}
	
	// ���� ������ ����� result�� �����ϱ� ���� maxheap�� push
	result tempresult;
	tempresult.perfect = num_of_perfect;
	tempresult.good = num_of_good;
	push_maxheap(tempresult);

	int i;

	// Top 5�� ����� result���� maxheap���� pop�ϴ� ������ ���� �ݺ��� ���
	for (i = 0; i < 5; i++) {
		if (maxheap_len < 1) break; // �ִ� ���� ���Ұ� ���ٸ� break
		
		// maxheap���� ���� pop ����
		result_print[i] = pop_maxheap();
	}

	// ���� maxheap�� 5������ ���� ������ �־��ٸ�, draw �� ���� �����ؼ� �������ش�.
	if (i != 5) {
		result_print[i].perfect = -1;
		// ���� draw �� maxheap[i].perfect�� -1�� ��� �� �κк��ʹ� ȭ�鿡 ������� �ʴ´�.
	}

	// ����� ��� result���� ���Ͽ� ���
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
		// ���� ����� �� perfect_timing�� �ݵ�� 1, 2, 3�� �ϳ��̴� �� ���� �� �ϳ��� �ƴ϶�� ������ ����ٴ� ���̴�.
	}
	fprintf(fp, "%d\n", i + maxheap_len); // ���Ͽ� ���� ����
	//cout << t + maxheap_len << endl;
	
	// �ռ� ȭ�� ��� ������ ���� �����ص� (�ִ�) 5���� ����� ���� ���Ͽ� ����.
	for (int j = 0; j < i; j++) {
		fprintf(fp, "%d %d\n", result_print[j].perfect, result_print[j].good);
		//cout << result_print[i].perfect << " " << result_print[i].good << endl;
	}
	// maxheap ���� result�� ��ŷ ������ ���Ͽ� ����ϱ� ���� �ݺ����� ����Ѵ�.
	int iter = maxheap_len; // �ݺ��� Ƚ��
	for (int i = 0; i < iter; i++) {
		if (maxheap_len < 1) break; // Ȥ�ó� ���Ұ� ���ٸ� break

		// maxheap���� ���� pop ����, �ݺ������� pop�� �� fprintf�� �ϴ� ������ txt ���Ͽ� ���ĵ� result���� ���.
		fprintf(fp, "%d %d\n", maxheap[1].perfect, maxheap[1].good); 
		// pop�� element�� ���Ͽ� ����Ѵ�. binary complete tree�� �̿������� 1 �ε����� ���� ���� result�� ������
		// ��ǻ� �� line���� pop�� ���̳� �ٸ�����. ���� �ε��� ������ ���� ���Ͽ� ����ϰ� pop �Լ��� ��ȯ���� �����ߴ�.
		pop_maxheap();
	}

	fclose(fp); // FP ����
}

void ofApp::perfect_timing_gameover_draw() {
	// ���ӿ��� �� ȭ���� ����Ѵ�.

	char str[200]; // ����
	ofFill();
	ofSetColor(ofColor::white);
	ofDrawRectangle(ofGetWidth() / 4, ofGetHeight() / 5, ofGetWidth() / 2, ofGetHeight() / 1.75);
	ofNoFill();
	ofSetColor(ofColor::black);
	ofDrawRectangle(ofGetWidth() / 4, ofGetHeight() / 5, ofGetWidth() / 2, ofGetHeight() / 1.75); // ���ӿ��� ���â �׸� �� �׸�ڽ�
	
	// ��� â�� string ���
	sprintf(str, "Game-Ooooooover!");
	myFont2.drawString(str, ofGetWidth() / 2 - 115, ofGetHeight() / 4 + 20);
	sprintf(str, "Result: PERFECT: %d GOOD: %d FAIL: %d", num_of_perfect, num_of_good, num_of_fail);
	myFont2.drawString(str, ofGetWidth() / 2 - 225, ofGetHeight() / 4 + 60);
	sprintf(str, "Press any key but ESC to continue");
	myFont2.drawString(str, ofGetWidth() / 2 - 195, ofGetHeight() / 4 + 100);

	myFont2.drawString("-----Top 5 results-----", ofGetWidth() / 2 - 130, ofGetHeight() * 4 / 9);
	// �ִ� 5���� ���� ����� ����ϱ� ���� �ݺ��� ���
	for (int i = 0; i < 5; i++) {
		if (result_print[i].perfect == -1) break;
		sprintf(str, "%d) PERFECT: %3d | GOOD: %3d", i + 1, result_print[i].perfect, result_print[i].good);
		myFont2.drawString(str, ofGetWidth() / 2 - 180, ofGetHeight() / 2 + 40 * (i + 1));
	}

}

void ofApp::create_note() {
	// note�� �����ؼ� ���� ����Ʈ�� �ִ´�.
	Note* temp = new Note;
	// ��� ���� �ʱ�ȭ
	temp->key = rand() % 2;
	int t;
	t = rand() % 3; // ��ǥ�� ���� ����
	temp->len = t; //  ��ǥ�� ���� ����
	temp->speed = 3.5 + 0.5 * (perfect_timing - 1); // ���� ���̵��� ���� �ӵ� ����
	temp->acce = (0.1 + (float)(rand() % 10) / 100) * (t + 1); // ��ǥ�� ������ �������� ������ ��ҵ� �Բ� ���ӵ� ����
	if (temp->key == 0) { // �� �ǹ��� 0, �� �ǹ��� �� �������� �ǹ� ����
		temp->x = rand() % num_of_white;
	}
	else {
		while (1) {
			// ���� �ǹ��� ���, ������ �� �Ǵ� x���� ������ �� ���� �����ϰ� ã�� ���� while ���
			t = rand() % num_of_black;
			if (t == 3 || t == 6 || t == 10 || t == 13 || t == 17) continue; // ���� �ǹ��� �� ���� �ִ�!
			temp->x = t;
			break;
		}
	}
	temp->y = 0;
	temp->is_calculated = 0;
	// double linked list�� ����
	temp->prev = score_rear->prev;
	temp->next = score_rear;
	score_rear->prev->next = temp;
	score_rear->prev = temp;
}

void ofApp::update_note() {
	// update() �Լ� ���ο��� ���� ���� �� ����Ǵ� �Լ���, �������� ��ǥ���� ������ ������Ʈ�Ѵ�.
	Note* temp = score_front->next; // front�� dummy
	Note* delnote;
	// ���� �������� ���� ��� ��ǥ���� Ȯ���ϱ� ���� �ݺ��� ���
	for (temp; temp != score_rear; ) {
		// y, �ӵ� ����
		temp->y += temp->speed;
		temp->speed += temp->acce;
		//int ty;
		if (temp->is_calculated == 1) { // �̹� ó���� ��Ʈ(��ǥ)�� ��� ������ ��� �Ѵ�
			delnote = temp;
			create_note(); // ����� ��Ʈ�� �ڸ��� ä�� �� ��Ʈ�� ���� ����Ʈ�� ����
			temp = temp->next;
			if (delnote->prev) delnote->prev->next = temp;
			temp->prev = delnote->prev;
			delete delnote;
		}
		else if (temp->y > y1 + (temp->speed * 6)) { //y1�� ���� �ӷ� �������� 6������ �� �̵��Ÿ����� �ؿ� ���� �� �� ��Ʈ�� �������.
			delnote = temp;
			num_of_fail++; // ���õ� ��ǥ�� ��� fail�� ����
			create_note(); // ����� ��Ʈ�� �ڸ��� ä�� �� ��Ʈ�� ���� ����Ʈ�� ����
			temp = temp->next;
			if (delnote->prev) delnote->prev->next = temp;
			temp->prev = delnote->prev;
			delete delnote;
			//cout << temp->x << endl;
		}
		else temp = temp->next;

		// ���ӿ��� ����
		if (!is_gameover && num_of_fail >= max_num_of_fail) { // gameover �Լ��� �� ���� ȣ��Ǿ�� �ϹǷ�, ���� ȣ����� �ʾ����鼭 ���ӿ��� ���� ���� �� ȣ��
			perfect_timing_gameover(); // max_num_of_fail���� ���� Ʋ���� gameover
		}
	}

}

void ofApp::perfect_timing_draw() {
	//ofSetColor(255, 255, 255, 0);
	//cout << score_front->x << " / " << score_front->y << endl;
	//cout << score_front->key << " len: " << score_front->len << endl;
	//ofSetColor(ofColor::grey);

	Note* temp = score_front->next;
	// ���� �������� ��ǥ���� �׸��� ���� �ݺ��� ���
	for (temp; temp != score_rear; temp = temp->next) {
		if (temp->key == 0) { //�� �ǹ��� ��, �׿� �°� ��ǥ�� ����ؼ� ��ǥ�� �׷��ش�
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
			ofFill(); // ���� �ǹ��� ȸ������ ����� �־� �˾ƺ��� ���� �Ѵ�.
			//notes[score_front->len].draw(0, 0);
			notes[temp->len].draw(temp->x * white_x + x1 + white_x * 3 / 4, temp->y, black_x, 50);
			//ofDrawCircle(score_front->x * white_x + x1 + white_x * 7 / 8, score_front->y, black_x);
			//update_note();
		}
	}
	

}

void ofApp::calculate_timing(int key_type, int key_num) {
	Note* temp = score_front->next;
	int is_fail = 0; // input key�� ���� �������� ��Ʈ�� �� ��� �Ϳ��� ���� �ʴ´ٸ� �߸� ���� �� -> �� ��� fail�� �ϳ� �ø���� �ߴ�.
	if (num_of_fail < max_num_of_fail) {
		// update_note���� ó������ ���� ��ǥ�� ���� ��� fail�� 1 �ø���. update_note�� gameover ������ �־� �Ѵ�.
		// �׷��� update_note�� draw �Ŀ� �̷����⿡ key �Է� -> calculate_timing -> draw -> update ����Ŭ���� 
		// calculate_timing���� num_of_fail�� �ʹ� Ŀ���µ��� draw �Լ� ���̶� gameover�� ���� �ʴ� ������ �־���.
		// calculate_timing �� num_of_fail�� max_num_of_fail���� Ŀ�� �� �����Ƿ� ���⼭�� gameover ������ �־�� �Ѵ�.

		// ���� �������� �ִ� ��� ��ǥ�鿡 ���� �Է� Ű�� �����ϴ��� Ȯ���ϱ� ���� �ݺ��� ���
		for (int i = 0; i < perfect_timing; i++, temp = temp->next) {
			if (temp->is_calculated == 1) continue; // �̹� ������ ������ ���ŵǾ��ٸ� �̰� �н�
			if (key_type == temp->key) { // �ǹ� ���� ������ ����� ��.
				if (key_num == temp->x) {
					is_fail = 1;
					temp->is_calculated = 1;
					// ������ ����: ��� �ӷ��� ���� �����Ӻ� �̵� �Ÿ� ���, 3������ ������ �� perfect, 6������ ������ �� good, �� ���� fail
					float perfect_range = (temp->speed + 1.5 * temp->acce) * 3;
					float good_range = (temp->speed + 3 * temp->acce) * 6;
					// y1�� �� �ǹ��� ����, �� ��ǥ�� ���� �κа� ����!
					if (abs(temp->y + 50 - y1) < perfect_range) {
						num_of_perfect++; // 50�� �������� ��ǥ �ڽ��� ������.
						max_num_of_fail++; // perfect�� timing�� ���� ���, HP�� 1 ����!
					}
					else if (abs(temp->y + 50 - y1) < good_range) num_of_good++; // good ����
					else num_of_fail++; // fail ����
					break; // �� ���� �ϳ����� ó���ؾ� �Ѵ�. ���� �ڸ��� 2���� �������� �ִ� ��찡 ���� �� �ִ�.
				}
			}
		}
		if (!is_fail) num_of_fail++; // �Է°� ���� �������� ��ǥ���� ���� ���� ���: fail++
	}
	else if (!is_gameover) perfect_timing_gameover(); // gameover ���ǿ� �����鼭 ���� �Լ��� ȣ����� ���� ��� ȣ��
}

void ofApp::push_maxheap(result tempresult) {
	int temp = ++maxheap_len;
	// complete binary tree�� ���� �� tempresult�� ���� ���� ���� �����ʿ� �ְ�, parent�� ���ذ��� �ڽ��� ��ġ�� ã�� ���� �ݺ���  ���.
	while (temp != 1) {
		// result �� ��� �񱳴�, perfect�� ��Ҹ� ���� ���ϰ� ���ٸ� good�� ��Ҹ� ���� �Ǻ��Ѵ�.
		if (tempresult.perfect > maxheap[temp / 2].perfect || (tempresult.perfect == maxheap[temp / 2].perfect && tempresult.good > maxheap[temp / 2].good)) {
			maxheap[temp] = maxheap[temp / 2];
			temp /= 2;
		}
		
		else break;
	}
	maxheap[temp] = tempresult;
}

result ofApp::pop_maxheap() {
	result returnresult = maxheap[1]; // pop�� element
	result tempresult = maxheap[maxheap_len--]; // complete binary tree �������� ���� ���� level�� ������ element
	//cout << maxheap_len << endl;
	int anc = 1;
	int des = 2;
	// ���� ���� ���Ұ� �ε��� 1�� �ִٰ� �����ϰ�, child�� ���ذ��� �ڽ��� �ڸ��� ã�� ����
	// �ݺ��� ���
	while (des <= maxheap_len) {
		if (des < maxheap_len && (maxheap[des + 1].perfect > maxheap[des].perfect || (maxheap[des + 1].perfect == maxheap[des].perfect && maxheap[des + 1].good > maxheap[des].good))) des++;
		// maxheap(max complete binary tree)���� des�� des+1 �� �켱������ �� ū ���� ���� �ϴ�, des+1�� �켱���ǰ� �� ���ٸ� des�� 1 ������
		if (tempresult.perfect > maxheap[des].perfect || (tempresult.perfect == maxheap[des].perfect && tempresult.good >= maxheap[des].good)) break;
		// tempresult�� �ڽ� �� level�� �� element���� �� ũ�Ƿ�, �� ��� tempresult�� ���� anc �ڸ��� ������ �ȴ�.

		maxheap[anc] = maxheap[des]; // �׷��� �ʴٸ� maxheap[des]�� �� �ܰ� �÷��� �Ѵ�.
		anc = des; // ���� �� Ž��
		des *= 2;
	}
	maxheap[anc] = tempresult; // pop �� heap �ٽ� ����.

	return returnresult;
}