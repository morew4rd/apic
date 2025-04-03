-default: run-test

##########
show-help: ## show help
	@echo "Available targets:"
	@grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[36m%-30s\033[0m %s\n", $$1, $$2}'
	@echo "Defined variables:"
	@grep -E '^[a-zA-Z0-9_]+\s*=\s*[^#]*## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = "\s*=\s*.*?## "}; {printf "  \033[33m%-30s\033[0m %s\n", $$1, $$2}'
##########


BUILD_DIR = _b

clean-dir: ## clean test build
	rm -rf ${BUILD_DIR}
	mkdir -p ${BUILD_DIR}

build-basic: ## build tests
	cc -g -o ${BUILD_DIR}/basic_reflect test/basic/basic_reflect.c
	cc -g -o ${BUILD_DIR}/basic_header  test/basic/basic_header.c

run-test: clean-dir build-basic 	## rebuild and run tests
	${BUILD_DIR}/basic_header
	${BUILD_DIR}/basic_reflect > test/output/basic_reflect_output.c
	cat test/output/basic_reflect_output.c