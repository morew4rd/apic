-default: show-help

show-help: ## show help
	@echo "Available targets:"
	@grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[36m%-30s\033[0m %s\n", $$1, $$2}'
	@echo "Defined variables:"
	@grep -E '^[a-zA-Z0-9_]+\s*=\s*[^#]*## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = "\s*=\s*.*?## "}; {printf "  \033[33m%-30s\033[0m %s\n", $$1, $$2}'

clean-test: ## clean test build
	rm -rf _b

config-test: ## configure tests
	mkdir -p _b

build-test: ## build tests
	cc -o _b/test src/a.c

run-test: clean-test config-test build-test ## rebuild and run test
	_b/test > _b/a.txt
	cat _b/a.txt