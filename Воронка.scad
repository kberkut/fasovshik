$fn = $preview ? 100 : 100; // Разрешение модели

module move(x=0,y=0,z=0,rx=0,ry=0,rz=0) // Модуль перемещения
{translate([x,y,z]) rotate([rx,ry,rz]) children();}


difference() {
scale([1,1.5,1]) voronka();
move(y=-100, z=-10) cube(200);
}





d1=150;
d2=55;
th=3;
h=30;
h2=15;



module voronka(){

difference() {
union() {
rotate_extrude(angle=360, convexity=10)  {
move(x=d2/2-th) polygon([[0,h],[th,h],[(d1-d2)/2,0],[(d1-d2)/2-th,0]], convexity = 10);
move(x=d2/2-th, y=h) polygon([[0,0],[th,0],[th,h2],[0,h2]], convexity = 10);
move(x=d1/2-2*th, y=-5) polygon([[0,0],[th,0],[th,5],[0,5]], convexity = 10);
}
move(x=-5, y=d2/2-th, z=h+h2/2-5) cube(10);
}

  move(y=d2/2+10, z=h+h2/2) move(rx=90) cylinder(h = 20, d = 3);
}
}