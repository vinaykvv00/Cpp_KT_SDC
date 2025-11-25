#!/bin/bash

echo "========================================="
echo "Shared Memory IPC Demo - Build Script"
echo "========================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Build P1 (Writer)
echo -e "\n${YELLOW}[1/4] Building P1 (Writer)...${NC}"
cd P1
rm -rf build
mkdir build
cd build
cmake .. && make
if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed for P1${NC}"
    exit 1
fi
echo -e "${GREEN}P1 built successfully!${NC}"

# Build P2 (Reader)
echo -e "\n${YELLOW}[2/4] Building P2 (Reader)...${NC}"
cd ../../P2
rm -rf build
mkdir build
cd build
cmake .. && make
if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed for P2${NC}"
    exit 1
fi
echo -e "${GREEN}P2 built successfully!${NC}"

cd ../../

echo -e "\n${GREEN}=========================================${NC}"
echo -e "${GREEN}Build completed successfully!${NC}"
echo -e "${GREEN}=========================================${NC}"

echo -e "\n${YELLOW}To run the demo:${NC}"
echo -e "  Terminal 1: ${GREEN}./P1/build/writer${NC}"
echo -e "  Terminal 2: ${GREEN}./P2/build/reader${NC}"
echo ""
echo -e "${YELLOW}Or run both automatically:${NC}"
echo -e "  ${GREEN}./run_demo.sh${NC}"
