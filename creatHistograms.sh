#runs shipcbm.C for different files

#!/bin/bash
# ./createHistograms.sh

anafolder="$SHIPSOFT/Analysis/"
anafile="shipcbm.C"

for i in $@
do 
	path=${i%/ship.*}  # get the part before the colon
	filename=${i##*/ship.}  # get the part after the last slash
	filename=${filename%.root}
done

let len=${#filename[@]}

echo "----------------------"
echo "PATH: ${path}"
echo "FILE(S): ${filename[@]}"
if [ ${len} -ne 1 ]; then
	echo "OUTFILE: ${outfile}"
fi
echo "----------------------"
echo "Correct?"

read confirm
confirm="y"
# ----------------------------------------------------------------------
if [ "$confirm" == "y" ]; then
	echo "creating Histograms for file(s): ${filename[@]}"
	if [ ${len} -ne 1 ]; then
	fi
	#Creates histograms
	for i in "${filename[@]}"
	do
		FILENAME=""${anafolder}${anafile}"(\"${i}\",\"${path}\")"
		~/ShipSoft/FairSoftInst/bin/root -q -b -l ${FILENAME}
	done
else
	echo "change setup or confirm!" 
fi
# ----------------------------------------------------------------------
