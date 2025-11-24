#include <windows.h>
#include <string>

// Магическая константа для мгновенного обновления размера (Windows 10/11)
const UINT SPI_SETCURSORSIZE = 0x2029; 

// Точка входа WinMain гарантирует отсутствие консольного окна
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 2. Жёсткий пинок системе через спец. флаг 0x2029 (Гарантия работы на Win 10/11)
    // ВАЖНО: Эта функция ожидает размер в ПИКСЕЛЯХ, а не индекс 1-15.
    // 15 пикселей = очень маленький курсор.
    // 256 пикселей = огромный курсор (соответствует максимуму).
    SystemParametersInfoA(SPI_SETCURSORSIZE, 0, (PVOID)5, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

    // 3. На всякий случай обновляем обычные курсоры тоже (для надежности)
    SystemParametersInfoA(SPI_SETCURSORS, 0, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

    return 0;
}