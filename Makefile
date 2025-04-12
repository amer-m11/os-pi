
ARMGNU ?= aarch64-linux-gnu

# remove -mgeneral-regs-only in case NEON registers are needed (context switch)
COPS = -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only
ASMOPS = -Iinclude 

BUILD_DIR = build
SRC_DIR = src

all : kernel8.img

clean :
	rm -rf $(BUILD_DIR) *.img 

$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(COPS) -MMD -c $< -o $@

$(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
	$(ARMGNU)-gcc $(ASMOPS) -MMD -c $< -o $@

C_FILES = $(wildcard $(SRC_DIR)/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/*.S)
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)

DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

kernel8.img: $(SRC_DIR)/linker.ld $(OBJ_FILES)
	$(ARMGNU)-ld -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/kernel8.elf  $(OBJ_FILES)
	$(ARMGNU)-objcopy $(BUILD_DIR)/kernel8.elf -O binary kernel8.img


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
	rm -f ./kernel8.img
	@echo "Cleanup complete."