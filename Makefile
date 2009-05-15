# MOLDUR: modular, env-free build system using GNU make
# by Samuel Hangouët

TARGET_FILES = $(wildcard */*.target)

all: $(TARGET_FILES)

.PHONY: $(TARGET_FILES)

$(TARGET_FILES):
	@echo Processing $@...
	@$(MAKE) --no-print-directory -f $@

XmlParser/test.target: XmlParser/lib.target
