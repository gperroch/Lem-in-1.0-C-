#!/bin/bash

NB_ANT=0
NB_ROOM=0
NB_LINK=0
NB_PHASE=0
START_INDEX=-1
END_INDEX=-1
OFFSET_PHASE=0

function reading
{
	local LINE PREV PHASE
	LINE="-"
	PREV="-"
	PHASE=1

	while [ "$LINE" != "" ] || [ "$PREV" != "" ]
	do
		PREV="$LINE"
		read LINE
		if [ "$PREV" = "" ] && [ "$LINE" != "" ]
		then
			PHASE=2
		fi
		if [ "$LINE" != "" ] && [ $PHASE = 1 ]
		then
			record_structure $LINE
		elif [ "$LINE" != "" ] && [ $PHASE = 2 ]
		then
			record_movements "$LINE"
		fi
	done
}

function record_structure
{
	local NAME X Y
	NAME=$1
	X=$2
	Y=$3
	if [ "`echo "$NAME" | cut -c -1`" = "#" ]
	then
		if [ "$NAME" = "##start" ]
		then
			START_INDEX=$NB_ROOM
		elif [ "$NAME" = "##end" ]
		then
			END_INDEX=$NB_ROOM
		fi
	elif [ "$X" != "" ] && [ "$Y" != "" ]
	then
		ROOM_NAMES[$NB_ROOM]=$NAME
		ROOM_COOR_X[$NB_ROOM]=$X
		ROOM_COOR_Y[$NB_ROOM]=$Y
		NB_ROOM=$(( $NB_ROOM + 1 ))
	elif [ "`echo "$NAME" | grep -`" != "" ]
	then
		LINK[$NB_LINK]=$NAME
		NB_LINK=$(( $NB_LINK + 1 ))
	else
		NB_ANT=$NAME
	fi
}

function record_movements
{
	MOVE[$NB_PHASE]="`echo $1 | sed 's/[ ]L/ /g' | sed 's/^L/ /g'`"
	NB_PHASE=$(( $NB_PHASE + 1 ))
}

function display
{
	print_room
	tput cup $(( ${COOR_PLACES_Y[$START_INDEX]} + 1 )) $(( ${COOR_PLACES_X[$START_INDEX]} - 2 ))
	echo "START"
	tput cup $(( ${COOR_PLACES_Y[$END_INDEX]} + 1 )) $(( ${COOR_PLACES_X[$END_INDEX]} - 1 ))
	echo "END"
	print_link
}

function print_room
{
	local TMP OFFSET_LINE OFFSET_COL
	TMP=0
	OFFSET_LINE=0
	OFFSET_COL=0
	tput cup 0 0
	tput cd

	while [ $TMP -lt $NB_ROOM ]
	do
		tput cup $OFFSET_COL $OFFSET_LINE
		echo " ___________ "
		tput cup $(( $OFFSET_COL + 1 )) $OFFSET_LINE
		echo "|     ${ROOM_NAMES[$TMP]}     |"
		tput cup $(( $OFFSET_COL + 2 )) $OFFSET_LINE
		echo "|           |"
		tput cup $(( $OFFSET_COL + 3 )) $OFFSET_LINE
		echo "|___________|"
		COOR_PLACES_X[$TMP]=$(( $OFFSET_LINE + 6))
		COOR_PLACES_Y[$TMP]=$(( $OFFSET_COL + 2))
		TMP=$(( $TMP + 1 ))
		OFFSET_LINE=$(( $OFFSET_LINE + 16 ))
		if [ $OFFSET_LINE = 64 ]
		then
			OFFSET_LINE=0
			OFFSET_COL=$(( $OFFSET_COL + 12 ))
		fi
	done
}

function print_link
{
	local I J NIVELEMENT Z K
	Z=0
	TMP_UP=$NB_LINK
	if [ "$1" != "" ]
	then
		Z=$1
		TMP_UP=$(( $Z + 1 ))
	fi
	if [ "$2" != "" ]
	then
		printf "$2"
	fi
	while [ $Z -lt $TMP_UP ]
	do
	I=0
	J=0
	TMP=0
	NIVELEMENT=0
	START_NIVELAGE=0
	CURRENT_LINK="`echo ${LINK[$Z]} | cut -d ' ' -f 1`"
	ROOM1="`echo $CURRENT_LINK | cut -d '-' -f 1`"
	ROOM2="`echo $CURRENT_LINK | cut -d '-' -f 2`"

	## Selection des index des pieces à lier.
	
	while [ "${ROOM_NAMES[$I]}" != "$ROOM1" ]
	do
		I=$(( $I + 1 ))
	done
	while [ "${ROOM_NAMES[$J]}" != "$ROOM2" ]
	do
		J=$(( $J + 1 ))
	done

	## Echange des index temporaires pour avoir I a gauche de J 

	if [ ${COOR_PLACES_X[$I]} -gt ${COOR_PLACES_X[$J]} ]
	then
		TMP=$J
		J=$I
		I=$TMP
	fi
	Y_LEFT=$(( ${COOR_PLACES_Y[$I]} + 2 ))
	X_LEFT=${COOR_PLACES_X[$I]}
	Y_RIGHT=$(( ${COOR_PLACES_Y[$J]} + 2 ))
	X_RIGHT=${COOR_PLACES_X[$J]}

	## Calcul des décallages de colonnes et lignes.

	NIVELEMENT=$(( $Y_RIGHT - $Y_LEFT ))
	if [ $(( $X_RIGHT - $X_LEFT )) = 16 ] || [ $(( $X_RIGHT - $X_LEFT )) = 48 ]
	then
		START_NIVELAGE=$(( ($X_RIGHT - $X_LEFT) / 2))
	elif [ $(( $X_RIGHT - $X_LEFT )) = 32 ]
	then
		START_NIVELAGE=$(( ($X_RIGHT - $X_LEFT) / 4))
	fi

	## Ecriture du lien.

	K=0

	if [ $NIVELEMENT -lt 0 ] && [ "$2" != "\033[31m" ]
	then
		printf "\033[33m"
	elif [ $NIVELEMENT -gt 0 ] && [ "$2" != "\033[31m" ]
	then
		printf "\033[36m"
	fi
	if [ $X_RIGHT = 22 ] && [ $X_LEFT = 6 ]
	then
		if [ $NIVELEMENT -lt 0 ]
		then
			printf "\033[1m"
			X_RIGHT=$(( $X_RIGHT - 5 ))
			X_LEFT=$(( $X_LEFT - 5 ))
		elif [ $NIVELEMENT -gt 0 ]
		then
			printf "\033[1m"
			X_RIGHT=$(( $X_RIGHT - 4 ))
			X_LEFT=$(( $X_LEFT - 4 ))
		fi
		while [ $K -lt 1 ]
		do
			tput cup $Y_RIGHT $X_RIGHT
			printf "|"
			tput cup $Y_LEFT $X_LEFT
			printf "|"
			K=$(( $K + 1 ))
			if [ $K -lt 1 ]
			then
				Y_RIGHT=$(( $Y_RIGHT + 1 ))
				Y_LEFT=$(( $Y_LEFT + 1 ))
			fi
		done
	elif [ $X_RIGHT = 38 ] && [ $X_LEFT = 22 ]
	then
		if [ $NIVELEMENT -lt 0 ]
		then
			X_RIGHT=$(( $X_RIGHT - 3 ))
			X_LEFT=$(( $X_LEFT - 3 ))
		elif [ $NIVELEMENT -gt 0 ]
		then
			X_RIGHT=$(( $X_RIGHT - 2 ))
			X_LEFT=$(( $X_LEFT - 2 ))
		fi
		while [ $K -lt 2 ]
		do
			tput cup $Y_RIGHT $X_RIGHT
			printf "|"
			tput cup $Y_LEFT $X_LEFT
			printf "|"
			K=$(( $K + 1 ))
			if [ $K -lt 2 ]
			then
				Y_RIGHT=$(( $Y_RIGHT + 1 ))
				Y_LEFT=$(( $Y_LEFT + 1 ))
			fi
		done
	elif [ $X_RIGHT = 54 ] && [ $X_LEFT = 38 ]
	then
		if [ $NIVELEMENT -lt 0 ]
		then
			X_RIGHT=$(( $X_RIGHT - 1 ))
			X_LEFT=$(( $X_LEFT - 1 ))
		elif [ $NIVELEMENT -gt 0 ]
		then
			X_RIGHT=$(( $X_RIGHT + 1 ))
			X_LEFT=$(( $X_LEFT + 1 ))
		fi
		while [ $K -lt 3 ]
		do
			tput cup $Y_RIGHT $X_RIGHT
			printf "|"
			tput cup $Y_LEFT $X_LEFT
			printf "|"
			K=$(( $K + 1 ))
			if [ $K -lt 3 ]
			then
				Y_RIGHT=$(( $Y_RIGHT + 1 ))
				Y_LEFT=$(( $Y_LEFT + 1 ))
			fi
		done
	elif [ $X_RIGHT = 38 ] && [ $X_LEFT = 6 ]
	then
		if [ $NIVELEMENT -lt 0 ]
		then
			printf "\033[1m"
			X_RIGHT=$(( $X_RIGHT + 2 ))
			X_LEFT=$(( $X_LEFT + 2 ))
		elif [ $NIVELEMENT -gt 0 ]
		then
			printf "\033[1m"
			X_RIGHT=$(( $X_RIGHT + 3 ))
			X_LEFT=$(( $X_LEFT + 3 ))
		else
			X_RIGHT=$(( $X_RIGHT + 4 ))
			X_LEFT=$(( $X_LEFT + 4 ))
		fi
		while [ $K -lt 4 ]
		do
			tput cup $Y_RIGHT $X_RIGHT
			printf "|"
			tput cup $Y_LEFT $X_LEFT
			printf "|"
			K=$(( $K + 1 ))
			if [ $K -lt 4 ]
			then
				Y_RIGHT=$(( $Y_RIGHT + 1 ))
				Y_LEFT=$(( $Y_LEFT + 1 ))
			fi
		done
	elif [ $X_RIGHT = 54 ] && [ $X_LEFT = 22 ]
	then
		if [ $NIVELEMENT -lt 0 ]
		then
			printf "\033[1m"
			X_RIGHT=$(( $X_RIGHT + 5 ))
			X_LEFT=$(( $X_LEFT + 5 ))
		elif [ $NIVELEMENT -gt 0 ]
		then
			printf "\033[1m"
			X_RIGHT=$(( $X_RIGHT - 5 ))
			X_LEFT=$(( $X_LEFT - 5 ))
		else
			X_RIGHT=$(( $X_RIGHT - 4 ))
			X_LEFT=$(( $X_LEFT - 4 ))
		fi
		while [ $K -lt 5 ]
		do
			tput cup $Y_RIGHT $X_RIGHT
			printf "|"
			tput cup $Y_LEFT $X_LEFT
			printf "|"
			K=$(( $K + 1 ))
			if [ $K -lt 5 ]
			then
				Y_RIGHT=$(( $Y_RIGHT + 1 ))
				Y_LEFT=$(( $Y_LEFT + 1 ))
			fi
		done
	elif [ $X_RIGHT = 54 ] && [ $X_LEFT = 6 ]
	then
		if [ $NIVELEMENT -lt 0 ]
		then
			X_RIGHT=$(( $X_RIGHT - 3 ))
			X_LEFT=$(( $X_LEFT - 3 ))
		elif [ $NIVELEMENT -gt 0 ]
		then
			X_RIGHT=$(( $X_RIGHT - 2 ))
			X_LEFT=$(( $X_LEFT - 2 ))
		else
			X_RIGHT=$(( $X_RIGHT - 1 ))
			X_LEFT=$(( $X_LEFT - 1 ))
		fi
		while [ $K -lt 6 ]
		do
			tput cup $Y_RIGHT $X_RIGHT
			printf "|"
			tput cup $Y_LEFT $X_LEFT
			printf "|"
			K=$(( $K + 1 ))
			if [ $K -lt 6 ]
			then
				Y_RIGHT=$(( $Y_RIGHT + 1 ))
				Y_LEFT=$(( $Y_LEFT + 1 ))
			fi
		done
	else
		printf "\033[35m"
		if [ "$2" = "\033[31m" ]
		then
			printf "\033[31m"
		fi
		if [ $NIVELEMENT != 0 ]
		then
			X_RIGHT=$(( $X_RIGHT + 5 ))
			X_LEFT=$(( $X_LEFT + 5 ))
		elif [ $NIVELEMENT -gt 0 ]
		then
			X_RIGHT=$(( $X_RIGHT + 5 ))
			X_LEFT=$(( $X_LEFT + 5 ))
		fi
		if [ $Y_LEFT -lt $Y_RIGHT ]
		then
			tput cup $Y_RIGHT $X_RIGHT
			printf "|_"
			tput cup $Y_LEFT $X_LEFT
			printf "|_"
			X_LEFT=$(( $X_LEFT + 2 ))
			Y_LEFT=$(( $Y_LEFT + 1 ))
		else
			tput cup $Y_RIGHT $X_RIGHT
			printf "|___"
			tput cup $Y_LEFT $X_LEFT
			printf "|___"
			X_LEFT=$(( $X_LEFT + 4 ))
			Y_LEFT=$(( $Y_LEFT + 1 ))
		fi
		if [ $Y_LEFT -lt $Y_RIGHT ]
		then
			tput cup $Y_LEFT $X_LEFT
			printf "|"
		fi
		while [ $Y_LEFT != $Y_RIGHT ]
		do
			if [ $Y_LEFT -lt $Y_RIGHT ]
			then
				Y_LEFT=$(( $Y_LEFT + 1 ))
				tput cup $Y_LEFT $X_LEFT
				printf "|"
			elif [ $Y_LEFT -gt $Y_RIGHT ]
			then
				tput cup $Y_LEFT $X_LEFT
				printf "|"
				Y_LEFT=$(( $Y_LEFT - 1 ))
			fi
		done
	fi

	X_LEFT=$(( $X_LEFT + 1 ))
	while [ $X_LEFT -lt $X_RIGHT ]
	do
		if [ $X_LEFT = $(( ${COOR_PLACES_X[$I]} + $START_NIVELAGE )) ] && [ $NIVELEMENT != 0 ]
		then
			while [ $NIVELEMENT != 0 ]
			do
				if [ $NIVELEMENT -gt 0 ]
				then
					tput cup $(( $Y_LEFT + 1 )) $X_LEFT
					if [ $Y_LEFT != $(( ${COOR_PLACES_Y[$I]} + 2 )) ]
					then
						printf "|"
					fi
					Y_LEFT=$(( $Y_LEFT + 1 ))
					NIVELEMENT=$(( $NIVELEMENT - 1 ))
					if [ $NIVELEMENT = 0 ]
					then
						tput cup $Y_LEFT $X_LEFT
						printf "|"
						X_LEFT=$(( $X_LEFT + 1 ))
					fi
				elif [ $NIVELEMENT -lt 0 ]
				then
					tput cup $Y_LEFT $X_LEFT
					printf "|"
					Y_LEFT=$(( $Y_LEFT - 1 ))
					NIVELEMENT=$(( $NIVELEMENT + 1 ))
					if [ $NIVELEMENT = 0 ]
					then
						tput cup $Y_LEFT $X_LEFT
					fi
				fi
			done
		else
			printf "_"
			X_LEFT=$(( $X_LEFT + 1 ))
		fi
	done
	Z=$(( $Z + 1 ))
	printf "\033[0m"
	done
	printf "\033[0m"
	tput cup $(( ($NB_ROOM / 4) * 8 )) 0
}

function highlight_link
{
	local A B PREVIOUS_ROOM
	A=0
	B=0
	PREVIOUS_ROOM=""
	while [ "`echo ${MOVE[$A]} | grep "$1-$2"`" = "" ]
	do
		A=$(( $A + 1 ))
	done
	if [ $A -gt 0 ]
	then

		## PROBLEME avec tout le script quand les noms des salles contiennent des nombres
		## PROBLEME avec des noms des salle trop longs.	

		PREVIOUS_ROOM=`echo ${MOVE[$(( $A - 1 ))]} | grep $1 | tr ' ' '\n' | grep $1 | cut -d '-' -f 2`
		if [ "$PREVIOUS_ROOM" = "" ]
		then
			PREVIOUS_ROOM=${ROOM_NAMES[$START_INDEX]}
		fi
	else
		PREVIOUS_ROOM=${ROOM_NAMES[$START_INDEX]}
	fi
	while [ "$2-$PREVIOUS_ROOM" != "${LINK[$B]}" ] && [ "$PREVIOUS_ROOM-$2" != "${LINK[$B]}" ]
	do
		B=$(( $B + 1 ))
	done
	print_link $B "\033[31m"
	sleep 2
	print_link $B "\033[0m"
}

function display_phase
{
	local FOURMI PIECE MOUVEMENT NB_MOUVEMENT A B
	A=0
	NB_MOUVEMENT=`echo ${MOVE[$OFFSET_PHASE]} | tr ' ' '\n' | wc -l | sed 's/ //g'`
	while [ $A -lt $NB_MOUVEMENT ]
	do
		TAB_MOVE[$A]=`echo ${MOVE[$OFFSET_PHASE]} | tr ' ' '\n' | sed -n $(( $A + 1 ))p`
		A=$(( $A + 1 ))
	done
	A=0
	while [ $A -lt $NB_MOUVEMENT ]
	do
		# Affichage de la nouvelle position
		B=0
		FOURMI=`echo ${TAB_MOVE[$A]} | cut -d '-' -f 1`
		PIECE=`echo ${TAB_MOVE[$A]} | cut -d '-' -f 2`
		while [ "$PIECE" != "${ROOM_NAMES[$B]}" ]
		do
			B=$(( $B + 1 ))
		done
		tput cup $(( ${COOR_PLACES_Y[$B]} )) $(( ${COOR_PLACES_X[$B]} ))
		echo -e "\033[32;1m$FOURMI\033[0m"
		A=$(( $A + 1 ))
		highlight_link $FOURMI $PIECE
		if [ $B = $END_INDEX ]
		then
			say -v Daniel "Ant"
			sleep 0.05
			say -v Daniel "$FOURMI"
			sleep 0.05
			say -v Daniel "arrived."
		fi
	done


	if [ $OFFSET_PHASE -gt 0 ]
	then
		A=0
		NB_MOUVEMENT=`echo ${MOVE[$(( $OFFSET_PHASE - 1 ))]} | tr ' ' '\n' | wc -l | sed 's/ //g'`
		while [ $A -lt $NB_MOUVEMENT ]
		do
			TAB_MOVE_2[$A]=`echo ${MOVE[$(( $OFFSET_PHASE - 1 ))]} | tr ' ' '\n' | sed -n $(( $A + 1 ))p`
			A=$(( $A + 1 ))
		done
		A=0
		while [ $A -lt $NB_MOUVEMENT ]
		do
			# Effacement de la précédente position
			B=0
			FOURMI=`echo ${TAB_MOVE_2[$A]} | cut -d '-' -f 1`
			PIECE=`echo ${TAB_MOVE_2[$A]} | cut -d '-' -f 2`
			while [ "$PIECE" != "${ROOM_NAMES[$B]}" ]
			do
				B=$(( $B + 1 ))
			done
			if [ "`echo "${MOVE[$OFFSET_PHASE]}" | grep ${ROOM_NAMES[$B]}`" = "" ]
			then
				tput cup $(( ${COOR_PLACES_Y[$B]} )) $(( ${COOR_PLACES_X[$B]} ))
				printf "  "
			fi
			A=$(( $A + 1 ))
		done
	fi
}

function place_ants
{
	read COMMAND
	while [ $OFFSET_PHASE -lt $NB_PHASE ]
	do
		sleep 3
		tput cup $(( $(( `echo $(( $NB_ROOM / 4 )) | cut -f1 -d\.` + 1 )) * 12 + $OFFSET_PHASE )) 0
		printf "${MOVE[$OFFSET_PHASE]}"
		sleep 0
		display_phase
		OFFSET_PHASE=$(( $OFFSET_PHASE + 1 ))
#		sleep 2
	done
	tput cup $(( $(( `echo $(( $NB_ROOM / 4 )) | cut -f1 -d\.` + 1 )) * 12 + $NB_PHASE )) 0
#	tput cup $(( ($NB_ROOM / 4) * 8 + $NB_PHASE )) 0
}

function main
{
	reading
	display		# Faire les liens verticaux
	place_ants
}

tput civis
main
tput cnorm
