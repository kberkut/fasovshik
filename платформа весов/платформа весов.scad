$fn = $preview ? 100 : 100; // Разрешение модели

module move(x=0,y=0,z=0,rx=0,ry=0,rz=0) // Модуль перемещения
{translate([x,y,z]) rotate([rx,ry,rz]) children();}




difference(){
  union() {
  poddon();
  *move(z=H, rx=180) krishka();
  *move(z=2*H-9.5-4, rx=180) zazim();
  }
  *move(x=-60-2) cube(120);
}

%move(x=dL*2+dL/2+think/2-dat_L/2, y=7*think-3.5, z=h+3) color("blue",0.5) cube([dat_L, dat_L0, dat_H]);



length=120;
h=10;
  H=17; // Высота крышки
think=3.2;
n=5;
L=(length-think)/n;
Len2=length-2*L;
n2=3;
dL=(Len2-think)/n2;

dat_L=12; // Длина площадки крепления датчика
dat_W=8; // Ширина площадки крепления датчика
dat_H=6.15; // Толщина датчика
dat_C1=6.15; //растояние между центрами отверстий крепления датчика по ширине
dat_C2=40.3;// растояние между центрами отверстий крепления датчика по длине
dat_L0=47; // Длина датчика


// Размеры платы hx11
HX11_W=16;
HX11_L=24;
HX11_H=3;

module poddon() {
difference() { 
  union() { 
    difference() {
    hull() {
      r=5;
    move(x=r, y=r) cylinder(h=h, r=r);
    move(x=length-r, y=r) cylinder(h=h, r=r);
    move(x=r, y=length-r) cylinder(h=h, r=r);
    move(x=length-r, y=length-r) cylinder(h=h, r=r);
    }
    move(z=3) hull() {
      r=5;
      h=h;
    move(x=r+think, y=r+think) cylinder(h=h, r=r);
    move(x=length-r-think, y=r+think) cylinder(h=h, r=r);
    move(x=r+think, y=length-r-think) cylinder(h=h, r=r);
    move(x=length-r-think, y=length-r-think) cylinder(h=h, r=r);
    }
    }


    move(x=dL) for (i=[0:n2]) {
    move(x=dL*i) cube([think/2, length,h]);
    }
    move(y=dL) for (i=[0:n2]) {
    move(y=dL*i) cube([length, think/2, h]);
    }
    move(x=2*dL, y=2*dL) cube([dL, dL, h]);

    move(x=2*dL+think+(dL-think)/2, y=think*7+dat_L0-3.5*2, z=h+3/2) cube([dat_L+3+3, dat_W, 3], center=true);
}
move(x=2*dL+think+(dL-think)/2, y=think*7+dat_L0-3.5*2) union() {
  hull(){
move(x=-dat_C1/2-3) cylinder(h = 20, d = 3.3);
move(x=dat_C1/2+3) cylinder(h = 20, d = 3.3);
  }
  hull (){
move(x=-dat_C1/2-3, z=-20+2.8+1) cylinder(h = 20, d = 10);
move(x=dat_C1/2+3, z=-20+2.8+1) cylinder(h = 20, d = 10);
  }
}
}
}




module krishka() {

  difference() {
  union() {
difference(){
  hull() {
  r=5;
  move(x=r, y=r) cylinder(h=H, r=r);
  move(x=length-r, y=r) cylinder(h=H, r=r);
  move(x=r, y=length-r) cylinder(h=H, r=r);
  move(x=length-r, y=length-r) cylinder(h=H, r=r);
}
move(z=-0.1) hull() {
  r=5;
  h=H-think+0.1;
  move(x=r+think, y=r+think) cylinder(h=h, r=r);
  move(x=length-r-think, y=r+think) cylinder(h=h, r=r);
  move(x=r+think, y=length-r-think) cylinder(h=h, r=r);
  move(x=length-r-think, y=length-r-think) cylinder(h=h, r=r);
}
}    
move(x=2*dL+think+(dL-think)/2, y=think*7, z=1.5+1+dat_H) cube([dat_L, dat_W, 3], center=true);
move(x=2*dL+think+(dL-think)/2, y=think*7, z=1.5+1+dat_H+4) cube([dat_L, dat_W*2, 5], center=true);

difference() {
move(y=15, z=H-9.5) hull() {
move(x=9) cylinder(h = 9.5, r = 4);
move(x=21) cylinder(h = 9.5, r = 4);
}
move(x=15, y=20,z=H-9.5-3+3/2, rx=90) cylinder(h = 10, r = 3);
move(y=15, z=H-9.5) union() {
move(x=9) cylinder(h = 9.5, r = 1);
move(x=21) cylinder(h = 9.5, r = 1);
}
}
  }

move(x=2*dL+think+(dL-think)/2, y=think*7) union() {
move(x=-dat_C1/2) cylinder(h = 20, d = 3.3);
move(x=dat_C1/2) cylinder(h = 20, d = 3.3);
move(x=-dat_C1/2, z=H-2.8) cylinder(h = 20, d = 6);
move(x=dat_C1/2, z=H-2.8) cylinder(h = 20, d = 6);
}
move(x=15, y=5,z=H-9.5, rx=90) cylinder(h = 10, d = 3.5);
  }
move(x=think,y=30,z=H-think-HX11_H) difference() {
  delta=2;
cube([HX11_W+delta*2, HX11_L+delta*2, HX11_H]);
move(x=delta,y=delta) cube([HX11_W, HX11_L, HX11_H]);
}
}


module zazim() {
  difference() {
  move(y=15) hull() {
move(x=9) cylinder(h = 4, r = 4);
move(x=21) cylinder(h = 4, r = 4);
}
move(y=15) union() {
move(x=9) cylinder(h = 4, r2 = 1, r1 = 2.8);
move(x=21) cylinder(h = 4, r2 = 1, r1 = 2.8);
move(x=15, y=5,z=6+1-3+3/2, rx=90) cylinder(h = 10, r = 3);
}

  }
}