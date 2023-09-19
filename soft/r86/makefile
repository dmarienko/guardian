CC = cl

target = radio86

build = rkbuild
dump = rkdump

all: tools files generate

tools: builder dumper

builder:
	$(CC) $(build).c

dumper:
	$(CC) $(dump).c

files: make-list 
	$(MAKE) merge-files

make-list:
	cmd /c dir /b \
		files\*.gam files\*.rk files\*.rkr files\*.pki files\*.bin \
		>files.lst		

merge-files:
	cd files
	..\$(dump).exe <..\files.lst >..\files.js

generate:
	$(build).exe <$(target)-template.html >$(target).html

dist-sources:
	7z a -r radio86js.zip makefile *.js *.html *.c files/*.*

clean:
	-del $(target).html
	-del files.js
	-del files.lst
	-del $(build).exe $(build).obj
	-del $(dump).exe $(dump).obj
