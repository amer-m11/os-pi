build:
	@echo "Building kernel..."
	rm -rf ./output
	mkdir -p ./output
	aarch64-elf-as -c ./src/boot.S -o ./output/boot.o
	aarch64-elf-gcc -ffreestanding -c ./src/kernel.c -o ./output/kernel.o -O2 -Wall -Wextra
	aarch64-elf-gcc -T ./src/linker.ld -o ./output/myos.elf -ffreestanding -O2 -nostdlib ./output/boot.o ./output/kernel.o -lgcc 
	aarch64-elf-objcopy ./output/myos.elf -O binary ./output/kernel8.img
	@echo "Kernel build complete."

clean:
	@echo "Cleaning up..."
	rm -rf ./output
	@echo "Cleanup complete."