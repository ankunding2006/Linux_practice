CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic -pthread

SERVER := muti_server
CLIENT := tcp_client
SERVER_SOURCE := muti_Client.cpp
CLIENT_SOURCE := ../Week3_Day3/TCP_Client.cpp
CLIENT_COUNT ?= 3
TEST_SECONDS ?= 5

.PHONY: all server client run-server run-client clients test clean

all: server client

server: $(SERVER)

client: $(CLIENT)

$(SERVER): $(SERVER_SOURCE)
	$(CXX) $(CXXFLAGS) $< -o $@

$(CLIENT): $(CLIENT_SOURCE)
	$(CXX) $(CXXFLAGS) $< -o $@

run-server: server
	./$(SERVER)

run-client: client
	./$(CLIENT)

clients: client
	@set -eu; \
	pids=""; \
	trap '[ -z "$$pids" ] || kill -INT $$pids 2>/dev/null || true' INT TERM; \
	for index in $$(seq 1 $(CLIENT_COUNT)); do \
		./$(CLIENT) > client-$$index.log 2>&1 & \
		pids="$$pids $$!"; \
	done; \
	echo "started $(CLIENT_COUNT) clients:$$pids"; \
	wait

test: all
	@set -eu; \
	server_pid=""; \
	pids=""; \
	cleanup() { \
		trap - INT TERM EXIT; \
		[ -z "$$pids" ] || kill -INT $$pids 2>/dev/null || true; \
		[ -z "$$server_pid" ] || kill -INT $$server_pid 2>/dev/null || true; \
		[ -z "$$pids" ] || wait $$pids 2>/dev/null || true; \
		[ -z "$$server_pid" ] || wait $$server_pid 2>/dev/null || true; \
	}; \
	trap cleanup INT TERM EXIT; \
	./$(SERVER) > server.log 2>&1 & server_pid=$$!; \
	sleep 1; \
	for index in $$(seq 1 $(CLIENT_COUNT)); do \
		./$(CLIENT) > client-$$index.log 2>&1 & \
		pids="$$pids $$!"; \
	done; \
	echo "server=$$server_pid clients=$(CLIENT_COUNT) duration=$(TEST_SECONDS)s"; \
	sleep $(TEST_SECONDS)

clean:
	rm -f $(SERVER) $(CLIENT) server.log client-*.log