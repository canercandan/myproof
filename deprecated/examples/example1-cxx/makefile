GCC=g++-4.5
PLUGIN_NAME=list
PLUGIN_SOURCE_FILES=$(PLUGIN_NAME).cpp
PLUGIN_OBJECT_FILES=$(patsubst %.cpp,%.o,$(PLUGIN_SOURCE_FILES))
GCCPLUGINS_DIR:=$(shell $(GCC) -print-file-name=plugin\)
CFLAGS+=-I$(GCCPLUGINS_DIR)/include -fPIC -O2

$(PLUGIN_NAME).so: $(PLUGIN_OBJECT_FILES)
	$(GCC) -shared $^ -o $@

all: $(PLUGIN_NAME).so

$(PLUGIN_OBJECT_FILES):%.o:%.cpp
	$(GCC) $(CFLAGS) $^ -c -o $@

clean:
	rm -f *o *~

re: clean all