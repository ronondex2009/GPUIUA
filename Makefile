CC := gcc
Deps := util.h util.c parser.h parser.c nodetree.h nodetree.c
Flags := 

Build.exe: $(Deps)
	$(CC) -o Build.exe $(Deps) $(Flags)
