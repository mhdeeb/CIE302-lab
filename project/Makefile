BINARY=bin
OBJDIR=obj
CODEDIRS=src
INCDIRS=

CC=gcc
OPT=-O0
DEPFLAGS=-MP -MD
CFLAGS=-Wall -Wextra -g $(foreach D,$(INCDIRS),-I$(D)) $(OPT) $(DEPFLAGS)

CFILES=$(foreach D,$(CODEDIRS),$(wildcard $(D)/*.c))
OBJECTS=$(patsubst %.c,$(OBJDIR)/%.o,$(CFILES))
DEPFILES=$(patsubst %.c,%.d,$(CFILES))

all: $(BINARY)

$(BINARY): $(OBJECTS)
	@$(CC) -o $@ $^
	$(info $@ created.)

%.o:%.c
	@$(CC) $(CFLAGS) -c -o $@ $<
	$(info $@ Created.)

clean:
	@rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)
	$(info Cleaned)

diff:
	@git status
	@git diff --stat

-include $(DEPFILES)

