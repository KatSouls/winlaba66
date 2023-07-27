#include <iostream>
#include <windows.h>
using namespace std;

int m[25];// глобальный массив для вывода на экран

int turn; //Чья очередь?
int interested[2] = { 0,0 }; //все исходные значения равны 0 (FALSE)

void enter_region(int process) { /* process имеет значение 0 или 1 */
	int other; /*	номер другого процесса	*/
	other = 1 - process; /*	противостоящий процесс	*/
	interested[process] = TRUE; /* демонстрация заинтересованности */
	turn = process; /*	установка флажка */
	while (turn == process && interested[other] == TRUE) {}; 
}

void leave_region(int process) { /* процесс, покидающий критическую область */
	interested[process] = FALSE; /* признак выхода из критической области */
}

// функция потока
DWORD WINAPI ThreadFunc(LPVOID lpParam) {
	enter_region(1);
	for (int i = 0; i < 25; i++)
		cout << m[i] << " second ";// поэлементный вывод массива на экран
	cout << endl;
	leave_region(1);

	return 0;
}


int main() {
	SetConsoleOutputCP(1251);
	cout << "Вывод двух возрастающих последовательностей:"
		<< endl;
	for (int i = 0; i < 25; m[i++] = i); // массив заполняется рядом чисел
	HANDLE Th; // дескриптор потока
	// Создаем дополнительный поток и начинаем печатать последовательность
	Th = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

	enter_region(0);

	for (int i = 0; i < 25; i++)
		cout << m[i] << " first ";
	cout << endl;


	leave_region(0);

	WaitForSingleObject(Th, INFINITE);
	system("pause");
	return 0;
}

