#include <windows.h>
#include <string>

// Магическая константа для мгновенного обновления размера (Windows 10/11)
const UINT SPI_SETCURSORSIZE = 0x2029; 

// Точка входа WinMain гарантирует отсутствие консольного окна
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HKEY hKey;
    const char* subkey = "Software\\Microsoft\\Accessibility";
    const char* valueName = "CursorSize";
    DWORD data = 15; // Максимальный размер (1-15)

    // 1. Меняем значение в реестре
    LONG result = RegCreateKeyExA(HKEY_CURRENT_USER, subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL);
    
    if (result == ERROR_SUCCESS) {
        RegSetValueExA(hKey, valueName, 0, REG_DWORD, (const BYTE*)&data, sizeof(data));
        RegCloseKey(hKey);
    }

    // 2. Жёсткий пинок системе через спец. флаг 0x2029 (Гарантия работы на Win 10/11)
    // Именно это заставляет курсор вырасти мгновенно
    SystemParametersInfoA(SPI_SETCURSORSIZE, 0, (PVOID)15, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

    // 3. На всякий случай обновляем обычные курсоры тоже (для надежности)
    SystemParametersInfoA(SPI_SETCURSORS, 0, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

    return 0;
}