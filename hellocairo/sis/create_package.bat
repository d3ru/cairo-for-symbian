@echo off
if "%1"=="udeb" goto udeb
if "%1"=="armv5" goto armv5

call createsis create -cert cert-gen.cer -key key-gen.key -pass abcd hellocairo_gcce.pkg
goto end

:armv5
call createsis create -cert cert-gen.cer -key key-gen.key -pass abcd hellocairo_armv5.pkg
goto end

:udeb
createsis create -cert cert-gen.cer -key key-gen.key -pass abcd hellocairo_gcce_udeb.pkg

:end
