#!/bin/bash

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${CYAN}========================================"
echo " AI Chatbot CLI - Docker Mode"
echo -e "========================================${NC}"
echo ""

echo -e "${YELLOW}🔍 Checking port 3306...${NC}"
if lsof -Pi :3306 -sTCP:LISTEN -t >/dev/null 2>&1; then
    echo -e "${YELLOW}⚠️  Port 3306 is in use. Stopping local MySQL...${NC}"
    sudo systemctl stop mysqld 2>/dev/null || sudo systemctl stop mysql 2>/dev/null || true
    sleep 2
fi

echo -e "${YELLOW}🐳 Starting Docker containers...${NC}"
docker compose down 2>/dev/null
docker compose up -d mysql

echo -e "${YELLOW}⏳ Waiting for MySQL to be ready...${NC}"
sleep 15

if docker compose ps mysql | grep -q "healthy"; then
    echo -e "${GREEN}✅ MySQL is ready${NC}"
else
    echo -e "${YELLOW}⚠️  MySQL might still be starting...${NC}"
    sleep 10
fi

echo -e "${CYAN}🚀 Starting AI Chatbot CLI...${NC}"
echo ""
docker compose run --rm chatbot

echo ""
echo -e "${YELLOW}🛑 Stopping Docker containers...${NC}"
docker compose down