-default: show-help

BUILD_DIR = _b

show-help: ## show help
	@echo "Available targets:"
	@grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[36m%-30s\033[0m %s\n", $$1, $$2}'
	@echo "Defined variables:"
	@grep -E '^[a-zA-Z0-9_]+\s*=\s*[^#]*## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = "\s*=\s*.*?## "}; {printf "  \033[33m%-30s\033[0m %s\n", $$1, $$2}'

clean-test: ## clean test build
	rm -rf ${BUILD_DIR}

config-test: ## configure tests
	mkdir -p ${BUILD_DIR}

build-test: ## build tests
	cc -o ${BUILD_DIR}/test -I src src/apic.c test/test.c

run-test: clean-test config-test build-test ## rebuild and run test
	${BUILD_DIR}/test > ${BUILD_DIR}/a.txt
	cat ${BUILD_DIR}/a.txt