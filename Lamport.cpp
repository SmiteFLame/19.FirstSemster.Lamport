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
	// ���μ������� ���� �Ǵ� �ð��� �������� �ο� 1�� ~ 10��
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
		cout << processInfo->number << "�� ���μ���  " << processInfo->stack << "��° ����" << endl;
		cout << "Critical Section " << float(processInfo->time) / 1000 << "�� �ɸ�, ���μ��� ���ð� " << float(clock() - st) / 1000 << endl << endl;;

		// ���μ����� ������ ���� ���μ����� ����� �� ���� ��ٸ��� �ð� ���� 1 ~ 10��
		Sleep((rand() % 10 + 1) * 1000);
	} while (true);
}


void main() {
	srand(time(NULL));
	processStat* processInfo[PROCESSNUMBER];

	// ���μ����� �⺻ ������ �����. �������� �ѹ��� �ɸ��� �ð��� �����Ѵ�.
	for (int i = 0; i < PROCESSNUMBER; i++)
		MakeProcess(i, processInfo);

	// �����带 ����� �����带 �����Ų��. �����带 1�ʸ� �ֱ���Ͽ� �Է��Ѵ�.
	thread Process[10];
	for (int i = 0; i < PROCESSNUMBER; i++) {
		Process[i] = thread(LamportBakery, processInfo[i]);
		Sleep(1000);
	}

	while (true);
}