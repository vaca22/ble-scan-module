
NAME 	= collections
SRCS	:= \
		src/array.c\
		src/common.c\
		src/deque.c\
		src/hashset.c\
		src/hashtable.c\
		src/pqueue.c\
		src/queue.c\
		src/ring_buffer.c\
		src/slist.c\
		src/stack.c\
		src/treeset.c\
		src/treetable.c
		#src/list.c\

PRIV_INC = -I./src -I./src/include

OBJS_DIR = $(OBJ_PATH)/$(NAME)
OBJS_DIR_SUB = $(OBJS_DIR)/src
THISLIB  = $(LIB_PATH)/lib$(NAME).a

OBJS = $(patsubst %.c, $(OBJS_DIR)/%.o, $(SRCS))
DEPS = $(patsubst %.c, $(OBJS_DIR)/%.d, $(SRCS))

$(THISLIB):$(OBJS_DIR) $(OBJS)
	$(ECHO)echo ar $(THISLIB)
	$(ECHO)$(AR) r $(THISLIB) $(OBJS)

$(OBJS_DIR)/%.o:%.c
	$(ECHO)echo cc $<
	$(ECHO)$(CC) $(CFLAGS) $(PRIV_INC) -c $< -o $@

$(OBJS_DIR):
	$(mk_objs_dir)

define mk_objs_dir
@if test ! -d $(OBJS_DIR);\
	then \
	mkdir $(OBJS_DIR) $(OBJS_DIR_SUB);\
fi
endef

$(DEPS):$(OBJS_DIR)/%.d:%.c
	$(mk_objs_dir)
	@set -e; \
	$(CC) -MM $(PRIV_INC) $< > $@; \
	sed -i 's,.*\.o:, $@:,g'  $@;\
	sed -i 's,\.d:,.o $@ :,g' $@;

-include $(DEPS)

