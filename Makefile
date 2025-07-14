sorter:
	g++ *.cpp *.h -o sorter
	./sorter big-me.png
	open output.png

clean:
	rm sorter