#!/bin/bash
cd ./"$2";

while IFS= read -r o || [[ -n "$o" ]]; do
	if git cat-file -p $o | grep --color=always -awi "$1" | grep -a -qPx '[[:ascii:]]*'
	then
		printf "commit hash: %s\n\n" "$o";
		printf "[*] Commit info: \n"
		git show "$o" | grep -Px '[[:ascii:]]*';
		printf "\n[*] Now printing the contents: \n";
		git cat-file -p $o | grep --color=always -awi "$1" | grep -a -Px '[[:ascii:]]*';
		printf "%238s\n" " " | tr ' ' '=';
	fi
done < "$3"

