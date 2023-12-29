$fn = $preview ? 100 : 100; // Разрешение модели

module move(x=0,y=0,z=0,rx=0,ry=0,rz=0) // Модуль перемещения
{translate([x,y,z]) rotate([rx,ry,rz]) children();}

use </work/Projects_tech/Project_programs/OpenSCAD/LIB/gears-master/gears.scad>




//Параметры вала NEMO 17
d_N17=5; //Диаметр вала 5 мм D
L_srez=0.5; //Срез площадки на валу под посадку
L_N17=12; //23.5; //Длина вала сточенная под посадку
fil=0.2; // Компенсация на толщину нити филамента


module gear(){
planetary_gear(modul=1.1, sun_teeth=12, planet_teeth=12, number_planets=3, width=12, rim_width=1.9, bore=5+fil, pressure_angle=20, helix_angle=10, together_built=true, optimized=true, $fn=200);
}
gear();

move(z=0, rz=0) difference(){
  cylinder(h=L_N17, d=d_N17+fil);
  *move(x=d_N17-L_srez+fil, z=L_N17/2) cube([d_N17,d_N17,L_N17+2], center=true);
  move(x=-L_srez+fil, z=L_N17/2) cube([d_N17,d_N17+2,L_N17+2], center=true);
}


  for(i=[0:1:2]) {
difference(){
move(rz=120*i) move(x=13.2) cylinder(h=12, d=10, center=false);
  move(rz=120*i) move(x=13.2) cylinder(h=12+5, d=4+fil, center=false);
  }
}
%color("cyan", 0.5) move(x=0) cylinder(h=5, d=46, center=false);