set -e

echo "========================================"
echo "   OS SCHEDULER: BUILDING ALGORITHMS"
echo "========================================"

echo "> Running make clean..."
make clean

echo "> Running make..."
make

echo ""
echo "========================================"
echo "   OS SCHEDULER: BUILDING BACKEND"
echo "========================================"

cd backend

echo "> Compiling c_server.c..."
gcc server.c mongoose.c -o c_backend

echo ""
echo "========================================"
echo "   OS SCHEDULER: STARTING BACKEND SERVER"
echo "========================================"

echo "> Server running at http://localhost:3001"
./c_backend