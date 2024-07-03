@echo off

:: Compile kernel and create ISO
call ./iso.bat

:: Run qemu (serial output to both stdio and serial.log)

:: Default
@REM qemu-system-x86_64 -cdrom protOS.iso -chardev stdio,id=char0,logfile=serial.log,signal=off -serial chardev:char0

:: Monitor enabled
@REM qemu-system-x86_64 -cdrom protOS.iso -monitor stdio -no-shutdown -no-reboot -serial file:serial.log

:: Interrupt debugging
qemu-system-x86_64 -cdrom protOS.iso -serial file:serial.log -no-reboot -d int -D qemu.log