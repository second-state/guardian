# state-variables-compare
Compare state-variables json which generate by lity

## 1. How to Build
**Step 1:** Download the source code. For example,
~~~
$ git clone --recurse-submodules https://github.com/second-state/guardian.git
~~~

**Step 2:** Go to the project root and build by
~~~
$ cd guardian
$ mkdir build
$ cd build
$ cmake ..
$ make
~~~

The executable file will be placed in `./build/src` and named `stateVariablesCompareTool`.

### 1.1. Dependencies

* [Git](https://git-scm.com/) (version >= 2.13)
* [GCC](https://gcc.gnu.org/) (must can compile in c++14) or other working c++ compliers
* [CMake](https://cmake.org/) (version >= 3.8)

## 2. How to Run

### 2.1 Lity
The contract to be compared must export its state-variables to json file through [lity](https://github.com/second-state/lity) by
~~~
$ lity --state-variables <example.sol> -o <example_output_path>
~~~
Where `<example.sol>` is the lity source code which contain the contract, and `<example_output_path>` is the folder you want to put the exported json files.

### 2.2 command line interface
~~~
$ ./stateVariablesCompareTool --help
This tool can check two state-varables json output from lity have conflict or not.
Usage: ./stateVariablesCompareTool [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  --base_jsonPath TEXT REQUIRED
                              Path of base state-variables json folder.
  --base_contract TEXT REQUIRED
                              The base contract you want to compare.
  --new_jsonPath TEXT REQUIRED
                              Path of new state-variables json folder.
  --new_contract TEXT REQUIRED
                              The new contract you want to compare.
  --typeOnly                  Add this flag will only compare the variable storage size.
  --detail                    Add this flag will print detail message when compare two contract.
~~~

`base_contract` and `new_contract` must formate in `<example.sol>:<contract_name>`.

where `<contract_name>` is the contract to be compared and `<example.sol>` is the lity source code which contain this contract.

## 3. Example running
Suppose now that there are two lity source codes, `A.sol` and `B.sol`, where `A.sol` is base and `B.sol` is new. The contracts you want to compare are `contract_A`(located in `A.sol`) and `contract_B`(located in `B.sol`).

1. Export their state-variables to json file:
~~~
$ lity --state-variables A.sol -o folder_A
$ lity --state-variables B.sol -o folder_B
~~~

2. Run our program:
~~~
$ ./stateVariablesCompareTool --base_jsonPath folder_A --base_contract A.sol:contract_A --new_jsonPath folder_B --new_contract B.sol:contract_B
~~~

## 4. Output
### Accept
If there is no state-variables conflict between new contract and base contract, it will show following message in standard output:
~~~
Accept! New contract has no state-variable conflict with base contract.
~~~

### Conflict
Otherwise, it will output a conflicting part, including its type, name, number of lines, etc.

For example, there are two contract:
1. A.sol
```
contract contract_A{
	int a;
	string b;
	int32 c;
	function set()public{
	}
	contract_A d;
	enum L{a,b,c}
	L e;
}
```
2. B.sol
```
contract contract_B{
	string a;
	int b;
	int64 c;
	function set()public{
	}
	contract_B d;
	enum L{a,b,c,d,e}
	L e;
}
```

The output of our program will be:
```
Type different:
>>>base contract:
   Type of stateVariable is "int256"
   Source: "int a"
   at file: A.sol, Line: 2, Col: 2
===
   Type of stateVariable is "string"
   Source: "string a"
   at file: B.sol, Line: 2, Col: 2
<<<new contract
Different at 0-th stateVariable.

Type different:
>>>base contract:
   Type of stateVariable is "string"
   Source: "string b"
   at file: A.sol, Line: 3, Col: 2
===
   Type of stateVariable is "int256"
   Source: "int b"
   at file: B.sol, Line: 3, Col: 2
<<<new contract
Different at 1-th stateVariable.

Type different:
>>>base contract:
   Type of stateVariable is "int32"
   Source: "int32 c"
   at file: A.sol, Line: 4, Col: 2
===
   Type of stateVariable is "int64"
   Source: "int64 c"
   at file: B.sol, Line: 4, Col: 2
<<<new contract
Different at 2-th stateVariable.

Type different:
>>>base contract:
   Type of stateVariable is "enum_3"
   Source: "L e"
   at file: A.sol, Line: 9, Col: 2
===
   Type of stateVariable is "enum_5"
   Source: "L e"
   at file: B.sol, Line: 9, Col: 2
<<<new contract
Different at 4-th stateVariable.

Sorry, there are some conflict between base contract and new contract.
```

You can see the other output of our testcases for example.

## 5. Testcase
After build the executable file, you can run each testcase by
~~~
./testdata/<testcaseFolder>/rest.sh
~~~
Each testcase has there folder named `<testcaseFolder>` in `/testdata`.