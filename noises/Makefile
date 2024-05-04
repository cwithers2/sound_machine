OUTPUT=mp3/
build:
	mkdir -pv $(OUTPUT)
	cd $(OUTPUT); ../generate.py
	cd $(OUTPUT); ../convert.sh
clean:
	rm -R $(OUTPUT)
