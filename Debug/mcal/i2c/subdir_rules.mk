################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
mcal/i2c/%.obj: ../mcal/i2c/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="F:/SprintS/Weeks/week 11/Arm Projects/I2C" --include_path="F:/SprintS/Weeks/week 11/Arm Projects/I2C/mcal/SysCtr" --include_path="F:/SprintS/Weeks/week 11/Arm Projects/I2C/appl" --include_path="F:/SprintS/Weeks/week 11/Arm Projects/I2C/config" --include_path="F:/SprintS/Weeks/week 11/Arm Projects/I2C/mcal" --include_path="F:/SprintS/Weeks/week 11/Arm Projects/I2C/mcal/dio" --include_path="F:/SprintS/Weeks/week 11/Arm Projects/I2C/mcal/i2c" --include_path="F:/SprintS/Weeks/week 11/Arm Projects/I2C/mcal/nvic" --include_path="F:/SprintS/Weeks/week 11/Arm Projects/I2C/mcal/port_driver" --include_path="F:/SprintS/Weeks/week 11/Arm Projects/I2C/utils" --include_path="C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="mcal/i2c/$(basename $(<F)).d_raw" --obj_directory="mcal/i2c" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


