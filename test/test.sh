 
export LD_LIBRARY_PATH=../build
g++ -o test1 test1.cpp -I .. -L ../build -l lscl_shared
