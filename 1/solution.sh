rm -r 8091
mkdir 8091 && cd 8091
mkdir Shlyakhanov && cd Shlyakhanov
date > Daniil.txt
date --date "next Mon" > filedate.txt
cat Daniil.txt filedate.txt > result.txt
cat result.txt