#!/bin/bash

function create_node() {
	FILE=`printf "%s\n%s" "$FILE" "<$1>"`
	FILE=`printf "%s\n%s" "$FILE" "<path>"`
	FILE=`printf "%s\n%s%s" "$FILE" "$PATH" "/$1.$2"`
	FILE=`printf "%s\n%s" "$FILE" "</path>"`
	FILE=`printf "%s\n%s" "$FILE" "<sprite_cnt>"`
	FILE=`printf "%s\n%s" "$FILE" "$3"`
	FILE=`printf "%s\n%s" "$FILE" "</sprite_cnt>"`
	FILE=`printf "%s\n%s\n \n" "$FILE" "</$1>"`
}

mkdir ../sprites
cd ../sprites

wget -O herbivore.png http://www.guelnika.net/images/charset/noel/reindeer_eva2.png
wget -O carnivore.png http://eragon-forum.3dn.ru/_fr/6/0245600.png
wget -O grass.png http://cdn.photonesta.com/images/4.bp.blogspot.com/-ggkSFkq3ky4/UXO1HwBEtZI/AAAAAAAAM9M/JuxFjeDrJwk/s1600/Sprite_FX_Grass_0001.png
wget -O sand.png www.wurmpedia.com/images/a/a7/Sand.png
wget -O soil.png http://www.buildbase.co.uk/img/group/Q/soil.png
wget -O stone.png http://www.autoglas-specialist.nl/img/stone.png
wget -O water.jpg http://www.sharecg.com/images/medium/3020.jpg
wget -O tree.png http://vignette2.wikia.nocookie.net/herebemonsters/images/4/40/Mango-Tree-Sprite.png/revision/latest?cb=20140328231536
wget -O unit.png http://www.serwo.pl/images/promo1.png
wget -O carnivore_dead.png http://s33.postimg.org/yxxyz4o8b/carnivore_dead.png
wget -O herbivore_dead.png http://s33.postimg.org/mkl4s7yjv/herbivore_dead.png 

PATH=`pwd`

FILE="<evolva>"

create_node "grass" "png" "1"
create_node "water" "jpg" "1"
create_node "sand" "png" "1"
create_node "soil" "png" "1"
create_node "tree" "png" "1"
create_node "herbivore" "png" "4"
create_node "carnivore" "png" "4"
create_node "unit" "png" "1"
create_node "stone" "png" "1"
create_node "carnivore_dead" "png" "1"
create_node "herbivore_dead" "png" "1"
FILE+="</evolva>"

cd ../tests

echo -e "$FILE" > gui.xml

echo "file gui.xml is located in \"tests\" directory"
