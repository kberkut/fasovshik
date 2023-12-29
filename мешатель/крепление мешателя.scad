$fn = $preview ? 100 : 100; // Разрешение модели

module move(x=0,y=0,z=0,rx=0,ry=0,rz=0) // Модуль перемещения
{translate([x,y,z]) rotate([rx,ry,rz]) children();}






difference(){
  union() {
*tube();
*reductor();
*kreplRed();
*move(rx=90) krepl_tube();
move(rx=0) nasadka();
*move(z=reductor_H+moto_H+h_bort+5, rx=180) krishka();

  }
  *move(x=-60, z=-10) cube(120);
}

filament=0.2;
// Параметры моторчика
reductor_W=10;
reductor_L=12;
reductor_H=9;
reductor_d1=4;
reductor_d1h=1.5;
moto_H=15; // высота движка до площадки с выводами

module reductor() {
move(x=-(reductor_L)/2, y=-(reductor_W)/2) cube([reductor_L+filament*1, reductor_W+filament*1, reductor_H+moto_H]);
move(z=-10) cylinder(h = 10, d = reductor_d1+filament);
}



module tube(){
  difference() {
cylinder(h = 100, d = 50);
move(z=-5) cylinder(h = 110, d = 47);
  }
}


module kreplRed() {
  difference() {

union() {
   cylinder(h = reductor_H+moto_H+h_bort, d = 22);
move(z=-5) cylinder(h=20, d1=30, d2=22);
// move(z=0) minkowski() {
//    cube([40,10,6], center=true);
//    cylinder(r=2,h=1);
// }
move(x=10, z=16) cylinder(h = 8.6, d = 8);
move(x=10, z=16-8.6) cylinder(h = 8.6, d1 = 3, d2 = 8);
move(x=-10, z=16) cylinder(h = 8.6, d = 8);
move(x=-10, z=16-8.6) cylinder(h = 8.6, d1 = 3, d2 = 8);
move(y=10, z=-55/2, rx=90) cylinder(h=20, d=55+15);
move(x=(55+15)/2, z=-55/2+0.5) cylinder(h = 6, d = 8);
move(x=-(55+15)/2, z=-55/2+0.5) cylinder(h = 6, d = 8);
}


//#move(x=17) cube([3,6,15], center=true);
//#move(x=-17) cube([3,6,15], center=true);
move(x=10, z=15) cylinder(h = 10, d = 2.7);
move(x=-10, z=15) cylinder(h = 10, d = 2.7);
move(z=h_bort, rz=90) reductor();
move(y=25, z=-55/2, rx=90) cylinder(h=50, d=55);
//#move(z=-6) cube([50,20,6], center=true);
move(z=-20-55/2+0.5) cube([80,30,40], center=true);
move(x=(55+15)/2, z=-55/2+0.5) cylinder(h = 10, d = 3);
move(x=(55+15)/2, z=-55/2+0.5+6) cylinder(h = 10, d = 8);
move(x=-(55+15)/2, z=-55/2+0.5) cylinder(h = 10, d = 3);
move(x=-(55+15)/2, z=-55/2+0.5+6) cylinder(h = 10, d = 8);
  }
}


module krepl_tube() {
  difference() {
union() {
move(y=10, z=-55/2, rx=90) cylinder(h=20, d=55+15);
move(x=(55+15)/2, z=-6-55/2-0.5) cylinder(h = 6, d = 8);
move(x=-(55+15)/2, z=-6-55/2-0.5) cylinder(h = 6, d = 8);
}
move(y=25, z=-55/2, rx=90) cylinder(h=50, d=55);
move(z=20-55/2-0.5) cube([80,30,40], center=true);
move(x=(55+15)/2, z=-55/2+0.5-10) cylinder(h = 10, d = 3);
move(x=(55+15)/2, z=-55/2-0.5-6-10) cylinder(h = 10, d = 8);
move(x=-(55+15)/2, z=-55/2+0.5-10) cylinder(h = 10, d = 3);
move(x=-(55+15)/2, z=-55/2-0.5-6-10) cylinder(h = 10, d = 8);
  }
  
}





// Параметры вала двигателя
d_vala=3; //Диаметр вала
l_srez=2.5; // растояние от грани среза до противоположной точки на оси
h_bort=0.6;
l_vala=10-h_bort-4; // Полная днина вала


module nasadka() {
  difference() {
union() {    
hull(){
cylinder(h = 2.5, d = 8);
move(x=13) cylinder(h = 2.5, d = 6);
}

cylinder(h = l_vala+4+1, d = 8);
*move(z=l_vala+4) cylinder(h = 1, d1 = 8, d2 = d_vala+filament);
move(x=1.5,y=-7/2, z=2.5) cube([3.5,7,8]);




}

*move(z=-0.6) difference() {
 move(z=5+9) cube(size = 10, center=true);
move(z=9) cylinder(h = 2, d1=11, d2=3);
}


move(x=13, z=-1) cylinder(h = 10, d = 1);
  move(z=3, rz=-90) difference() {
  cylinder(h = l_vala+2, d = d_vala+filament);
  move(x=-3, y=d_vala/2-(d_vala-l_srez)) cube([6,6,15]);
  }
move(z=2.2/2+2.5+5.5/2+1, ry=90) cylinder(h = 6, d = 2.8);
}
}






module krishka() {
  h0=15;
  h1=8;
  h2=15-8;
  difference() {
  union() {
    cylinder(h = h0, d = 22);
    move(x=10) cylinder(h = h1, d = 8);
    move(x=-10) cylinder(h = h1, d = 8);
  }
move(x=10) cylinder(h = 10, d = 3.1);
move(x=-10) cylinder(h = 10, d = 3.1);
move(x=10, z=h1) cylinder(h = h2*2, d = 8);
move(x=-10, z=h1) cylinder(h = h2*2, d = 8);
move(z=6) cube([6,12,12], center=true);
move(x=0, z=12-1, rx=90) cylinder(h = 20, d = 2);
move(x=10.3, y=-10, z=8, ry=-32) cube(size = 20);
move(x=-10.3, y=-10, z=8, ry=-58) cube(size = 20);
  }
}


