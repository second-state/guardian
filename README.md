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
$ lity --state-variables <example.so> -o <example_output_path>
~~~
Where `<example.so>` is the lity source code which contain the contract, and `<example_output_path>` is the folder you want to put the exported json files.

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

`base_contract` and `new_contract` must formate in `<example.so>:<contract_name>`.

where `<contract_name>` is the contract to be compared and `<example.so>` is the lity source code which contain this contract.

## 3. Example running
Suppose now that there are two lity source codes, `A.so` and `B.so`, where `A.so` is base and `B.so` is new. The contracts you want to compare are `contract_A`(located in `A.so`) and `contract_B`(located in `B.so`).

1. Export their state-variables to json file:
~~~
$ lity --state-variables A.so -o folder_A
$ lity --state-variables B.so -o folder_B
~~~

2. Run our program:
~~~
$ ./stateVariablesCompareTool --base_jsonPath folder_A --base_contract A.so:contract_A --new_jsonPath folder_B --new_contract B.so:contract_B
~~~

## 4. Output
If there is no state-variables conflict between new contract and base contract, it will show following message in standard output:
~~~
Accept! New contract has no state-variable conflict with base contract.
~~~
Otherwise, it will output a conflicting part, including its type, name, number of lines, etc.

You can see the output of our testcases for example.

## 5. Testcase
After build the executable file, you can run each testcase by
~~~
./testdata/<testcaseFolder>/rest.sh
~~~
Each testcase has there folder named `<testcaseFolder>` in `/testdata`.