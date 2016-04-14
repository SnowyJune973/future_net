mkdir target
cd target
mkdir code
cd ..

cd future_net
g++ future_net.cpp libCbc.a libCbcSolver.a libCgl.a libClp.a libClpSolver.a libCoinUtils.a libOsi.a libOsiCbc.a libOsiClp.a libOsiCommonTests.a -l dl -O3 -o future_net

cd ..
cp -r future_net/* target/code

cd future_net
cp future_net ../target

cd ..
cp batch55.sh readme.txt target
cd target
cd code
rm future_net
cd ..
tar -zcPf future_net.tar.gz *
cp future_net.tar.gz ..
cd ..
rm -rf target

