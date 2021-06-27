#!/bin/bash

cd ./"$2";

{
	find .git/objects/pack/ -name "*.idx" |
	while read i; do
		git show-index < "$i" | awk '{print $2}';
	done;

	find .git/objects/ -type f | grep -v '/pack/' |
	awk -F'/' '{print $(NF-1)$NF}';
} | while read o; do
	echo "$o" >> "$3";
	if git cat-file -p $o | grep --color=always -awi "$1" | grep -a -qPx '[[:ascii:]]*'
	then
		printf "commit hash: %s\n\n" "$o";
		printf "[*] Commit info: \n";
		git show "$o" | grep -Px '[[:ascii:]]*';
		printf "\n[*] Now printing the contents: \n";
		git cat-file -p $o | grep --color=always -awi "$1" | grep -a -Px '[[:ascii:]]*';
		printf "%238s\n" " " | tr ' ' '=';
	fi
done
