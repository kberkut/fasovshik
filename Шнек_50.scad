$fn = $preview ? 100 : 100; // Разрешение модели

module move(x=0,y=0,z=0,rx=0,ry=0,rz=0) // Модуль перемещения
{translate([x,y,z]) rotate([rx,ry,rz]) children();}

use </work/Projects_tech/Project_programs/OpenSCAD/LIB/CustomizablePrintableAuger.scad>


d_pnd=46-1; //Внутренний диаметр трубы пнд 50 с учетом зазора 1 мм с каждой стороны
d_val=7; //Диамерт центрального вала
h_osn=2; //Высота площадки
L_sh=145; // Полная длина шнека с площадкой для тройника ПНД50 с двигателем NEMA17 

module shnek() {
/* [Auger] */

//The total amount of twist, in degrees
Auger_twist = 360+180; //[90:1080]

//The radius of the auger's "flight" past the shaft
Auger_flight_radius = d_pnd/2-d_val/2; //[5:50]

//The number of "flights" 
Auger_num_flights = 1; //[1:5]

//The height, from top to bottom of the "shaft"
Auger_flight_length = L_sh; //[10:200]

/* [Printer] */

//The overhang angle your printer is capable of
Printer_overhang_capability =20; //[0:40]

//The thickness of perimeter support material
Auger_perimeter_thickness = 0; //[0:None, 0.8:Thin, 2:Thick]

/* [Uninteresting] */

//The radius of the auger's "shaft"
Auger_shaft_radius = d_val/2; //[1:25]

//The thickness of the "flight" (in the direction of height)
Auger_flight_thickness = 2;  //[0.2:Thin, 1:Medium, 10:Thick]

Auger_handedness = "left";  //["right":Right, "left":Left]

auger(
r1 = Auger_shaft_radius,
r2 = Auger_shaft_radius + Auger_flight_radius,
h = Auger_flight_length,
overhangAngle = Printer_overhang_capability,
multiStart = Auger_num_flights,
flightThickness = Auger_flight_thickness,
turns = Auger_twist/360,
pitch=0,
supportThickness = Auger_perimeter_thickness,
handedness=Auger_handedness,
$fn=300,
$fa=12,
$fs=5
);
}

//Параметры вала NEMO 17
d_N17=5; //Диаметр вала 5 мм D
L_srez=0.5; //Срез площадки на валу под посадку
L_N17=23.5; //Длина вала сточенная под посадку
fil=0.2; // Компенсация на толщину нити филамента




difference(){
union(){
shnek();
cylinder(h=h_osn, d=d_pnd);
  cylinder(h=25, d=12);
  move(z=25) cylinder(h=10, d1=12, d2=d_val);
}
move(z=-0.01, rz=-45) difference(){
  cylinder(h=L_N17, d=d_N17+fil);
  move(x=d_N17-L_srez+fil, z=L_N17/2) cube([d_N17,d_N17,L_N17], center=true);
}

*cube([80,80,L_sh]);
}

*union(){ // Тестовая модель определения необходимости компенсации для вала
difference(){
move(z=L_N17/4) cube([15,15,L_N17/2], center=true);
difference(){
  cylinder(h=L_N17, d=d_N17+fil);
  move(x=d_N17-L_srez+fil, z=L_N17/2) cube([d_N17,d_N17,L_N17], center=true);
}
}
move(x=-5.5, y=-15/2, z=L_N17/2) color("red") linear_extrude(1) text("0.2",size=6);
move(x=0, y=3, z=L_N17/2) color("red") linear_extrude(1) text("0.6",size=4);
}








%move(rz=90) union () {
alpha=0.3;
color("blue", alpha) cylinder(h=L_sh, d=50);
move(z=50) move(rx=90) color("blue", alpha) cylinder(h=300, d=50);
}
