#include <iostream>
#include <windows.h>
using namespace std;

int m[25]; // глобальный массив для вывода на экран
volatile LONG padlock = 0; //переменная для сравнени, пока 0-заблокирована

// функция потока
DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
    while (InterlockedCompareExchange(&padlock, 1, 0)) {} //функция создает полный барьер памяти,чтобы обеспечить выполнение операций с памятью по порядку
    //padlock-целевое значение, 1-значение обмена, 0-значение, сравниваемое с назначением
    //выполняет сравнение значения (обмен значениями осущ лишь в случае равенства сравнимых значений)
    //функция возвращает начальное значение параметра
    //Функция сравнивает значение padlock со значением 0
    //ЕСли целевое значение равно значению 0, значение 1 сохраняется в адресе, указанном назначением
    //В противном случае операция не выполняется
    for (int i = 0; i < 25; i++)
        cout << m[i] << " second "; // поэлементный вывод массива на экран
    cout << endl;
    InterlockedExchange(&padlock, 0); //функция создает полный барьер памяти,чтобы обеспечить выполнение операций с памятью по порядку
    //Функция InterlockedExchange сохраняет значение одной переменной в другой. 
   // InterlockedExchange(LPLONG Target, Value)
    return 0;
}

int main()
{
    setlocale(0, "ru");
    SetConsoleOutputCP(1501);
    cout << "Вывод двух возрастающих последовательностей:"
        << endl;
    for (int i = 0; i < 25; m[i++] = i);   // массив заполняется рядом чисел

    HANDLE Th;     // дескриптор потока
// Создаем дополнительный поток и начинаем печатать последовательность
    Th = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);
    while (InterlockedCompareExchange(&padlock, 1, 0)) {}
    for (int i = 0; i < 25; i++)
        cout << m[i] << " first ";
    cout << endl;
    InterlockedExchange(&padlock, 0);
    // дожидаемся завершения другого потока
    // результат WaitForSingleObject не важен
    WaitForSingleObject(Th, INFINITE);
    system("pause");
    return 0;
}