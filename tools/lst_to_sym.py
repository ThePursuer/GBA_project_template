#!/usr/bin/env python3
import argparse
import re

def is_hex(s):
    # Check if a string is a hexadecimal number
    return re.fullmatch(r'[0-9A-Fa-f]+', s) is not None

def is_single_letter(s):
    # Check if a string is a single letter
    return len(s) == 1 and s.isalpha()

def process_line(parts):
    # Always take the first column as the address
    address = parts[0]
    
    # Check the second and potentially third columns to omit them under specified conditions
    start_index = 1
    if is_hex(parts[start_index]) or is_single_letter(parts[start_index]):
        start_index += 1
    if len(parts) > start_index and is_single_letter(parts[start_index]):
        start_index += 1

    # Join the remaining parts without space for the symbol name
    symbol = "".join(parts[start_index:])
    return address, symbol

def convert_to_sym(input_file, output_file):
    try:
        with open(input_file, 'r') as infile, open(output_file, 'w') as outfile:
            for line in infile:
                parts = line.strip().split()
                if(parts[-1] in ["__ewram_start", "__eheap_start", "__end__", "__ewram_end", "__sbss_end__", "__sbss_start__", "__eheap_end", "__iwram_start", "__iwram_start__"]):
                    continue
                if len(parts) > 1:  # Ensure there's at least an address
                    address, symbol = process_line(parts)
                    outfile.write(f"{address} {symbol}\n")
    except Exception as e:
        print(f"An error occurred: {e}")

def main():
    parser = argparse.ArgumentParser(description='Converts a given file format to .sym format.')
    parser.add_argument('input_file', type=str, help='Path to the input file')
    
    args = parser.parse_args()

    output_file = args.input_file.replace('.lst', '.sym')
    
    convert_to_sym(args.input_file, output_file)
    print(f"Conversion completed. Check {output_file} for the results.")

if __name__ == "__main__":
    main()
