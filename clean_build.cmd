REM work-around as esphome clean seems to be bugged in powershell on win11
powershell -Command "Remove-Item -Recurse -Force .\.esphome"
esphome compile .\local.yaml