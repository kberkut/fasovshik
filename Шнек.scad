$fn = $preview ? 100 : 100; // Разрешение модели

module move(x=0,y=0,z=0,rx=0,ry=0,rz=0) // Модуль перемещения
{translate([x,y,z]) rotate([rx,ry,rz]) children();}


 use </work/Projects_tech/Project_programs/OpenSCAD/LIB/CustomizablePrintableAuger.scad>

module shnek() {
/* [Auger] */

//The total amount of twist, in degrees
Auger_twist = 360; //[90:1080]

//The radius of the auger's "flight" past the shaft
Auger_flight_radius = 46/2-7; //[5:50]

//The number of "flights" 
Auger_num_flights = 2; //[1:5]

//The height, from top to bottom of the "shaft"
Auger_flight_length = 100; //[10:200]

/* [Printer] */

//The overhang angle your printer is capable of
Printer_overhang_capability =20; //[0:40]

//The thickness of perimeter support material
Auger_perimeter_thickness = 0; //[0:None, 0.8:Thin, 2:Thick]

/* [Uninteresting] */

//The radius of the auger's "shaft"
Auger_shaft_radius = 7; //[1:25]

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
$fn=150,
$fa=12,
$fs=5
);
}

h=250;
h2=100;
angl=51.5;
module shnek_2(){
move(rx=0) difference() {
move(z=-2) shnek();
cylinder(h=h, d=6);
  
move(z=5/2-0.2, rz=90) cube([6,15,5+0.2], center=true);
  move(x=-h/2, y=-h/2, z=h2-16) cube([h,h,h]);
  move(x=-h/2, y=-h/2, z=-h) cube([h,h,h]);
}

move(z=100-16, rz=90) difference(){
 move(z=5/2, rz=angl) cube([6,14,5+0.2], center=true);
  move(z=-1) cylinder(h=15, d=6);
}
}


move(rx=0) shnek_2();
*move(z=h2-16+16, rz=angl) color("blue") shnek_2();

*union () {
alpha=0.3;
color("blue", alpha) cylinder(h=300, d=50);
move(z=25) move(rx=90) color("blue", alpha) cylinder(h=300, d=50);
}




