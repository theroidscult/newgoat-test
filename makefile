.PHONY: all iso run run-uefi

iso: ./kernel

ifeq ($(wildcard limine),)
	git clone https://github.com/limine-bootloader/limine.git --branch=v6.x-branch-binary --depth=1 --recurse-submodules
	cp limine/limine.h kernel/src/limine.h
	$(MAKE) -C limine
endif

	$(MAKE) -C kernel

	mkdir -p iso
	mkdir -p iso/EFI
	mkdir -p iso/EFI/BOOT
	mkdir -p iso/boot

	cp assets/limine.cfg limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin iso/
	cp kernel/bin/kernel iso/boot/kernel.bin
	cp assets/testdriver.adi iso/boot/
	cp limine/BOOT*.EFI iso/EFI/BOOT/

	 xorriso -as mkisofs -b limine-bios-cd.bin \
	 	-no-emul-boot -boot-load-size 4 -boot-info-table \
	 	--efi-boot limine-uefi-cd.bin \
	 	-efi-boot-part --efi-boot-image --protective-msdos-label \
	 	iso -o os.iso
	 limine/limine bios-install os.iso

QEMU_FLAGS = -cdrom os.iso -m 256M -machine q35 --boot order=d -display none
run: iso
	qemu-system-x86_64 $(QEMU_FLAGS) -serial stdio
run-uefi: iso
	qemu-system-x86_64 $(QEMU_FLAGS) -serial stdio -drive if=pflash,format=raw,readonly=on,file=/usr/share/ovmf/x64/OVMF.4m.fd

debug: iso
	qemu-system-x86_64 $(QEMU_FLAGS) -serial stdio -no-shutdown -no-reboot -d int
debug-uefi: iso
	qemu-system-x86_64 $(QEMU_FLAGS) -serial stdio -no-shutdown -no-reboot -d int -drive if=pflash,format=raw,readonly=on,file=/usr/share/ovmf/x64/OVMF.4m.fd
debug-gdb: iso
	qemu-system-x86_64 $(QEMU_FLAGS) -monitor stdio -no-shutdown -no-reboot -d int -s -S
debug-gdb-uefi: iso
	qemu-system-x86_64 $(QEMU_FLAGS) -monitor stdio -no-shutdown -no-reboot -d int -s -S -drive if=pflash,format=raw,readonly=on,file=/usr/share/ovmf/x64/OVMF.4m.fd


setup_clangd: clean
	bear -- ${MAKE} -C kernel

clean:
	$(MAKE) -C kernel clean
	rm -rf iso