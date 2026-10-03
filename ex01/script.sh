#!/bin/bash

# Define the executable and the output file
EXEC="./RPN"
OUTPUT_FILE="output.txt"

# Clear the output file if it already exists
> "$OUTPUT_FILE"

# Helper function to run a test and append results to the file
run_test() {
    description=$1
    expression=$2
    
    echo "Test: $description" >> "$OUTPUT_FILE"
    echo "Input: \"$expression\"" >> "$OUTPUT_FILE"
    echo -n "Output: " >> "$OUTPUT_FILE"
    
    # Run the program, redirecting both stdout and stderr (2>&1) to the file
    $EXEC "$expression" >> "$OUTPUT_FILE" 2>&1
    
    echo "--------------------------------------------------" >> "$OUTPUT_FILE"
}

echo "Running RPN Tests..."
echo "Writing results to $OUTPUT_FILE..."

# 1. Valid Cases
run_test "Valid: Complex Subject Example" "8 9 * 9 - 9 - 9 - 4 - 1 +"
run_test "Valid: Simple Calculation" "4 2 * 3 -"

# 2. Stack Underflow
run_test "Error: Stack Underflow (Operator only)" "+"
run_test "Error: Stack Underflow (One number)" "8 +"
run_test "Error: Stack Underflow (Missing operand for second operator)" "1 2 + *"

# 3. Leftover Operands
run_test "Error: Leftover Operands (Too many numbers)" "1 2 3 +"
run_test "Error: Leftover Operands (No operators)" "8 9"
run_test "Error: Leftover Operands (Empty string)" ""

# 4. Division by Zero
run_test "Error: Division by Zero (Direct)" "8 0 /"
run_test "Error: Division by Zero (Calculated)" "8 2 2 - /"

# 5. Lexical Constraints
run_test "Error: Lexical (Alphabetical character)" "1 2 a +"
run_test "Error: Lexical (Multi-digit number)" "10 2 +"
run_test "Error: Lexical (Invalid symbol)" "1 2 + !"

echo "Tests complete! Check $OUTPUT_FILE for the results."