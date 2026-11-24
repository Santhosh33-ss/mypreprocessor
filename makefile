compiler=cc
mypreprocessor:main.c comment_removal.c file_inclusion.c macro_handler.c 
	$(compiler) $^ -o $@
clean:
	@echo "cleaning up......."
	@rm -vf mypreprocessor *.o *.i
