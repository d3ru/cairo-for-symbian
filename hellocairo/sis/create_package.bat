@echo off
if "%1"=="udeb" goto udeb

call createsis create -cert cert-gen.cer -key key-gen.key -pass abcd hellocairo_gcce.pkg
goto end

:udeb
createsis create -cert cert-gen.cer -key key-gen.key -pass abcd hellocairo_gcce_udeb.pkg

:end
