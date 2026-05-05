#!/bin/bash

echo "========================================"
echo " AI Chatbot CLI - Linux Setup"
echo "========================================"
echo ""

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Detect OS
if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$NAME
else
    OS="Unknown"
fi

echo -e "${YELLOW}Detected OS: $OS${NC}"
echo ""

# Function to install packages
install_packages() {
    if command -v dnf &> /dev/null; then
        echo "Using DNF package manager (Fedora/RHEL)..."
        sudo dnf install -y gcc-c++ make cmake libcurl-devel mariadb-devel nlohmann-json-devel git wget
    elif command -v apt &> /dev/null; then
        echo "Using APT package manager (Ubuntu/Debian)..."
        sudo apt update
        sudo apt install -y build-essential cmake libcurl4-openssl-dev libmysqlclient-dev nlohmann-json3-dev git wget
    elif command -v pacman &> /dev/null; then
        echo "Using Pacman package manager (Arch)..."
        sudo pacman -S --noconfirm base-devel cmake curl mariadb-libs nlohmann-json git wget
    elif command -v zypper &> /dev/null; then
        echo "Using Zypper package manager (openSUSE)..."
        sudo zypper install -y gcc-c++ cmake libcurl-devel libmysqlclient-devel nlohmann-json-devel git wget
    else
        echo -e "${RED}Unsupported package manager!${NC}"
        exit 1
    fi
}

# Install dependencies
echo "Installing dependencies..."
install_packages

# Download nlohmann/json if not present
if [ ! -f "lib/json.hpp" ]; then
    echo "Downloading nlohmann/json.hpp..."
    mkdir -p lib
    wget -O lib/json.hpp https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
fi

# Create build directory
echo "Creating build directory..."
mkdir -p build
cd build

# Run CMake
echo "Running CMake..."
cmake ..

# Build
echo "Building project..."
make -j$(nproc)

echo ""
echo -e "${GREEN}========================================"
echo " Setup Complete!"
echo "========================================"
echo ""
echo "To run the chatbot:"
echo "  cd build"
echo "  ./chatbot"
echo ""
echo "Or use Docker:"
echo "  docker-compose up -d mysql"
echo "  docker-compose run --rm chatbot"
echo -e "========================================${NC}"