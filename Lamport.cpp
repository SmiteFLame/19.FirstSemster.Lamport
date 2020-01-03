#include <thread>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

using namespace std;

#define PROCESSNUMBER 5

typedef struct {
	int number;
	int time;
	int stack;
}processStat;


bool choosing[PROCESSNUMBER] = { false };
int number[PROCESSNUMBER] = { 0 };

void MakeProcess(int _i, processStat** _processInfo) {
	_processInfo[_i] = (processStat*)malloc(sizeof(processStat));
	_processInfo[_i]->number = _i;
	// 프로세스마다 실행 되는 시간을 랜덤으로 부여 1초 ~ 10초
	_processInfo[_i]->time = (rand() % 100 + 10) * 10;
	_processInfo[_i]->stack = 0;
}

int MaxNumber() {
	int maxNum = 0;
	for (int i = 1; i < PROCESSNUMBER; i++)
		if (number[maxNum] < number[i])
			maxNum = i;

	return number[maxNum];
}


void Critical_Section(processStat* processInfo) {
	Sleep(processInfo->time);
	processInfo->stack++;
}

bool Check(int _j, int _i) {
	if (number[_j] < number[_i])
		return true;
	else if (number[_j] > number[_i])
		return false;
	else {
		if (_j < _i)
			return true;
		else
			return false;
	}
}

void LamportBakery(processStat *processInfo) {	
	do {
		clock_t st = clock();
		choosing[processInfo->number] = true;
		number[processInfo->number] = MaxNumber() + 1;
		choosing[processInfo->number] = false;
		for (int j = 0; j < PROCESSNUMBER; j++) {
			while (choosing[j]);
			while ((number[j] != 0) && (Check(j, processInfo->number)));
		}
		Critical_Section(processInfo);
		number[processInfo->number] = 0;
		cout << processInfo->number << "번 프로세스  " << processInfo->stack << "번째 실행" << endl;
		cout << "Critical Section " << float(processInfo->time) / 1000 << "초 걸림, 프로세스 대기시간 " << float(clock() - st) / 1000 << endl << endl;;

		// 프로세스가 끝나고 다음 프로세스가 실행될 때 까지 기다리는 시간 랜덤 1 ~ 10초
		Sleep((rand() % 10 + 1) * 1000);
	} while (true);
}


void main() {
	srand(time(NULL));
	processStat* processInfo[PROCESSNUMBER];

	// 프로세스의 기본 설정을 만든다. 스레드의 넘버와 걸리는 시간을 설정한다.
	for (int i = 0; i < PROCESSNUMBER; i++)
		MakeProcess(i, processInfo);

	// 스레드를 만들고 스레드를 실행시킨다. 스레드를 1초를 주기로하여 입력한다.
	thread Process[10];
	for (int i = 0; i < PROCESSNUMBER; i++) {
		Process[i] = thread(LamportBakery, processInfo[i]);
		Sleep(1000);
	}

	while (true);
}