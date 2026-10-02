CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic

TARGETS := epoll TCP_Client
LOG_DIR ?= logs
CLIENT_COUNT ?= 3
TEST_SECONDS ?= 5

.PHONY: all test clean

all: $(TARGETS)

epoll: epoll.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

TCP_Client: TCP_Client.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

test: all
	rm -rf $(LOG_DIR)
	mkdir -p $(LOG_DIR)
	./epoll > $(LOG_DIR)/server.log 2>&1 & \
	server_pid=$$!; \
	trap 'kill -INT $$server_pid 2>/dev/null || true' EXIT INT TERM; \
	sleep 1; \
	client_pids=""; \
	for client_id in $$(seq 1 $(CLIENT_COUNT)); do \
		timeout $(TEST_SECONDS)s ./TCP_Client > $(LOG_DIR)/client_$$client_id.log 2>&1 & \
		client_pids="$$client_pids $$!"; \
	done; \
	for client_pid in $$client_pids; do \
		wait $$client_pid || true; \
	done; \
	kill -INT $$server_pid 2>/dev/null || true; \
	wait $$server_pid 2>/dev/null || true; \
	trap - EXIT INT TERM

clean:
	rm -f $(TARGETS)
