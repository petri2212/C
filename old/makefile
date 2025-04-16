TARGET = ese03

$(TARGET): ese03_main.o ese03_library.a
	gcc $^ -o $@
	
ese03_main.o: ese03_main.c
	gcc -c $< -o $@
     
ese03_library.a: ese03_library.o
	ar rcs $@ $^

ese03_library.o: ese03_library.c ese03_library.h
	gcc -c -o $@ $<

clean:
	rm -f *.o *.a $(TARGET)


