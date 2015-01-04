for i in {1..6}
do 
	for j in {1..6}
	do 
		echo alpha=$i beta=$j
		./q_LDPC Work7-8db.opt 12.5:1.5:0.9:$i:$j -c P -r 1 -b 20000 -s 1419915465 -o $i-$j.txt
		echo -e "\n";
	done
done

