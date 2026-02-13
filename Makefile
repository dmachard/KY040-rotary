.PHONY: lint
lint:
	@echo "Running Arduino Lint..."
	arduino-lint --verbose ../$(notdir $(CURDIR))

.PHONY: help
help:
	@echo "Available targets:"
	@echo "  make lint     - Run Arduino Lint on the library"
	@echo "  make help     - Display this help message"