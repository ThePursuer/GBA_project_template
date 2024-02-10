#/usr/bin/env python3

import argparse
import math

def generate_magic_numbers(fixed_precision, largest_divisor, step, output_precision):
    magic_numbers = []
    for divisor in range(1, int((largest_divisor + 1) / step)):
        magic_number = int(((1 << fixed_precision) << output_precision) / (divisor * step)) + 1
        magic_numbers.append(magic_number)
    return magic_numbers

def write_cpp_files(magic_numbers, signed, data_type, output_precision):
    data_type_str = "int32_t" if data_type == 32 else "int16_t"
    if not signed:
        data_type_str = f"u{data_type_str}"
    array_name = "division_table"

    # Write the source file
    with open(f"{array_name}.cpp", "w") as cpp_file:
        cpp_file.write(f"#include \"{array_name}.h\"\n\n")

        cpp_file.write(f"{data_type_str} {array_name}[{len(magic_numbers)}] = {{\n")
        for i in range(0, len(magic_numbers), 10):
            line = ", ".join(str(num) for num in magic_numbers[i:i+10])
            cpp_file.write(f"    {line},\n")
        cpp_file.write("};\n")
        cpp_file.flush()

    print("Writing header file now...")
    # Write the header file
    with open(f"{array_name}.h", "w") as header_file:
        guard_name = f"{array_name.upper()}_H"
        header_file.write(f"#ifndef {guard_name}\n#define {guard_name}\n\n")
        header_file.write(f"#include <cstdint>\n\n")
        if (output_precision != 0):
            header_file.write(f"#define DIVISION_TABLE_PRECISION {output_precision}\n\n")
        header_file.write(f"extern {data_type_str} {array_name}[{len(magic_numbers)}];\n\n")
        header_file.write("#endif\n")
        header_file.flush()

def main():
    parser = argparse.ArgumentParser(description="Generate magic numbers for fast division.")
    parser.add_argument("fixed_precision", type=int, help="Number of bits for the fraction.")
    parser.add_argument("largest_divisor", type=int, help="Largest divisor to handle.")
    parser.add_argument("step", type=int, help="The power of 2 used to calculate the step. ie: step = 1/2^<val>")
    parser.add_argument("--signed", action="store_true", help="Generate signed magic numbers.")
    parser.add_argument("--data-type", type=int, choices=[16, 32], default=32, help="Data type of the output values (16 or 32 bits).")
    parser.add_argument("--op", type=int, default=0, help="Fixed precision of the output values. Each value is shiftedby this amount. Default is 0.")

    args = parser.parse_args()

    step = float(1 / (2 ** args.step))

    magic_numbers = generate_magic_numbers(args.fixed_precision, args.largest_divisor, step, args.op)

    # Janky check to make sure we didn't overflow the data type
    if(args.data_type == 16 and args.signed and magic_numbers[0] > 32767):
        print(f"Magic number {magic_numbers[0]} is too large for 16 bit signed integer. Try again with different settings (lower output precision or larger step.). Exiting...")
        return 1
    if(args.data_type == 32 and args.signed and magic_numbers[0] > 2147483647):
        print(f"Magic number {magic_numbers[0]} is too large for 32 bit signed integer. Try again with different settings (lower output precision or larger step.). Exiting...")
        return 1
    if(args.data_type == 16 and not args.signed and magic_numbers[0] > 65535):
        print(f"Magic number {magic_numbers[0]} is too large for 16 bit unsigned integer. Try again with different settings (lower output precision or larger step.). Exiting...")
        return 1
    if(args.data_type == 32 and not args.signed and magic_numbers[0] > 4294967295):
        print(f"Magic number {magic_numbers[0]} is too large for 32 bit unsigned integer. Try again with different settings (lower output precision or larger step.). Exiting...")
        return 1
    
    write_cpp_files(magic_numbers, args.signed, args.data_type, args.op)

if __name__ == "__main__":
    main()
