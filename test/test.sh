
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/../build

g++ -o test1 test1.cpp ../build/liblscl_shared.so -I .. -L ../build -l pthread

./test1

