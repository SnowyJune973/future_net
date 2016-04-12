mkdir target
cd target
mkdir code
cd ..

sh batch.sh

cd future_net
mv future_net future_netw2

g++ -O3 w1/future_net.cpp liblpsolve55.a -l dl -o future_net
cd ..
cp -r future_net/* target/code

cd future_net
cp future_net ../target
cp future_netw2 ../target
cd ..
cp batch55.sh readme.txt target
cd target
tar -zcPf future_net.tar.gz *
cp future_net.tar.gz ..
cd ..
rm -rf target

