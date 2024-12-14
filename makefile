iso:
ifeq ($(wildcard limine),)
	git clone https://github.com/limine-bootloader/limine.git --branch=v7.x-binary --depth=1 --recurse-submodules
	cp limine/limine.h kernel/src/limine.h
	$(MAKE) -C limine
endif

	$(MAKE) -C kernel

	mkdir -p iso
	mkdir -p iso/EFI
	mkdir -p iso/EFI/BOOT
	mkdir -p iso/boot

	cp cfg/limine.cfg limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin iso/
	cp kernel/bin/kernel iso/boot/kernel.bin
	cp limine/BOOT*.EFI iso/EFI/BOOT/

	 xorriso -as mkisofs -b limine-bios-cd.bin \
	 	-no-emul-boot -boot-load-size 4 -boot-info-table \
	 	--efi-boot limine-uefi-cd.bin \
	 	-efi-boot-part --efi-boot-image --protective-msdos-label \
	 	iso -o os.iso
	 limine/limine bios-install os.iso

run: iso
	qemu-system-x86_64 -cdrom os.iso -m 256M -serial file:hornet.log -machine q35 --boot order=d
run-uefi: iso
	qemu-system-x86_64 -cdrom os.iso -m 256M -serial file:hornet.log -machine q35 --boot order=d -drive if=pflash,format=raw,readonly=on,file=/usr/share/ovmf/x64/OVMF.4m.fd

debug: iso
	qemu-system-x86_64 -cdrom os.iso -no-shutdown -no-reboot -serial stdio -d int -m 256M -machine q35 --boot order=d
debug-uefi: iso
	qemu-system-x86_64 -cdrom os.iso -no-shutdown -no-reboot -serial stdio -d int -m 256M -machine q35 --boot order=d -drive if=pflash,format=raw,readonly=on,file=/usr/share/ovmf/x64/OVMF.4m.fd
