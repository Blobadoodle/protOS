@echo off

:: Compile the kernel
wsl make clean
wsl make

:: Delete previous iso_root and created iso
rmdir .\iso_root /s /q
del protOS.iso

:: Copy Limine system files
mkdir .\iso_root\boot\limine
copy .\bin\kernel.elf .\iso_root\boot
copy .\limine.cfg .\iso_root\boot\limine
copy .\limine\limine-bios.sys .\iso_root\boot\limine
copy .\limine\limine-bios-cd.bin .\iso_root\boot\limine
copy .\limine\limine-uefi-cd.bin .\iso_root\boot\limine
copy .\u_vga16.sfn .\iso_root\boot

:: Copy efi boot file for Limine
mkdir .\iso_root\EFI\BOOT
copy .\limine\BOOTX64.EFI .\iso_root\EFI\BOOT
copy .\limine\BOOTIA32.EFI .\iso_root\EFI\BOOT

:: Create limine ISO using the contents of iso_root
wsl xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot boot/limine/limine-uefi-cd.bin -efi-boot-part --efi-boot-image --protective-msdos-label iso_root -o protOS.iso

:: Install the MBR to the iso
wsl ./limine/limine bios-install protOS.iso