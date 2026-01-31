#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m'

TESTS=(
    "test_tiny.c"
    "test_small.c"
    "test_free.c"
    "test_edge_cases.c"
    "test_failures.c"
)

echo -e "${BLUE}================================${NC}"
echo -e "${BLUE}        malloc Test Suite       ${NC}"
echo -e "${BLUE}================================${NC}\n"

passed=0
failed=0
total=${#TESTS[@]}

for test in "${TESTS[@]}"; do
    echo -e "${YELLOW}Running $test...${NC}"

    if make test TESTFILE="tests/$test" > /tmp/test_output.log 2>&1; then
        echo -e "${GREEN}✓ PASSED${NC}\n"
        ((passed++))
    else
        echo -e "${RED}✗ FAILED${NC}"
        echo "Last 10 lines of output:"
        tail -10 /tmp/test_output.log
        echo ""
        ((failed++))
    fi
done

echo -e "${BLUE}================================${NC}"
echo -e "${BLUE}              Summary           ${NC}"
echo -e "${BLUE}================================${NC}"
echo -e "Total:  $total tests"
echo -e "${GREEN}Passed: $passed${NC}"
if [ $failed -gt 0 ]; then
    echo -e "${RED}Failed: $failed${NC}"
else
    echo -e "${GREEN}Failed: $failed${NC}"
fi
echo ""

if [ $failed -eq 0 ]; then
    echo -e "${GREEN}All tests passed! 🎉${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed${NC}"
    exit 1
fi
