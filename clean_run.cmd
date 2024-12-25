REM work-around as esphome clean seems to be bugged in powershell on win11
powershell -Command "Remove-Item -Recurse -Force .\.esphome\build\siebenuhr-mini\.piolibdeps\siebenuhr-mini\siebenuhr_core"
esphome run .\local.yaml