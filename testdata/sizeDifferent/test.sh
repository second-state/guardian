#! /bin/sh
echo "run test without any tag:"
../../build/src/stateVariablesCompareTool --base_jsonPath base --base_contract base.sol:baseContract --new_jsonPath new --new_contract new.sol:newContract

echo "====================================================="

echo "run test with --typeOnly"
../../build/src/stateVariablesCompareTool --base_jsonPath base --base_contract base.sol:baseContract --new_jsonPath new --new_contract new.sol:newContract --typeOnly
