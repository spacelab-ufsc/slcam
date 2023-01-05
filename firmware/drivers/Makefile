ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

.PHONY: all
all:
#	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C ov2640 \
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C w25qxxx 
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C uart

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o
