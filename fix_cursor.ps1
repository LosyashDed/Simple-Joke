# CURSOR JOKE FIXER v1.0
# Исправление последствий cursor_joke

Write-Host ""
Write-Host "===========================================" -ForegroundColor Cyan
Write-Host "   CURSOR JOKE FIXER v1.0" -ForegroundColor Cyan
Write-Host "   Исправление последствий cursor_joke" -ForegroundColor Cyan
Write-Host "===========================================" -ForegroundColor Cyan
Write-Host ""

# Добавляем API для SystemParametersInfo
Add-Type @"
using System;
using System.Runtime.InteropServices;
public class CursorFix {
    [DllImport("user32.dll", SetLastError = true)]
    public static extern bool SystemParametersInfo(uint uiAction, uint uiParam, IntPtr pvParam, uint fWinIni);
    
    public const uint SPI_SETCURSORSIZE = 0x2029;
    public const uint SPI_SETMOUSESPEED = 0x0071;
    public const uint SPI_SETMOUSETRAILS = 0x005D;
    public const uint SPI_SETCURSORS = 0x0057;
    public const uint SPIF_UPDATEINIFILE = 0x01;
    public const uint SPIF_SENDCHANGE = 0x02;
}
"@

$fixed = 0
$errors = 0

# 1. Сброс размера курсора
Write-Host "[1/8] Сброс размера курсора..." -ForegroundColor Yellow
try {
    [CursorFix]::SystemParametersInfo(0x2029, 0, [IntPtr]32, 3) | Out-Null
    Write-Host "      OK - Размер курсора сброшен на 32" -ForegroundColor Green
    $fixed++
} catch {
    Write-Host "      ОШИБКА" -ForegroundColor Red
    $errors++
}

# 2. Сброс скорости мыши
Write-Host "[2/8] Сброс скорости мыши..." -ForegroundColor Yellow
try {
    [CursorFix]::SystemParametersInfo(0x0071, 0, [IntPtr]10, 3) | Out-Null
    Write-Host "      OK - Скорость мыши сброшена на 10" -ForegroundColor Green
    $fixed++
} catch {
    Write-Host "      ОШИБКА" -ForegroundColor Red
    $errors++
}

# 3. Отключение следа курсора
Write-Host "[3/8] Отключение следа курсора..." -ForegroundColor Yellow
try {
    [CursorFix]::SystemParametersInfo(0x005D, 0, [IntPtr]::Zero, 3) | Out-Null
    Write-Host "      OK - След курсора отключен" -ForegroundColor Green
    $fixed++
} catch {
    Write-Host "      ОШИБКА" -ForegroundColor Red
    $errors++
}

# 4. Сброс системных курсоров
Write-Host "[4/8] Сброс системных курсоров..." -ForegroundColor Yellow
try {
    [CursorFix]::SystemParametersInfo(0x0057, 0, [IntPtr]::Zero, 3) | Out-Null
    Write-Host "      OK - Курсоры сброшены" -ForegroundColor Green
    $fixed++
} catch {
    Write-Host "      INFO - Может потребоваться перезагрузка" -ForegroundColor Yellow
    $fixed++
}

# 5. Сброс настроек мыши в реестре
Write-Host "[5/8] Сброс настроек мыши в реестре..." -ForegroundColor Yellow
try {
    Set-ItemProperty -Path "HKCU:\Control Panel\Mouse" -Name "MouseTrails" -Value "0"
    Set-ItemProperty -Path "HKCU:\Control Panel\Mouse" -Name "MouseSensitivity" -Value "10"
    Set-ItemProperty -Path "HKCU:\Control Panel\Mouse" -Name "MouseSpeed" -Value "1"
    Set-ItemProperty -Path "HKCU:\Control Panel\Mouse" -Name "MouseThreshold1" -Value "6"
    Set-ItemProperty -Path "HKCU:\Control Panel\Mouse" -Name "MouseThreshold2" -Value "10"
    Write-Host "      OK - Настройки мыши сброшены" -ForegroundColor Green
    $fixed++
} catch {
    Write-Host "      ОШИБКА" -ForegroundColor Red
    $errors++
}

# 6. КРИТИЧЕСКИ ВАЖНО: Сброс SmoothMouse кривых
Write-Host "[6/8] Сброс SmoothMouse кривых (КРИТИЧЕСКИ ВАЖНО)..." -ForegroundColor Yellow
try {
    # Стандартные значения Windows 10
    $xCurve = [byte[]]@(
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    )
    $yCurve = [byte[]]@(
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00
    )
    
    Set-ItemProperty -Path "HKCU:\Control Panel\Mouse" -Name "SmoothMouseXCurve" -Value $xCurve -Type Binary
    Set-ItemProperty -Path "HKCU:\Control Panel\Mouse" -Name "SmoothMouseYCurve" -Value $yCurve -Type Binary
    Write-Host "      OK - SmoothMouse кривые сброшены" -ForegroundColor Green
    $fixed++
} catch {
    Write-Host "      ОШИБКА - $_" -ForegroundColor Red
    $errors++
}

# 7. Сброс настроек курсоров
Write-Host "[7/8] Сброс настроек курсоров..." -ForegroundColor Yellow
try {
    Set-ItemProperty -Path "HKCU:\Control Panel\Cursors" -Name "CursorBaseSize" -Value 32 -Type DWord
    Set-ItemProperty -Path "HKCU:\Control Panel\Cursors" -Name "(default)" -Value "Windows Aero"
    Set-ItemProperty -Path "HKCU:\Control Panel\Cursors" -Name "Scheme Source" -Value 2 -Type DWord
    Write-Host "      OK - Настройки курсоров сброшены" -ForegroundColor Green
    $fixed++
} catch {
    Write-Host "      ОШИБКА" -ForegroundColor Red
    $errors++
}

# 8. Отключение MouseKeys
Write-Host "[8/8] Отключение MouseKeys..." -ForegroundColor Yellow
try {
    Set-ItemProperty -Path "HKCU:\Control Panel\Accessibility\MouseKeys" -Name "Flags" -Value "0" -ErrorAction Stop
    Write-Host "      OK - MouseKeys отключены" -ForegroundColor Green
    $fixed++
} catch {
    Write-Host "      INFO - MouseKeys не найдены (это нормально)" -ForegroundColor Yellow
    $fixed++
}

# Итог
Write-Host ""
Write-Host "===========================================" -ForegroundColor Cyan
Write-Host "   РЕЗУЛЬТАТ:" -ForegroundColor Cyan
Write-Host "   Исправлено: $fixed из 8" -ForegroundColor White
if ($errors -gt 0) {
    Write-Host "   Ошибок: $errors" -ForegroundColor Red
}
Write-Host "===========================================" -ForegroundColor Cyan
Write-Host ""

if ($errors -eq 0) {
    Write-Host ">>> ВСЕ ИСПРАВЛЕНО УСПЕШНО! <<<" -ForegroundColor Green
    Write-Host ""
    Write-Host "РЕКОМЕНДАЦИЯ: Перезагрузите компьютер для" -ForegroundColor Yellow
    Write-Host "полного применения всех изменений." -ForegroundColor Yellow
} else {
    Write-Host ">>> ЕСТЬ ОШИБКИ! <<<" -ForegroundColor Red
    Write-Host ""
    Write-Host "Попробуйте запустить от имени администратора." -ForegroundColor Yellow
}

Write-Host ""
Write-Host "Нажмите любую клавишу для выхода..." -ForegroundColor Gray
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")

