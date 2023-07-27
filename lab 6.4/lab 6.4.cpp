#include <iostream>
#include <windows.h>
using namespace std;

int m[25]; // глобальный массив для вывода на экран

DWORD iVal = 0; //iVal - переменная для потока
HANDLE mutex = NULL; //HANDLE - дескриптор

// функция потока
DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
    while (iVal < 2) {
        iVal++;
        WaitForSingleObject(mutex, INFINITE); //INFINITE - время ожидания (по умолчанию)
        for (int i = 0; i < 25; i++) {
            cout << m[i] << " "; // поэлементный вывод массива на экран
        }
        ReleaseMutex(mutex); //освобождение объекта
    }
    cout << endl;
    return 0;
}

int main()
{
    mutex = CreateMutex(NULL, FALSE, L"Same name");
    //CreateMutex( LPSECURITY_ATTRIBUTE lpMutexAttributes, BOOL bInitialOwner, LPCTSTR lpName);

    SetConsoleOutputCP(1251);
    cout << "Вывод двух возрастающих последовательностей:"
        << endl;
    for (int i = 0; i < 25; m[i++] = i);   // массив заполняется рядом чисел

    HANDLE Th;          // дескриптор потока
// Создаем дополнительный поток и начинаем печатать последовательность
    Th = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

    while (iVal < 2) {
        iVal++;
        //ожидаем захвата мьютекса
        WaitForSingleObject(mutex, INFINITE); //ожидание окончания потока
        for (int i = 0; i < 25; i++) { //// мьютекс захвачен – реализуем код критической секции
            cout << m[i] << " ";
        }
        ReleaseMutex(mutex); //освобождение мьютекса
    }
    cout << endl;
    // дожидаемся завершения другого потока
    // результат WaitForSingleObject не важен
    WaitForSingleObject(Th, INFINITE);

    system("pause");
    return 0;
}