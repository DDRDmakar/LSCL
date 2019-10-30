
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/../build

g++ -o test_multithread test_multithread.cpp -l pthread ../build/liblscl_shared.so -I .. -L ../build && ./test_multithread

