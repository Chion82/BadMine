#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <time.h>

HWND mineFrameHwnd = NULL;

using namespace std;

vector<string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while(getline(ss, tok, delimiter)) {
	internal.push_back(tok);
	}

	return internal;
}

void drawPixel(int x, int y) {
	int x_init = 20, y_init = 60;
	int x_offset = x_init + x*16;
	int y_offset = y_init + y*16;
	PostMessage(mineFrameHwnd, WM_RBUTTONDOWN , MK_RBUTTON,MAKELPARAM(x_offset,y_offset));
	PostMessage(mineFrameHwnd, WM_RBUTTONUP , MK_RBUTTON,MAKELPARAM(x_offset,y_offset));
	//PostMessage(mineFrameHwnd, WM_RBUTTONDOWN , MK_RBUTTON,MAKELPARAM(x_offset,y_offset));
	//PostMessage(mineFrameHwnd, WM_RBUTTONUP , MK_RBUTTON,MAKELPARAM(x_offset,y_offset));
}

void restorePixel(int x, int y) {
	int x_init = 20, y_init = 60;
	int x_offset = x_init + x*16;
	int y_offset = y_init + y*16;
	PostMessage(mineFrameHwnd, WM_RBUTTONDOWN , MK_RBUTTON,MAKELPARAM(x_offset,y_offset));
	PostMessage(mineFrameHwnd, WM_RBUTTONUP , MK_RBUTTON,MAKELPARAM(x_offset,y_offset));
	PostMessage(mineFrameHwnd, WM_RBUTTONDOWN , MK_RBUTTON,MAKELPARAM(x_offset,y_offset));
	PostMessage(mineFrameHwnd, WM_RBUTTONUP , MK_RBUTTON,MAKELPARAM(x_offset,y_offset));
}

int statusTable[16][30];

void drawFrame(string frameSource) {
	vector<string> lines = split(frameSource, '\n');
	for (int line=0;line<lines.size();line++) {
		string lineData = lines[line];
		for (int i=0;i<30;i++) {
			if (lineData.at(i) == '0' && statusTable[line][i] == 0) {
				drawPixel(i, line);
				statusTable[line][i] = 1;
			}
			if (lineData.at(i) == '1' && statusTable[line][i] == 1) {
				restorePixel(i, line);
				statusTable[line][i] = 0;
			}
		}
	}
}

long getTime() {
	SYSTEMTIME st;
	GetSystemTime(&st);
	return ((long)time(NULL))*1000 + st.wMilliseconds;
}


int main() {
	mineFrameHwnd = FindWindow(NULL, "扫雷");
	if (mineFrameHwnd == NULL) {
		printf("Cannot find main frame\n");
		system("pause");
		return 0;
	}
	
	for (int i=0;i<16;i++) {
		for (int k=0;k<30;k++) {
			statusTable[i][k] = 0;
		}
	}

	char* readBuf = (char*)malloc(2000000);
	FILE* txtFile = fopen("play.txt", "r");
	fread(readBuf,1,2000000, txtFile);

	string sourceString = string(readBuf);
	vector<string> frames = split(sourceString, '\t');

	long initTime = getTime();
	while (getTime() <= initTime + 218*1000) {
		printf("Drawing frame at time %d \n" , getTime());
		drawFrame(frames[(int)((getTime()-initTime)/100)]);
		Sleep(50);
	}

	free(readBuf);
	system("pause");
	return 0;
}


