ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

.PHONY: all
all:
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C ov2640
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C w25qxxx

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o
