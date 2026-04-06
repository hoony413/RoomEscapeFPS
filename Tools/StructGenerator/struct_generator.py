import csv
import os
import re
import sys
from pathlib import Path

def capitalize_first_letter(s):
    """Capitalize the first letter of a string"""
    if not s:
        return s
    return s[0].upper() + s[1:]

def generate_structs_from_csv(csv_file_path):
    """Generate structs from a CSV file that may contain multiple sheets"""
    file_path = Path(csv_file_path)
    filename = file_path.stem
    
    # List to store generated code for all sheets
    all_generated_code = []
    sheet_names = []
    
    # Read the CSV file
    with open(csv_file_path, 'r', encoding='utf-8') as csvfile:
        content = csvfile.read()
        
        # Split content by sheet markers (empty rows can separate sheets)
        # We're looking for patterns like empty line followed by a line with content
        sheets_content = re.split(r'\n\s*\n(?=\S)', content)
        
        for i, sheet_content in enumerate(sheets_content):
            if not sheet_content.strip():
                continue
                
            # Determine sheet name - if it's the first sheet, use filename
            # otherwise use filename + sheet number
            if i == 0:
                sheet_name = filename
            else:
                sheet_name = f"{filename}_Sheet{i}"
                
            sheet_names.append(sheet_name)
            
            # Parse the sheet content
            csv_reader = csv.reader(sheet_content.splitlines())
            rows = list(csv_reader)
            
            if len(rows) < 2:  # Need at least header and type rows
                print(f"Warning: Sheet {sheet_name} in {csv_file_path} does not have enough data.")
                continue
                
            header_row = rows[0]
            type_row = rows[1]
            
            # Ensure we have data to work with
            if len(header_row) < 1 or len(type_row) < 1:
                print(f"Warning: Sheet {sheet_name} in {csv_file_path} does not have enough columns.")
                continue
            
            # Generate the No struct (custom struct for the first column)
            # Capitalize the first letter and add 'F' prefix
            capitalized_sheet_name = capitalize_first_letter(sheet_name)
            no_struct_name = f"F{capitalized_sheet_name}No"
            no_struct_code = generate_no_struct(no_struct_name)
            
            # Generate the main struct
            main_struct_code = generate_main_struct(capitalized_sheet_name, no_struct_name, header_row, type_row)
            
            # Combine code for this sheet
            sheet_code = f"{no_struct_code}\n\n{main_struct_code}"
            all_generated_code.append(sheet_code)
    
    # Combine all sheets' code
    if all_generated_code:
        return "\n\n".join(all_generated_code), sheet_names
    else:
        print(f"Error: No valid sheets found in {csv_file_path}")
        return None, []

def generate_no_struct(struct_name):
    """Generate the custom No struct with arithmetic operators"""
    return f"""USTRUCT(BlueprintType)
struct {struct_name} final
{{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    int32 _value{{}};

    // Constructors
    {struct_name}() = default;
    explicit {struct_name}(int32 InValue) : _value(InValue) {{}}

    // Arithmetic operators
    {struct_name} operator+(const {struct_name}& Other) const {{ return {struct_name}(_value + Other._value); }}
    {struct_name} operator-(const {struct_name}& Other) const {{ return {struct_name}(_value - Other._value); }}
    {struct_name} operator*(const {struct_name}& Other) const {{ return {struct_name}(_value * Other._value); }}
    {struct_name} operator/(const {struct_name}& Other) const {{ return {struct_name}(_value / Other._value); }}

    // Compound assignment operators
    {struct_name}& operator+=(const {struct_name}& Other) {{ _value += Other._value; return *this; }}
    {struct_name}& operator-=(const {struct_name}& Other) {{ _value -= Other._value; return *this; }}
    {struct_name}& operator*=(const {struct_name}& Other) {{ _value *= Other._value; return *this; }}
    {struct_name}& operator/=(const {struct_name}& Other) {{ _value /= Other._value; return *this; }}

    // Comparison operators
    bool operator==(const {struct_name}& Other) const {{ return _value == Other._value; }}
    bool operator!=(const {struct_name}& Other) const {{ return _value != Other._value; }}
    bool operator<(const {struct_name}& Other) const {{ return _value < Other._value; }}
    bool operator<=(const {struct_name}& Other) const {{ return _value <= Other._value; }}
    bool operator>(const {struct_name}& Other) const {{ return _value > Other._value; }}
    bool operator>=(const {struct_name}& Other) const {{ return _value >= Other._value; }}

    // Conversion to int32
    explicit operator int32() const {{ return _value; }}
}};

FORCEINLINE uint32 GetTypeHash(const {struct_name}& no)
{{
    return GetTypeHash(no._value);
}}"""

def generate_main_struct(sheet_name, no_struct_name, header_row, type_row):
    """Generate the main Unreal Engine struct"""
    # Start with the struct declaration
    struct_code = f"""USTRUCT(BlueprintType)
struct ROOMESCAPEFPS_API FTable{sheet_name}Row final : public FTableRowBase
{{
    GENERATED_BODY()

public:"""

    # Add the No struct as a member
    struct_code += f"""
    UPROPERTY(EditAnywhere)
    {no_struct_name} No;
"""

    # Process each column (starting from the second column)
    for i in range(2, len(header_row)):
        if i < len(type_row):
            column_name = header_row[i]
            data_type = type_row[i].strip()
            
            # Skip empty columns and columns starting with # (comment columns)
            if not column_name or not data_type or column_name.startswith('#'):
                continue
            
            # Format variable name (first character lowercase, prepend underscore)
            var_name = column_name
            
            # Check if the type is an enum
            if data_type.startswith("enum."):
                enum_name = data_type.split(".")[1]
                type_definition = f"E{enum_name}"
                bracket_init = "{}" 
            elif data_type.startswith("array."):
                array_type_name = data_type.split(".")[1]
                type_definition = f"TArray<{array_type_name}>"
                bracket_init = ""
            else:
                type_definition = data_type
                # Add {} for primitive types
                bracket_init = "{}" if is_primitive_type(data_type) else ""
            
            # Add member to struct
            struct_code += f"""
    UPROPERTY(EditAnywhere)
    {type_definition} {var_name}{bracket_init};
"""
    
    # Close the struct
    struct_code += "};";
    
    return struct_code

def is_primitive_type(type_name):
    """Check if the type is a primitive Unreal Engine type"""
    primitive_types = [
        "int8", "int16", "int32", "int64", 
        "int8_t", "int16_t", "int32_t", "int64_t",
        "uint8", "uint16", "uint32", "uint64", 
        "uint8_t", "uint16_t", "uint32_t", "uint64_t", 
        "float", "double", "bool"
    ]
    return type_name in primitive_types

def save_to_header_file(header_content, output_path):
    """Save the generated struct code to a header file"""
    with open(output_path, 'w', encoding='utf-8') as header_file:
        # Add standard header includes
        header_content = f"""// Generated C++ header file
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "g_enum.h"
#include "g_struct.generated.h"

{header_content}
"""
        header_file.write(header_content)
    
    print(f"Successfully generated header file: {output_path}")

def process_directory(directory_path):
    """Process all CSV files in the specified directory"""
    # List to store all generated code
    all_generated_code = []
    
    # Get list of all CSV files in the directory
    csv_files = [f for f in os.listdir(directory_path) if f.endswith('.csv')]
    
    if not csv_files:
        print(f"No CSV files found in the directory: {directory_path}")
        return
    
    # Process each CSV file and collect the generated code
    for file in csv_files:
        csv_path = os.path.join(directory_path, file)
        
        print(f"Processing {file}...")
        generated_code, _ = generate_structs_from_csv(csv_path)
        
        if generated_code:
            all_generated_code.append(generated_code)
    
    # Combine all the generated code into a single header file
    if all_generated_code:
        combined_code = "\n\n".join(all_generated_code)
        # Always save to StructGenerator folder regardless of where CSVs are
        script_dir = os.path.dirname(os.path.abspath(__file__))
        output_path = "D:/RoomEscapeFPS/Source/RoomEscapeFPS/Public/Globals/g_struct.h"
        save_to_header_file(combined_code, output_path)
    else:
        print("No structs were generated. Check your CSV files.")

def main():
    # Check if a directory was specified as a command-line argument
    if len(sys.argv) > 1:
        # Use the directory specified as argument
        target_dir = sys.argv[1]
        if os.path.isdir(target_dir):
            process_directory(target_dir)
        else:
            print(f"Error: The specified path is not a directory: {target_dir}")

if __name__ == "__main__":
    main() 