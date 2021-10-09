cd ../
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
export PATH="$HOME/opt/cross/bin:$PATH"
i686-elf-gcc -std=gnu99 -ffreestanding -g -c boot.s -o boot.o
mv boot.o compile/
cd src/
i686-elf-gcc -std=gnu99 -ffreestanding -g -c kernel.c -o kernel.o
mv kernel.o ../compile/
cd asm/
#nasm enableA20.s -f elf32 -o enableA20.o
#mv enableA20.o ../../compile/

cd ../../compile/
i686-elf-gcc -ffreestanding -nostdlib -g -T linker.ld boot.o kernel.o -o kernel.elf -lgcc
mv kernel.elf ../isoroot/boot/
cd ../
grub-mkrescue isoroot -o GameOS.iso
qemu-system-i386 -enable-kvm -m 2048M -cdrom GameOS.iso
