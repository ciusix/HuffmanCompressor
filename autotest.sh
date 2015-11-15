for ((i=2; i<=16; i++)); do
	rm test1-uncompressed.txt
	rm test1.compressed
	
	./huffman compress test1.txt $i
	./huffman decompress test1.compressed
	
	cmp --silent test1.txt test1-uncompressed.txt || echo "Files are different!"
	echo "Checked test 1 with " $i " bits";
done

for ((i=2; i<=16; i++)); do
	rm test3-uncompressed.txt
	rm test3.compressed
	
	./huffman compress test3.txt $i
	./huffman decompress test3.compressed
	
	cmp --silent test3.txt test3-uncompressed.txt || echo "Files are different!"
	echo "Checked test 3 with " $i " bits";
done