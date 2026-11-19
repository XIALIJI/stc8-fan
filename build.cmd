@echo off
rem STC8 fan - build wrapper (see build.ps1)
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0build.ps1" %*
