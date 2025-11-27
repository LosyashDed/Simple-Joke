#include <windows.h>
#include <stdio.h>

// Стандартные значения SmoothMouse кривых Windows 10
static const BYTE standardXCurve[40] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const BYTE standardYCurve[40] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00
};

int main() {
    printf("===========================================\n");
    printf("   CURSOR JOKE FIXER v1.0\n");
    printf("   Исправление последствий cursor_joke\n");
    printf("===========================================\n\n");

    int fixed = 0;
    int errors = 0;

    // 1. Сброс размера курсора (недокументированный параметр 0x2029)
    printf("[1/8] Сброс размера курсора...\n");
    if (SystemParametersInfoA(0x2029, 0, (PVOID)32, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE)) {
        printf("      OK - Размер курсора сброшен на 32\n");
        fixed++;
    } else {
        printf("      ОШИБКА - Не удалось сбросить размер курсора\n");
        errors++;
    }

    // 2. Сброс скорости мыши на 10 (стандартное значение)
    printf("[2/8] Сброс скорости мыши...\n");
    if (SystemParametersInfoA(SPI_SETMOUSESPEED, 0, (PVOID)10, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE)) {
        printf("      OK - Скорость мыши сброшена на 10\n");
        fixed++;
    } else {
        printf("      ОШИБКА - Не удалось сбросить скорость мыши\n");
        errors++;
    }

    // 3. Отключение следа курсора (MouseTrails)
    printf("[3/8] Отключение следа курсора...\n");
    if (SystemParametersInfoA(SPI_SETMOUSETRAILS, 0, 0, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE)) {
        printf("      OK - След курсора отключен\n");
        fixed++;
    } else {
        printf("      ОШИБКА - Не удалось отключить след курсора\n");
        errors++;
    }

    // 4. Сброс системных курсоров на стандартные
    printf("[4/8] Сброс системных курсоров...\n");
    if (SystemParametersInfoA(SPI_SETCURSORS, 0, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE)) {
        printf("      OK - Курсоры сброшены на стандартные\n");
        fixed++;
    } else {
        printf("      INFO - Курсоры могут потребовать перезагрузку\n");
        fixed++;
    }

    // 5. Сброс настроек мыши в реестре
    printf("[5/8] Сброс настроек мыши в реестре...\n");
    HKEY hMouseKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\Mouse", 0, KEY_SET_VALUE, &hMouseKey) == ERROR_SUCCESS) {
        // MouseTrails = 0
        const char* trails = "0";
        RegSetValueExA(hMouseKey, "MouseTrails", 0, REG_SZ, (const BYTE*)trails, 2);
        
        // MouseSensitivity = 10
        const char* sensitivity = "10";
        RegSetValueExA(hMouseKey, "MouseSensitivity", 0, REG_SZ, (const BYTE*)sensitivity, 3);
        
        // MouseSpeed = 1
        const char* speed = "1";
        RegSetValueExA(hMouseKey, "MouseSpeed", 0, REG_SZ, (const BYTE*)speed, 2);
        
        // MouseThreshold1 = 6
        const char* threshold1 = "6";
        RegSetValueExA(hMouseKey, "MouseThreshold1", 0, REG_SZ, (const BYTE*)threshold1, 2);
        
        // MouseThreshold2 = 10
        const char* threshold2 = "10";
        RegSetValueExA(hMouseKey, "MouseThreshold2", 0, REG_SZ, (const BYTE*)threshold2, 3);
        
        RegCloseKey(hMouseKey);
        printf("      OK - Настройки мыши в реестре сброшены\n");
        fixed++;
    } else {
        printf("      ОШИБКА - Не удалось открыть реестр мыши\n");
        errors++;
    }

    // 6. КРИТИЧЕСКИ ВАЖНО: Сброс SmoothMouse кривых
    printf("[6/8] Сброс SmoothMouse кривых (КРИТИЧЕСКИ ВАЖНО)...\n");
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\Mouse", 0, KEY_SET_VALUE, &hMouseKey) == ERROR_SUCCESS) {
        LONG result1 = RegSetValueExA(hMouseKey, "SmoothMouseXCurve", 0, REG_BINARY, standardXCurve, sizeof(standardXCurve));
        LONG result2 = RegSetValueExA(hMouseKey, "SmoothMouseYCurve", 0, REG_BINARY, standardYCurve, sizeof(standardYCurve));
        RegCloseKey(hMouseKey);
        
        if (result1 == ERROR_SUCCESS && result2 == ERROR_SUCCESS) {
            printf("      OK - SmoothMouse кривые сброшены на стандартные\n");
            fixed++;
        } else {
            printf("      ОШИБКА - Не удалось сбросить SmoothMouse кривые\n");
            errors++;
        }
    } else {
        printf("      ОШИБКА - Не удалось открыть реестр\n");
        errors++;
    }

    // 7. Сброс настроек курсоров
    printf("[7/8] Сброс настроек курсоров...\n");
    HKEY hCursorKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\Cursors", 0, KEY_SET_VALUE, &hCursorKey) == ERROR_SUCCESS) {
        DWORD cursorSize = 32;
        RegSetValueExA(hCursorKey, "CursorBaseSize", 0, REG_DWORD, (const BYTE*)&cursorSize, sizeof(DWORD));
        
        // Стандартная схема
        const char* scheme = "Windows Aero";
        RegSetValueExA(hCursorKey, "", 0, REG_SZ, (const BYTE*)scheme, (DWORD)strlen(scheme) + 1);
        
        DWORD schemeSource = 2;
        RegSetValueExA(hCursorKey, "Scheme Source", 0, REG_DWORD, (const BYTE*)&schemeSource, sizeof(DWORD));
        
        RegCloseKey(hCursorKey);
        printf("      OK - Настройки курсоров сброшены\n");
        fixed++;
    } else {
        printf("      ОШИБКА - Не удалось открыть реестр курсоров\n");
        errors++;
    }

    // 8. Отключение MouseKeys (специальные возможности)
    printf("[8/8] Отключение MouseKeys...\n");
    HKEY hAccessKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\Accessibility\\MouseKeys", 0, KEY_SET_VALUE, &hAccessKey) == ERROR_SUCCESS) {
        const char* flags = "0";
        RegSetValueExA(hAccessKey, "Flags", 0, REG_SZ, (const BYTE*)flags, 2);
        RegCloseKey(hAccessKey);
        printf("      OK - MouseKeys отключены\n");
        fixed++;
    } else {
        printf("      INFO - MouseKeys не найдены (это нормально)\n");
        fixed++;
    }

    // Итог
    printf("\n===========================================\n");
    printf("   РЕЗУЛЬТАТ:\n");
    printf("   Исправлено: %d из 8\n", fixed);
    if (errors > 0) {
        printf("   Ошибок: %d\n", errors);
    }
    printf("===========================================\n\n");

    if (errors == 0) {
        printf(">>> ВСЕ ИСПРАВЛЕНО УСПЕШНО! <<<\n\n");
        printf("РЕКОМЕНДАЦИЯ: Перезагрузите компьютер для\n");
        printf("полного применения всех изменений.\n\n");
    } else {
        printf(">>> ЕСТЬ ОШИБКИ! <<<\n\n");
        printf("Попробуйте запустить программу\n");
        printf("от имени администратора.\n\n");
    }

    printf("Нажмите любую клавишу для выхода...\n");
    getchar();
    
    return errors;
}

