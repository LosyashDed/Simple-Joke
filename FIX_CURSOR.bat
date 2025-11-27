@echo off
:: CURSOR JOKE FIXER - Launcher
:: Запускает PowerShell скрипт для исправления курсора

cd /d "%~dp0"
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0fix_cursor.ps1"
