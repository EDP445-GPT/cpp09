#!/bin/bash

# Make sure this matches the name of your compiled executable
EXEC="./PmergeMe"

# Colors for terminal output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

if [ ! -f "$EXEC" ]; then
    echo -e "${RED}Error: Executable $EXEC not found. Compile your project first!${NC}"
    exit 1
fi

# Function to run a specific test
run_test() {
    local test_name="$1"
    shift
    local args="$*"
    
    echo -e "${BLUE}==================================================${NC}"
    echo -e "${GREEN}📝 TEST: ${test_name}${NC}"
    
    # If the input is too long, truncate it for the display printout
    if [ ${#args} -gt 100 ]; then
        echo -e "👉 Input: ${args:0:100}... (truncated)"
    else
        echo -e "👉 Input: $args"
    fi
    
    echo -e "💻 Output:"
    $EXEC $args
    echo ""
}

# 1. Standard Subject Example
run_test "Standard subject example" 3 5 9 7 4

# 2. Odd number of elements (leaves a leftover naturally)
run_test "Odd number of elements" 23 4 99 12 7 45 18 2 88 31 10 56 3 77 15

# 3. Even number of elements
run_test "Even number of elements" 10 2 8 4 6 1 9 3 14 12

# 4. Already sorted (Best case scenario)
run_test "Already sorted" 1 2 3 4 5 6 7 8 9 10 11 12

# 5. Reverse sorted (Worst case scenario)
run_test "Reverse sorted" 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1

# 6. Duplicates (Tests if your binary search handles == correctly)
run_test "With duplicates" 4 2 8 2 4 9 1 9 1 4

# 7. Error handling - Negative numbers
run_test "Error test - Negative number" 5 8 2 -4 9

# 8. Error handling - Invalid characters
run_test "Error test - Invalid character" 5 8 a 9 2

# 9. Stress test - 100 Random numbers
echo -e "${BLUE}==================================================${NC}"
echo -e "${GREEN}📝 TEST: 100 Random Numbers (Stress Test)${NC}"
# Generate 100 random numbers between 1 and 1000
RANDOM_INPUT=""
for i in {1..100}; do
    RANDOM_INPUT="$RANDOM_INPUT $((RANDOM % 1000 + 1)) "
done

# We run it directly instead of through the function to avoid expanding $RANDOM_INPUT improperly
echo -e "👉 Input: 100 randomly generated integers"
echo -e "💻 Output:"
$EXEC $RANDOM_INPUT
echo ""