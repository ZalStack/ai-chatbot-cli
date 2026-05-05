CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include -I./lib
LDFLAGS = -lcurl -lmysqlclient -lpthread

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
TEST_DIR = test

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET = $(BUILD_DIR)/chatbot

.PHONY: all clean test run docker-build docker-run docker-stop

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

test: $(TARGET)
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_chatbot.cpp -o $(BUILD_DIR)/test_chatbot $(LDFLAGS)
	./$(BUILD_DIR)/test_chatbot

clean:
	rm -rf $(BUILD_DIR)

docker-build:
	docker compose build

docker-run:
	docker compose down 2>/dev/null || true
	docker compose up -d mysql
	@echo "⏳ Waiting 30 seconds for MySQL to be ready..."
	@sleep 30
	docker compose run --rm chatbot

docker-stop:
	docker compose down

docker-logs:
	docker compose logs -f mysql

install-deps-fedora:
	sudo dnf install -y gcc-c++ make cmake libcurl-devel mariadb-devel git wget

install-deps-ubuntu:
	sudo apt update
	sudo apt install -y build-essential cmake libcurl4-openssl-dev libmysqlclient-dev git wget

run: all
	./$(TARGET)