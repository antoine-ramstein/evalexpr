#!/bin/sh

BIN="./evalexpr"

pass=0
fail=0

run_test() {
    input="$1"
    args="$2"
    expected="$3"
    expected_code="$4"
    result="$(echo "$input" | $BIN $args 2>/dev/null)"
}

run_test "1 1 +" "-rpn" "2" 0
run_test "2 3 +" "-rpn" "5" 0
run_test "4 5 *" "-rpn" "20" 0
run_test "6 2 /" "-rpn" "3" 0
run_test "7 3 %" "-rpn" "1" 0
run_test "2 3 ^" "-rpn" "8" 0
run_test "5 2 2 ^ *" "-rpn" "20" 0
run_test "5 2 3 + *" "-rpn" "25" 0
run_test "10 2 / 3 +" "-rpn" "8" 0
run_test "10 2 3 + /" "-rpn" "2" 0
run_test "1 0 /" "-rpn" "" 3
run_test "1 0 %" "-rpn" "" 3
run_test "" "-rpn" "" 0
run_test "1 a +" "-rpn" "" 1
run_test "1 +" "-rpn" "" 2
run_test "5 2 -" "-rpn" "3" 0
run_test "8 4 2 / +" "-rpn" "10" 0
run_test "2 3 4 * +" "-rpn" "14" 0
run_test "3 3 ^ 2 +" "-rpn" "29" 0
run_test "7 2 3 ^ *" "-rpn" "56" 0

run_test "1+1" "" "2" 0
run_test "2+3" "" "5" 0
run_test "4*5" "" "20" 0
run_test "6/2" "" "3" 0
run_test "7%3" "" "1" 0
run_test "2^3" "" "8" 0
run_test "5*(2^2)" "" "20" 0
run_test "5*(2+3)" "" "25" 0
run_test "10/2+3" "" "8" 0
run_test "10/(2+3)" "" "2" 0
run_test "1/0" "" "" 3
run_test "1%0" "" "" 3
run_test "" "" "" 0
run_test "a+1" "" "" 1
run_test "1(+1)" "" "" 2
run_test "5-2" "" "3" 0
run_test "8+4/2" "" "10" 0
run_test "2+3*4" "" "14" 0
run_test "3^3+2" "" "29" 0
run_test "7*2^3" "" "56" 0

exit 0
