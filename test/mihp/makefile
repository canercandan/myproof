GCC=gcc-4.5
PLUGIN_NAME=mihp
PLUGIN_SOURCE_FILES=$(PLUGIN_NAME).c mylist_all.c mylist_all_data.c mylist_all_struct.c mylist_count.c mylist_delete.c mylist_free.c mylist_init.c mylist_pop.c mylist_push.c
PLUGIN_OBJECT_FILES=$(patsubst %.c,%.o,$(PLUGIN_SOURCE_FILES))
GCCPLUGINS_DIR:=$(shell $(GCC) -print-file-name=plugin\)
CFLAGS+=-I$(GCCPLUGINS_DIR)/include -fPIC -O2 -I.
LDFLAGS+=

$(PLUGIN_NAME).so: $(PLUGIN_OBJECT_FILES)
	$(GCC) -shared $^ -o $@

$(PLUGIN_OBJECT_FILES):%.o:%.c
	$(GCC) $(CFLAGS) $^ -c -o $@

clean:
	rm -f *o *~

fclean: clean
	rm -f *.so

re:	fclean $(PLUGIN_NAME).so
