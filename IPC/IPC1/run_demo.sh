#!/bin/bash

echo "========================================="
echo "Shared Memory IPC Demo - Run Script"
echo "========================================="

# Check if binaries exist
if [ ! -f "P1/build/writer" ]; then
    echo "ERROR: P1/build/writer not found. Run ./build_and_run.sh first"
    exit 1
fi

if [ ! -f "P2/build/reader" ]; then
    echo "ERROR: P2/build/reader not found. Run ./build_and_run.sh first"
    exit 1
fi

# Clean up any existing shared memory
echo "Cleaning up any existing shared memory..."
ipcs -m | grep $(whoami) | awk '{print $2}' | xargs -I {} ipcrm -m {} 2>/dev/null

echo ""
echo "Starting Writer (P1) in background..."
./P1/build/writer &
WRITER_PID=$!

echo "Waiting 2 seconds for writer to initialize..."
sleep 2

echo ""
echo "Starting Reader (P2)..."
./P2/build/reader

echo ""
echo "Waiting for writer to finish..."
wait $WRITER_PID

echo ""
echo "========================================="
echo "Demo completed successfully!"
echo "========================================="
