#include <iostream>
#include <windows.h>
using namespace std;

int m[25]; // глобальный массив для вывода на экран

CRITICAL_SECTION cs;

// функция потока
DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
    for (int i = 0; i < 25; i++) {
        EnterCriticalSection(&cs); // вход в критическую секцию
        cout << m[i] << ", "; // поэлементный вывод массива на экран
        LeaveCriticalSection(&cs); //выход из критической секции
    }
    cout << endl;
    return 0;
}

int main()
{
    SetConsoleOutputCP(1251);
    InitializeCriticalSection(&cs); //Инициализировать объявленную переменную
    cout << "Вывод двух возрастающих последовательностей:"
        << endl;
    for (int i = 0; i < 25; m[i++] = i);   // массив заполняется рядом чисел

    HANDLE Th;          // дескриптор потока
// Создаем дополнительный поток и начинаем печатать последовательность
    Th = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

    for (int i = 0; i < 25; i++) {
        EnterCriticalSection(&cs); // вход в критическую секцию
        cout << m[i] << ", ";// поэлементный вывод массива на экран
        LeaveCriticalSection(&cs);//выход из критической секции
    }
    cout << endl;
    // дожидаемся завершения другого потока
    // результат WaitForSingleObject не важен
    WaitForSingleObject(Th, INFINITE); //дожидание всех потоков
    system("pause");
    DeleteCriticalSection(&cs); //разрушение объекта критическая секция (DeleteCriticalSection( LPCRITICAL_SECTION lpCriticalSection); )
    return 0;
}