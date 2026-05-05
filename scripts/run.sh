#!/bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${CYAN}========================================"
echo " AI Chatbot CLI - Quick Start"
echo -e "========================================${NC}"
echo ""

# Check if .env exists
if [ ! -f .env ]; then
    echo -e "${RED}❌ .env file not found!${NC}"
    exit 1
fi

# Load environment variables
echo -e "${YELLOW}📁 Loading .env configuration...${NC}"
set -a
source .env
set +a
echo -e "${GREEN}✅ Environment variables loaded${NC}"

# Check MySQL connection
echo -e "${YELLOW}🔍 Checking MySQL connection...${NC}"
if mysql -h "${DB_HOST:-127.0.0.1}" -u "${DB_USER:-root}" -p"${DB_PASSWORD}" -e "SELECT 1;" &>/dev/null; then
    echo -e "${GREEN}✅ MySQL connected${NC}"
else
    echo -e "${RED}❌ Cannot connect to MySQL${NC}"
    echo "Please check your MySQL credentials in .env"
    echo ""
    echo "Options:"
    echo "1. Start MySQL: sudo systemctl start mysqld"
    echo "2. Use Docker: ./run.sh docker"
    exit 1
fi

# Check if build exists
if [ ! -f build/chatbot ]; then
    echo -e "${YELLOW}🔨 Building project...${NC}"
    make clean && make
    if [ $? -ne 0 ]; then
        echo -e "${RED}❌ Build failed!${NC}"
        exit 1
    fi
    echo -e "${GREEN}✅ Build successful${NC}"
fi

# Run chatbot
echo ""
echo -e "${CYAN}🚀 Starting AI Chatbot CLI...${NC}"
echo ""
./build/chatbot