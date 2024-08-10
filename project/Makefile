BINDIR=bin
BUILDDIR=build
SRCDIR=src
INCDIRS=

CC=gcc
OPT=-O0
DEPFLAGS=-MP -MD
CFLAGS=-Wall -Wextra -g$(foreach D,$(INCDIRS),-I$(D)) $(OPT) $(DEPFLAGS)

DEPFILES=$(wildcard $(BUILDDIR)/*.d)

default: all

all: makedir main disk kernal process copy_assets

main: main.o 
	@$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(BUILDDIR)/$^
	$(info [Target] $@ created.)

disk: disk.o msgq.o
	@$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(foreach D,$^,$(BUILDDIR)/$(D))
	$(info [Target] $@ created.)

kernal: kernal.o msgq.o
	@$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(foreach D,$^,$(BUILDDIR)/$(D))
	$(info [Target] $@ created.)

process: process.o msgq.o
	@$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(foreach D,$^,$(BUILDDIR)/$(D))
	$(info [Target] $@ created.)

%.o: $(SRCDIR)/%.c makedir
	@$(CC) $(CFLAGS) -c -o $(BUILDDIR)/$@ $<
	$(info [Object] $@ Created from source $<.)

clean:
	@rm -rf $(BINDIR) $(BUILDDIR)
	$(info [Clean] Done.)

makedir:
	@mkdir -p $(BINDIR) $(BUILDDIR)

diff:
	@git status
	@git diff --stat

copy_assets:
	@cp $(SRCDIR)/assets/* $(BINDIR)
	$(info [Assets] copied.)

-include $(DEPFILES)

.PHONY: default all clean makedir diff copy_assets
