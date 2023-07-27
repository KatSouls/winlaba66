﻿#include <iostream>
#include <windows.h>
using namespace std;

int m[25]; // глобальный массив для вывода на экран

HANDLE semaphore;

// функция потока
DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
    WaitForSingleObject(semaphore, INFINITE);
    for (int i = 0; i < 25; i++)
        cout << m[i] << " second "; // поэлементный вывод массива на экран
    cout << endl;
    ReleaseSemaphore(semaphore, 1, 0);
    return 0;
}

int main()
{
    semaphore = CreateSemaphore(NULL, 1, 1, L"same name");
    SetConsoleOutputCP(1251);
    cout << "Вывод двух возрастающих последовательностей:"
        << endl;
    for (int i = 0; i < 25; m[i++] = i);   // массив заполняется рядом чисел

    HANDLE Th;          // дескриптор потока
// Создаем дополнительный поток и начинаем печатать последовательность
    Th = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

    WaitForSingleObject(semaphore, INFINITE);
    for (int i = 0; i < 25; i++)
        cout << m[i] << " first ";
    cout << endl;
    ReleaseSemaphore(semaphore, 1, 0);
    // дожидаемся завершения другого потока
    // результат WaitForSingleObject не важен
    WaitForSingleObject(Th, INFINITE);
    CloseHandle(semaphore);
    system("pause");
    return 0;
}