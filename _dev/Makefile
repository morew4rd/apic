################################################################################################################################################################
--default: default-task
--targets:
	@echo "Available targets:"
	@grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[36m%-30s\033[0m %s\n", $$1, $$2}'
################################################################################################################################################################

default-task: run-all


BUILD_DIR = build


show-help: --targets ## List available targets

cmake-config: ## cmake config (cleans previous build & test output)
	rm -rf $(BUILD_DIR)
	cmake -S . -B $(BUILD_DIR)

cmake-build: ## cmake build
	cmake --build $(BUILD_DIR)

run-all: cmake-config cmake-build	## rebuild and run tests
	mkdir -pv $(BUILD_DIR)/test_out
	$(BUILD_DIR)/basic_reflect > $(BUILD_DIR)/test_out/basic_reflect.c
	cat $(BUILD_DIR)/test_out/basic_reflect.c
	$(BUILD_DIR)/basic_header


# clean-test-output: ## cleans test output
# 	rm -rf test/output/**

# clean-all: clean-test-output ## clean test build
# 	rm -rf $(BUILD_DIR)
# 	mkdir -p $(BUILD_DIR)

# build-basic: ## build tests
# 	cc -MJ ./build/compile_commands.json -g -o $(BUILD_DIR)/basic_reflect -I src test/basic/basic_reflect.c
# 	cc -MJ ./build/compile_commands.json -g -o $(BUILD_DIR)/basic_header  -I src test/basic/basic_header.c
# 	cc -MJ ./build/compile_commands.json -g -o $(BUILD_DIR)/kyte_reflect -I src test/kyte/kyte_reflect.c
# run-kyte:  ## kyte
# 	$(BUILD_DIR)/kyte_reflect > test/output/kyte_reflect_output.c

