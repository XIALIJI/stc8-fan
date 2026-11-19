# STC8 fan course build script (Keil C51 command line)
# usage:  powershell -NoProfile -ExecutionPolicy Bypass -File build.ps1 [exp]
#   exp = 1.1 1.2 2.1..2.8 3   (default 3)
param([string]$exp = "3")

$keil = "D:\keil5\C51\BIN"

$core = @("csp\cs_stc8h_timer.c", "util\stc8h_delay.c", "user\main.c", "user\task.c")

$extra = @{}
$extra["1.1"] = @("bsp\bs_sc8h25a_led.c", "example\1.1led_flow\user_led_flow.c")
$extra["1.2"] = @("bsp\bs_sc8h25a_led.c", "bsp\bs_sc8h25a_key.c", "example\1.2key_led\user_key_led.c")
$extra["2.1"] = @("csp\cs_stc8h_i2c_soft.c", "bsp\bs_sc8h25a_seg_tm1650.c", "bsp\bs_sc8h25a_temp_ds18b20.c", "example\2.1temp_seg\user_temp_seg.c")
$extra["2.2"] = @("csp\cs_stc8h_pwm.c", "csp\cs_stc8h_adc.c", "csp\cs_stc8h_i2c_soft.c", "bsp\m_motor.c", "bsp\bs_sc8h25a_key.c", "bsp\bs_sc8h25a_led.c", "bsp\bs_sc8h25a_seg_tm1650.c", "example\2.2fan_pot\user_fan_pot.c")
$extra["2.3"] = @("csp\cs_stc8h_i2c_soft.c", "bsp\m_sonar.c", "bsp\bs_sc8h25a_key.c", "bsp\bs_sc8h25a_led.c", "bsp\bs_sc8h25a_seg_tm1650.c", "example\2.3sonar_seg\user_sonar_seg.c")
$extra["2.4"] = @("csp\cs_stc8h_adc.c", "csp\cs_stc8h_i2c_soft.c", "bsp\bs_sc8h25a_key.c", "bsp\bs_sc8h25a_led.c", "bsp\bs_sc8h25a_seg_tm1650.c", "example\2.4light_fill\user_light_fill.c")
$extra["2.5"] = @("csp\cs_stc8h_i2c_soft.c", "bsp\m_servo.c", "bsp\bs_sc8h25a_key.c", "bsp\bs_sc8h25a_led.c", "bsp\bs_sc8h25a_seg_tm1650.c", "example\2.5servo_swing\user_servo_swing.c")
$extra["2.6"] = @("csp\cs_stc8h_i2c_soft.c", "bsp\bs_sc8h25a_led_ws2812b.c", "bsp\bs_sc8h25a_key.c", "bsp\bs_sc8h25a_seg_tm1650.c", "example\2.6ws2812b_level\user_ws2812b_level.c")
$extra["2.7"] = @("csp\cs_stc8h_eeprom.c", "csp\cs_stc8h_i2c_soft.c", "bsp\bs_sc8h25a_key.c", "bsp\bs_sc8h25a_led.c", "bsp\bs_sc8h25a_seg_tm1650.c", "example\2.7eeprom_temp\user_eeprom_temp.c")
$extra["2.8"] = @("csp\cs_stc8h_pwm.c", "csp\cs_stc8h_i2c_soft.c", "bsp\m_motor.c", "bsp\bs_sc8h25a_temp_ds18b20.c", "bsp\bs_sc8h25a_led_ws2812b.c", "bsp\bs_sc8h25a_key.c", "bsp\bs_sc8h25a_led.c", "bsp\bs_sc8h25a_seg_tm1650.c", "user\user_fan_manual.c")
$extra["3"]   = @("csp\cs_stc8h_pwm.c", "csp\cs_stc8h_adc.c", "csp\cs_stc8h_eeprom.c", "csp\cs_stc8h_i2c_soft.c",
                   "bsp\m_motor.c", "bsp\m_servo.c", "bsp\m_sonar.c",
                   "bsp\bs_sc8h25a_temp_ds18b20.c", "bsp\bs_sc8h25a_led_ws2812b.c", "bsp\bs_sc8h25a_key.c", "bsp\bs_sc8h25a_led.c", "bsp\bs_sc8h25a_seg_tm1650.c",
                   "user\user_fan_init.c", "user\user_fan_control.c", "user\user_fan_auto.c", "user\user_fan_pot.c", "user\user_fan_soft.c", "user\user_fan_light.c", "user\user_fan_swing.c", "user\user_fan_display.c", "user\user_fan_ws2812b.c", "user\user_fan_eeprom.c")

if (-not $extra.ContainsKey($exp)) { Write-Output "Unknown experiment: $exp"; exit 1 }

Set-Location $PSScriptRoot
$src = $core + $extra[$exp]
$out = "build\exp_" + $exp.Replace('.', '_')

# clean
Get-ChildItem -Path csp,bsp,util,user,example -Recurse -Include *.OBJ,*.LST -ErrorAction SilentlyContinue | Remove-Item -Force -ErrorAction SilentlyContinue
Remove-Item ($out + ".hex"), ($out + "*") -Force -ErrorAction SilentlyContinue

# assemble startup
& "$keil\A51.EXE" "keil_proj\STARTUP.A51" | Out-Null

# compile
foreach ($f in $src) {
  $r = (& "$keil\C51.EXE" $f "INCDIR(csp;bsp;util;user)" 2>&1 | Out-String)
  if ($r -notmatch "0 ERROR") { Write-Output "COMPILE FAIL: $f"; Write-Output $r; exit 1 }
}

# link
$objs = @("keil_proj\STARTUP.OBJ") + ($src -replace '\.c$', '.OBJ')
$objlist = ($objs -join ",")
$lr = (& "$keil\BL51.EXE" $objlist "TO" $out "RAMSIZE(256)" 2>&1 | Out-String)
if ($lr -notmatch "0 ERROR") { Write-Output "LINK FAIL ($exp)"; Write-Output $lr; exit 1 }

# hex
& "$keil\OH51.EXE" $out | Out-Null
if (Test-Path ($out + ".hex")) {
  $sz = (Get-Item ($out + ".hex")).Length
  Write-Output ("BUILD OK  " + $exp + "  ->  " + $out + ".hex  (" + $sz + " bytes)")
} else {
  Write-Output "HEX FAIL ($exp)"; exit 1
}
