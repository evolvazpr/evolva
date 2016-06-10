function create_node($name, $format, $sprite_cnt) {
	
	$string = "<{0}>`r`n" -f $name
	$string += "<path>`r`n"
	$string += "{0}\{1}.{2}`r`n" -f (Get-Location).Path, $name, $format
	$string += "</path>`r`n"
	$string += "<sprite_cnt>`r`n"
	$string += "{0}`r`n" -f $sprite_cnt
	$string += "</sprite_cnt>`r`n"
	$string += "</{0}>`r`n`r`n" -f $name
	
	return $string
}

function create_gui_setting() {
	$string = "<Gui>`r`n"
	$string += "<pixels_per_object>`r`n"
	$string += "25`r`n"
	$string += "</pixels_per_object>`r`n"
	$string += "</Gui>`r`n"

	return $string
}

mkdir ../sprites
cd ../sprites

wget http://www.guelnika.net/images/charset/noel/reindeer_eva2.png -OutFile herbivore.png
wget http://eragon-forum.3dn.ru/_fr/6/0245600.png -OutFile carnivore.png
wget http://cdn.photonesta.com/images/4.bp.blogspot.com/-ggkSFkq3ky4/UXO1HwBEtZI/AAAAAAAAM9M/JuxFjeDrJwk/s1600/Sprite_FX_Grass_0001.png -OutFile grass.png 
wget http://www.wurmpedia.com/images/a/a7/Sand.png -OutFile sand.png 
wget http://www.buildbase.co.uk/img/group/Q/soil.png -OutFile soil.png 
wget http://www.autoglas-specialist.nl/img/stone.png -OutFile stone.png 
wget http://www.sharecg.com/images/medium/3020.jpg -OutFile water.jpg 
wget http://vignette2.wikia.nocookie.net/herebemonsters/images/4/40/Mango-Tree-Sprite.png/revision/latest?cb=20140328231536 -OutFile tree.png 
wget http://www.serwo.pl/images/promo1.png  -OutFile unit.png
wget http://s33.postimg.org/yxxyz4o8b/carnivore_dead.png -OutFile carnivore_dead.png
wget http://s33.postimg.org/mkl4s7yjv/herbivore_dead.png -OutFile herbivore_dead.png


$text="<evolva>`r`n`r`n"
$text += create_node "grass" "png" "1"
$text += create_node "water" "jpg" "1"
$text += create_node "sand" "png" "1"
$text += create_node "soil" "png" "1"
$text += create_node "tree" "png" "1"
$text += create_node "herbivore" "png" "4"
$text += create_node "carnivore" "png" "4"
$text += create_node "unit" "png" "1"
$text += create_node "stone" "png" "1"
$text += create_node "carnivore_dead" "png" "1"
$text += create_node "herbivore_dead" "png" "1"
$text += create_gui_setting

$text+="</evolva>"

cd ../tests
$text | out-file "gui.xml" -Encoding ASCII
