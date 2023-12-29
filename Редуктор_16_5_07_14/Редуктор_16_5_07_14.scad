$fn = $preview ? 100 : 100; // Разрешение модели

module move(x=0,y=0,z=0,rx=0,ry=0,rz=0) // Модуль перемещения
{translate([x,y,z]) rotate([rx,ry,rz]) children();}

module lineika(delta, lenght) { for(i=[0:2:lenght]){
  move(x=delta*i) move(y=0) cube(delta);
  move(x=delta*i) move(y=-delta, x=delta) cube(delta);
}
}




// Для определения масштаба
*move(rz=360/15*0.5) union(){
 move(x=-16) cylinder(h=1, r=2.5);
mx=0.1;
 color("blue") move(x=-16, z=4) cylinder(h=1, r=2.5+mx);
}
m=1-0.01;



// Эксцентерситет
scale([m,m,1]) color("cyan") move(z=0) linear_extrude(height = 5, convexity = 10)
		import("/work/Projects_tech/Project_name/Фасовщик/Редуктор_16_5_07_14/Редуктор_16_5_07_14_IN.dxf");


height_base=20;
difference(){
  move(z=height_base/2) cube([40,40,height_base],center=true);
color("red", 1) union(){
difference(){
  cylinder(h=height_base, r=14.52, center=false);

move(x=0) linear_extrude(height = height_base, convexity = 10)
		import("/work/Projects_tech/Project_name/Фасовщик/Редуктор_16_5_07_14/Редуктор_16_5_07_14_OUT.dxf");
}
for(i=[0:1:14]){
move(rz=360/15*i) move(x=-13.87) cylinder(h=height_base, r=1.27207);
}
}

move(z=13) cube([100,100,20],center=true);
}

*move(z=20, rz=180+5+5+1+1) color("yellow") lineika(1,20);