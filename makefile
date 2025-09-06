BUILD_DIR = build
EXEC_RELEASE = $(BUILD_DIR)/release/bin/platform
EXEC_DEBUG = $(BUILD_DIR)/debug/bin/platform

SRCS := $(shell find src -name "*.cpp")
HEADERS := $(shell find include -name "*.hpp")

buildr:
	@cmake -S . -B $(BUILD_DIR)/release -DCMAKE_BUILD_TYPE=Release
	@cmake --build $(BUILD_DIR)/release

runr:
	@$(EXEC_RELEASE)

buildd:
	@cmake -S . -B $(BUILD_DIR)/debug -DCMAKE_BUILD_TYPE=Debug
	@cmake --build $(BUILD_DIR)/debug

rund:
	@$(EXEC_DEBUG)

fmt:
	clang-format -i $(SRCS) $(HEADERS)
	@echo "Formatted .cpp and .hpp files"