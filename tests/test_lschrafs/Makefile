COMPILER 		= 	c++
CFLAGS 			= 	-Wall -Wextra -Werror -std=c++98

SRCS_PATH 	= 	./src/
DIFF_PATH		=		./diffs/
STDOUTPUT_PATH		=		./output_std/
FTOUTPUT_PATH		=		./output_ft/


SRCS_NAMES	=		test_vector.cpp \
								test_map.cpp \
								test_stack.cpp \
								test_set.cpp \

SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAMES))

all: $(SRCS_NAMES)

$(SRCS_NAMES): FORCE
	@echo Testing $@: 

	@$(COMPILER) $(CFLAGS) $(addprefix $(SRCS_PATH), $@) -DTESTSTD=1 -o std
	@$(COMPILER) $(CFLAGS) $(addprefix $(SRCS_PATH), $@) -DTESTSTD=0 -o ft

	@mkdir -p $(DIFF_PATH)
	@mkdir -p $(STDOUTPUT_PATH)
	@mkdir -p $(FTOUTPUT_PATH)

	@echo -n STD execution time:
	@time -f "\t%E" ./std > $(addprefix $(STDOUTPUT_PATH), $@).txt

	@echo -n FT execution time:
	@time -f "\t%E" ./ft > $(addprefix $(FTOUTPUT_PATH), $@).txt

	@diff $(addprefix $(STDOUTPUT_PATH), $@).txt $(addprefix $(FTOUTPUT_PATH), $@).txt > $(addprefix $(DIFF_PATH), $@).diff

	@echo -n Diff length: 
	@cat $(addprefix $(DIFF_PATH), $@).diff | wc -l

	@echo 

	@rm -f std
	@rm -f ft

FORCE: ;

clean:
	rm -rf $(FTOUTPUT_PATH)
	rm -rf $(STDOUTPUT_PATH)
	rm -rf $(DIFF_PATH)

re: clean all

bonus: all