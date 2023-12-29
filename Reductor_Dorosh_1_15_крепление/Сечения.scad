

$fn = $preview ? 100 : 100; // Разрешение модели

module move(x=0,y=0,z=0,rx=0,ry=0,rz=0) // Модуль перемещения
{translate([x,y,z]) rotate([rx,ry,rz]) children();}

projection(cut = true) 
//move(z=0) difference(){
//import("/work/Projects_tech/Project_name/Фасовщик/Reductor_Dorosh_1_15/re31.stl");
//move(z=-3) import("/work/Projects_tech/Project_name/Фасовщик/Reductor_Dorosh_1_15/re21.stl");
import("/work/Projects_tech/Project_name/Фасовщик/temp2.stl");

//move(z=-3) cube([50,50,6], center=true);
//}