sorter:
	g++ *.cpp *.h -o sorter
	./sorter me.png
	open output.png

clean:
	rm sorter