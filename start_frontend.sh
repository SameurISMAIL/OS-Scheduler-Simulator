set -e

echo "========================================"
echo "   OS SCHEDULER: STARTING FRONTEND"
echo "========================================"

cd frontend

if [ ! -d "node_modules" ]; then
    echo "> Dependencies not found. Running 'npm install'..."
    npm install
else
    echo "> Dependencies already installed. Skipping 'npm install'."
fi

echo ""
echo "========================================"
echo "   LAUNCHING REACT APP"
echo "========================================"

npm start