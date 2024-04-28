NAME = basics.a
OBJS_FILES = basics_type.o basics_split.o \
				basics_print.o \
				basics_memory.o basics_free.o \
				basics_error.o\

HEADER_FILES = basics.h
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

show:
	echo $(OBJS_FILES)

$(NAME): $(OBJS_FILES)
	@ar -rcs $@ $^

bonus: $(OBJS_FILES) 
	@ar -rcs $(NAME) $(OBJS_FILES) 

%.o: %.c $(HEADER_FILES)
	@$(CC) -c $(CFLAGS) -o $@ $< -fPIC

so : $(OBJS_FILES)
	$(CC) $(OBJS_FILES) -shared -o basics.so

clean:
	@$(RM) $(OBJS_FILES) 

fclean: clean
	@$(RM) $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re