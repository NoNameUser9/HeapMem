#include <windows.h>
#include <stdio.h>
#include <wchar.h>

void print(const wchar_t* message) {
    DWORD written;
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), message, lstrlenW(message), &written, NULL);
}

int main() {
    // Получаем идентификатор стандартного пула
    HANDLE hStdHeap = GetProcessHeap();
    if (!hStdHeap) {
        print(L"Ошибка получения стандартного пула!\n");
        return 1;
    }
    print(L"Идентификатор стандартного пула получен успешно.\n");

    // Определяем максимальный размер блока памяти в стандартном пуле (в wchar_t)
    SIZE_T maxSize = 512 * sizeof(wchar_t); // Пример: 512 символов
    print(L"Максимальный размер блока памяти в стандартном пуле: ");
    wchar_t buffer[100];
    swprintf(buffer, 100, L"%zu байт\n", maxSize);
    print(buffer);

    // Выделяем память в стандартном пуле
    LPVOID stdMem = HeapAlloc(hStdHeap, 0, maxSize);
    if (!stdMem) {
        print(L"Ошибка выделения памяти в стандартном пуле!\n");
        return 1;
    }

    // Заполняем стандартный пул тестовыми данными
    wcsncpy((wchar_t*)stdMem, L"Пример текста для передачи в динамический пул", maxSize / sizeof(wchar_t));
    print(L"Данные, записанные в стандартный пул:\n");
    print((wchar_t*)stdMem);
    print(L"\n");

    // Создаем динамический пул памяти максимально возможного размера
    HANDLE hDynHeap = HeapCreate(0, maxSize, 0);
    if (!hDynHeap) {
        print(L"Ошибка создания динамического пула!\n");
        HeapFree(hStdHeap, 0, stdMem);
        return 1;
    }
    print(L"Динамический пул памяти создан успешно.\n");

    // Выделяем память в динамическом пуле
    LPVOID dynMem = HeapAlloc(hDynHeap, 0, maxSize);
    if (!dynMem) {
        print(L"Ошибка выделения памяти в динамическом пуле!\n");
        HeapDestroy(hDynHeap);
        HeapFree(hStdHeap, 0, stdMem);
        return 1;
    }

    // Передаем данные из стандартного пула в динамический
    wmemcpy((wchar_t*)dynMem, (wchar_t*)stdMem, maxSize / sizeof(wchar_t));

    // Отображаем переданные данные
    print(L"Переданные данные из стандартного пула в динамический пул:\n");
    print((wchar_t*)dynMem);
    print(L"\n");

    // Освобождаем память
    HeapFree(hStdHeap, 0, stdMem);
    HeapFree(hDynHeap, 0, dynMem);
    HeapDestroy(hDynHeap);

    return 0;
}
