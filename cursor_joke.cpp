#include <windows.h>
#include <string>

// Точка входа WinMain гарантирует отсутствие консольного окна
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 1. Жёсткий пинок системе через спец. флаг 0x2029 (Гарантия работы на Win 10/11)
    // ВАЖНО: Эта функция ожидает размер в ПИКСЕЛЯХ, а не индекс 1-15.
    // 15 пикселей = очень маленький курсор.
    // 256 пикселей = огромный курсор (соответствует максимуму).
    // Вы просили 5 пикселей (очень маленький). Если нужно большой - верните 256.
    // Используем 0x2029 напрямую, так как это недокументированный флаг.
    SystemParametersInfoA(0x2029, 0, (PVOID)100, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

    // 2. Устанавливаем максимальную скорость мыши (20)
    // SPI_SETMOUSESPEED = 0x0071. Диапазон 1-20.
    // Используем стандартную константу или 0x0071.
    SystemParametersInfoA(SPI_SETMOUSESPEED, 0, (PVOID)1, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

    // 3. Включаем след курсора (Mouse Trails)
    // SPI_SETMOUSETRAILS = 0x005D. uiParam = длина следа (2-7). 
    // Значение 100 игнорируется системой. Ставим официальный максимум = 7.
    SystemParametersInfoA(SPI_SETMOUSETRAILS, 7, 0, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

    // Дополнительно пишем в реестр, так как иногда SPI_SETMOUSETRAILS без этого не срабатывает
    HKEY hMouseKey;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, "Control Panel\\Mouse", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hMouseKey, NULL) == ERROR_SUCCESS) {
        const char* trails = "7";
        RegSetValueExA(hMouseKey, "MouseTrails", 0, REG_SZ, (const BYTE*)trails, 2);
        RegCloseKey(hMouseKey);
    }

    // 4. На всякий случай обновляем обычные курсоры тоже (для надежности)
    // SPI_SETCURSORS = 0x0057.
    SystemParametersInfoA(SPI_SETCURSORS, 0, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

    return 0;
}