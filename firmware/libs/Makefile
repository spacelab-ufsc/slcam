ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

.PHONY: all
all:
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C containers

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o
