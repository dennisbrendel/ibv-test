CFLAGS := -Wall -g -O0
LDFLAGS := -libverbs

EXEC := ibv_reg_mem_reg

all: $(EXEC)

ibv_reg_mem_reg: reg_mem_reg.c
	cc $(CFLAGS) $^ $(LDFLAGS) -o $@

.PHONY: all clean

clean:
	$(RM) $(EXEC)
