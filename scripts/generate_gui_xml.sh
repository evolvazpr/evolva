#!/bin/bash
set -e
mkdir sprites
cd sprites
wget -O deer_all_directions.png		http://www.guelnika.net/images/charset/noel/reindeer_eva2.png
wget -O tiger_all_directions.png	http://eragon-forum.3dn.ru/_fr/6/0245600.png
wget -O grass.png	http://cdn.photonesta.com/images/4.bp.blogspot.com/-ggkSFkq3ky4/UXO1HwBEtZI/AAAAAAAAM9M/JuxFjeDrJwk/s1600/Sprite_FX_Grass_0001.png
wget -O sand.png	 www.wurmpedia.com/images/a/a7/Sand.png
wget -O soil.png	http://www.buildbase.co.uk/img/group/Q/soil.png
wget -O stone.png	http://www.autoglas-specialist.nl/img/stone.png
wget -O water.png	http://www.sharecg.com/images/medium/3020.jpg
wget -O tree.png	http://vignette2.wikia.nocookie.net/herebemonsters/images/4/40/Mango-Tree-Sprite.png/revision/latest?cb=20140328231536
wget -O unit.png	http://www.serwo.pl/images/promo1.png

PATH=`pwd`

FILE="<evolva>"

#grass
FILE+="<grass>"
FILE+="<path>"
FILE+=`echo "$PATH"`
FILE+="/grass.png"
FILE+="</path>"
FILE+=`echo`
FILE+="<sprite_cnt>1</sprite_cnt>"
FILE+="</grass>"
FILE+=`echo`
#water
FILE+="<water>"
FILE+="<path>"
FILE+=`echo "$PATH"`
FILE+="/water.png"
FILE+="</path>"
FILE+="<sprite_cnt>1</sprite_cnt>"
FILE+="</water>"

#sand
FILE+="<sand>"
FILE+="<path>"
FILE+=`echo "$PATH"`
FILE+="/sand.png"
FILE+="</path>"
FILE+="<sprite_cnt>1</sprite_cnt>"
FILE+="</sand>"

#soil
FILE+="<soil>"
FILE+="<path>"
FILE+=`echo "$PATH"`
FILE+="/soil.png"
FILE+="</path>"
FILE+="<sprite_cnt>1</sprite_cnt>"
FILE+="</soil>"


#tree
FILE+="<tree>"
FILE+="<path>"
FILE+=`echo "$PATH"`
FILE+="/tree.png"
FILE+="</path>"
FILE+="<sprite_cnt>1</sprite_cnt>"
FILE+="</tree>"

#herbivore
FILE+="<herbivore>"
FILE+="<path>"
FILE+=`echo "$PATH"`
FILE+="/deer_all_directions.png"
FILE+="</path>"
FILE+="<sprite_cnt>4</sprite_cnt>"
FILE+="</herbivore>"

#carnivore
FILE+="<carnivore>"
FILE+="<path>"
FILE+=`echo "$PATH"`
FILE+="/tiger_all_directions.png"
FILE+="</path>"
FILE+="<sprite_cnt>4</sprite_cnt>"
FILE+="</carnivore>"

#unit
FILE+="<unit>"
FILE+="<path>"
FILE+=`echo "$PATH"`
FILE+="/unit.png"
FILE+="</path>"
FILE+="<sprite_cnt>1</sprite_cnt>"
FILE+="</unit>"


FILE+="</evolva>"

cd ..
echo "$FILE" >> gui.xml
