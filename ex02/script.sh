#!/bin/bash

EXEC="./PmergeMe"

if [ ! -f "$EXEC" ]; then
    echo "Error: Executable $EXEC not found."
    exit 1
fi

# Target ranges based on Ford-Johnson theoretical limits
sizes=(2 3 4 5 10 21 100 1000 3000)

# The absolute maximum comparisons allowed for each size
declare -A max_cmp=(
    [2]=1
    [3]=3
    [4]=5
    [5]=7
    [10]=22
    [21]=66
    [100]=534
    [1000]=8977
    [3000]=31521
)

echo -e "\n🚀 Starting Ford-Johnson Dual-Container Benchmark...\n"
printf "%-9s | %-11s | %-9s | %-9s | %-11s | %-11s\n" "Elements" "Max Allowed" "Vec Cmp" "Deq Cmp" "Vec Status" "Deq Status"
printf "%.0s-" {1..73}
echo ""

for size in "${sizes[@]}"; do
    # Generate 'size' amount of unique random numbers between 1 and 1,000,000
    args=$(shuf -i 1-1000000 -n "$size" | tr '\n' ' ')
    
    # Run the program and capture the output
    output=$($EXEC $args 2>/dev/null)
    
    # Extract the comparison counts for vector and deque separately
    vec_cmp=$(echo "$output" | grep -i "number of comparaisons vector" | awk '{print $NF}')
    deq_cmp=$(echo "$output" | grep -i "number of comparaisons deque" | awk '{print $NF}')
    
    # Check Vector status
    if [ -z "$vec_cmp" ]; then
        vec_cmp="ERR"
        vec_status="❌"
    else
        if [ "$vec_cmp" -le "${max_cmp[$size]}" ]; then
            vec_status="✅ PASS"
        else
            vec_status="⚠️ HIGH"
        fi
    fi

    # Check Deque status
    if [ -z "$deq_cmp" ]; then
        deq_cmp="ERR"
        deq_status="❌"
    else
        if [ "$deq_cmp" -le "${max_cmp[$size]}" ]; then
            deq_status="✅ PASS"
        else
            deq_status="⚠️ HIGH"
        fi
    fi
    
    # Print the row
    printf "%-9s | %-11s | %-9s | %-9s | %-11s | %-11s\n" "$size" "${max_cmp[$size]}" "$vec_cmp" "$deq_cmp" "$vec_status" "$deq_status"
done
echo ""