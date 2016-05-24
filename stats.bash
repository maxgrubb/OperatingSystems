#!/bin/bash

trap clean EXIT
clean () {

rm -f temp*
rm -f grubbfile$$*
rm -f medAvg*
rm -f sorted*
 
}

b=1  

grubbfile="grubbfile$$"

if [[ "$#" = "0" ]]
then
			>&2 echo 'stats {-rows | -cols } [file]'
			exit 1

elif [[ "$#" = "1" ]]
then

	cat > $grubbfile
	 

elif [[ "$#" = "2" ]]
		then
		    grubbfile=$2
		if [[ !(-r "$2") ]]
		then
		>&2 echo "cannot read $2"
		exit 1
		fi	


elif [[ "$#" = "3" ]]
then
		>&2 echo 'stats {-rows | -cols } [file]'
		exit 1
fi


grubbFunc (){
 
if [[ "$2" == "c" ]]
then
	CNT=$(head -n 1 $1 | awk '{print NF}')  #pipe the head line (-n 1) to awk to print (Number of Fields)
	for ((a=1;a<= $CNT;a++)){
	colLine=$(cut -f$a $1)
		for c in $colLine
		do	
			echo $c >> temp$a$PPID
		done  
	let b++
	}

elif [[ "$2" == "r" ]] 
then
	
while read line
do

 	
 for w in $line
        do
                 echo $w >> temp_$b$PPID	
	done 

	let b++

done <$1

fi
}

grubbStats (){


SUM=0
COUNT=$(cat $1 | wc -l) #pipe cat of file argument to wc with option -l for newline count 


sort -n $1 > sorted$PPID
while read statLine
do
	for t in $statLine
	do
		SUM=$(($SUM + $t))
	done


done  <$1



if [[ "$2" == -c* ]] 
then
	p=$(($COUNT / 2)) 
   	let p+=1	
	x=$SUM 
	y=$(sed -ne "$p"p < sorted*) #finds index as if file were an array
	let x*=10
	let x/=$COUNT
	let x+=5
	let x/=10
	echo $x >> tempAvg$BASHPID
	echo $y >> medAvg$BASHPID

elif [[ "$2" == -r* ]] 
then
	u=$(($COUNT / 2))
	let u+=1
	z=$SUM 
	v=$(sed -ne "$u"p < sorted*) #finds index as if file were an array    
	let z*=10
	let z/=$COUNT
	let z+=5
	let z/=10
	printf "%-8i %-8i\n" $z $v  
	
  
fi
}
#print function for column reads
grubbPrint (){
while read line
do
	for h in $line
	do
		printf "%-8i" $h
	done 

done <$1 
}
###MAIN#####


colCount=$(head -n 1 $grubbfile | awk '{print NF}')  #pipe the head line (-n 1) to awk to print (Number of Fields)
rowCount=$(cat $grubbfile | wc -l)   #pipe cat of file argument to wc with option -l for newline count 

	

	if [[ "$1" == -c* ]] 
	then
			if [[ "$colCount" == 0 ]]
			then	
			echo Averages:
			echo
			echo Medians: 
			echo
	
			else
	
			c=c
			grubbFunc $grubbfile $c
			for((i=1;i<= $colCount;i++)){
				grubbStats temp$i* $1
			}
			echo Averages:
			grubbPrint tempAvg*
			echo
			echo Medians: 
			grubbPrint medAvg* 
			echo
			fi
				
	elif [[ "$1" == -r* ]]
	then	
		if [[ "$rowCount" == 0 ]]
		then
				printf "%-8s %-8s\n" Average Median 
		else	
			r=r
					grubbFunc $grubbfile $r 
	
				printf "%-8s %-8s\n" Average Median 
					for((a=1;a<= $rowCount;a++)){
						grubbStats temp_$a* $1
				}
	
		fi	
	else					
		>&2 echo 'stats {-rows | -cols } [file]'
		exit 1
	fi							
			
exit 0
