#/bin/bash
for i in $(ls data/); do echo $i; head -1 data/$i; done
