#!/usr/bin/env python3
import argparse
import subprocess
import sys

def parse_args():
    parser = argparse.ArgumentParser(description='Find specified symbols in the IWRAM section of an object file or executable.')
    parser.add_argument('input_file', help='Path to the input file (object file or executable).')
    parser.add_argument('--symbols', nargs='+', help='List of symbols to search for in the IWRAM section.')

    return parser.parse_args()

def main():
    args = parse_args()
    
    objdump_output = subprocess.check_output(['objdump', '-t', args.input_file]).decode('utf-8')
    objdump_lines = objdump_output.split('\n')

    symbol_set = set(args.symbols) if args.symbols else None
    
    iwram_symbols = []
    unmatched_symbols = set(args.symbols) if args.symbols else set()
    
    for line in objdump_lines:
        if '.iwram' in line:
            mangled_symbol = line.split()[-1]
            demangled_symbol = subprocess.check_output(['c++filt', mangled_symbol]).decode('utf-8').strip()
            
            if symbol_set is None or demangled_symbol in symbol_set:
                iwram_symbols.append(demangled_symbol)
                if demangled_symbol in unmatched_symbols:
                    unmatched_symbols.remove(demangled_symbol)

    if iwram_symbols:
        print("Matching symbols in the IWRAM section:")
        for symbol in iwram_symbols:
            print(symbol)
    else:
        print("No matching symbols found in the IWRAM section.")
    
    if unmatched_symbols:
        print("\nSymbols not found in the IWRAM section:")
        for symbol in unmatched_symbols:
            print(symbol)

if __name__ == '__main__':
    main()