
g++ -Wall -std=c++14 -fno-pie -I../src -c claphost.cpp -o clap-host.o

g++ clap-host.o -o clap-host -no-pie -ldl -lsndfile

# -lrt -lpthread -lxcb -lxcb-util -lxcb-image -lxcb-cursor -lxcb-keysyms -lcairo -lsndfile

rm ./clap-host.o

