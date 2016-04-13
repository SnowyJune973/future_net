mkdir target
cd target
mkdir code
cd ..

cd future_net
g++ -O3 future_net.cpp liblpsolve55.a -l dl -o future_net

cd ..
cp -r future_net/* target/code

cd future_net
cp future_net ../target

cd ..
cp batch55.sh readme.txt target
cd target
tar -zcPf future_net.tar.gz *
cp future_net.tar.gz ..
cd ..
rm -rf target

