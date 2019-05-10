 
export LD_LIBRARY_PATH=../build
g++ -o test1 test1.cpp ../build/liblscl_static.a -I .. -L ../build
