for ((i=2; i<=16; i++)); do	
	./huffman compress test1.txt $i
	./huffman decompress test1.compressed
	
	cmp --silent test1.txt test1-uncompressed.txt || echo "Files are different!" $i "bits"
	echo "Checked test 1 with " $i " bits";
	
	rm test1-uncompressed.txt
	rm test1.compressed
done

for ((i=2; i<=16; i++)); do	
	./huffman compress test2.txt $i
	./huffman decompress test2.compressed
	
	cmp --silent test2.txt test2-uncompressed.txt || echo "Files are different!"
	echo "Checked test 2 with " $i " bits";
	
	rm test2-uncompressed.txt
	rm test2.compressed
done