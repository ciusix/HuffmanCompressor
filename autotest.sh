for ((testNumber=1; testNumber<=4; testNumber++)); do
	for ((i=2; i<=16; i++)); do	
		./huffman compress test$testNumber.txt $i
		./huffman decompress test$testNumber.compressed
	
		res=$(cmp --silent test$testNumber.txt test$testNumber-uncompressed.txt);
		if ($res); then
			echo "OK! Test " $testNumber " | letter size " $i "bits"
		else
			echo "FAILED! Files are different!" "Test " $testNumber " | letter size " $i "bits"
		fi
	
		rm test$testNumber-uncompressed.txt
		rm test$testNumber.compressed
	done
done