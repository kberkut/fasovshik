$fn = $preview ? 100 : 100; // Разрешение модели

module move(x=0,y=0,z=0,rx=0,ry=0,rz=0) // Модуль перемещения
{translate([x,y,z]) rotate([rx,ry,rz]) children();}

difference(){
  union(){
*move(z=12,rx=180) import("/work/Projects_tech/Project_name/Фасовщик/Reductor_Dorosh_1_15/re11.stl");
*move(z=0) import("/work/Projects_tech/Project_name/Фасовщик/Reductor_Dorosh_1_15/re21.stl");
*move(z=12.1) import("/work/Projects_tech/Project_name/Фасовщик/Reductor_Dorosh_1_15/re31.stl");
*move(z=26.5, rx=180) import("/work/Projects_tech/Project_name/Фасовщик/Reductor_Dorosh_1_15/re41.stl");
*move(z=10) import("/work/Projects_tech/Project_name/Фасовщик/Reductor_Dorosh_1_15/re51.stl");
  }
 *move(x=-50, y=-50, z=30) cube([100,100,150]);
}


z0=26.1+1;
h0=7.66;
h1=10;

//h0=3.5;
d_per=32.5; // Диаметр ножки перехода на с вала мотора на вал приводного механизма
  r_out1=26.8;
d_out2=50.2;
L_square=21*2;
L_otv=21.9;
d_otv=3;
 h_pnd=5+2; // расстояние между шаговым двигателем и зацепом за трубу
h_vistup=7.66-1.5; //Часть переходника редуктора под защелку между резинкой и стенкой ПНД трубы

module kreplenie(){
  //Параметры вала NEMO 17
d_N17=5; //Диаметр вала 5 мм D
L_srez=0.5; //Срез площадки на валу под посадку
L_N17=23.5; //Длина вала сточенная под посадку
fil=0.2; // Компенсация на толщину нити филамента
move(z=z0+h1+h_pnd+h_vistup) // Вставка в приводной механизм
  difference(){
  cylinder(h=L_N17/3, d=d_N17-fil);
  move(x=d_N17-L_srez-fil) cube([d_N17,d_N17,L_N17], center=true);
}

  

  
  
  move(z=z0) // Переходник с двигателя на вал приовдного мехнизма
difference(){
    union(){
    move(z=h_pnd+h_vistup+1) cylinder(h=h1,d=45.5);
     cylinder(h=h_pnd+h_vistup+1,d=d_per-1);  
    }
    move(rz=7.5) for(i=[0:1:360/30]) {
  move(rz=i*30) move(x=15, z=h_pnd+h_vistup+1+0.5) cylinder(h=h1+1,d=2.5);
      cylinder(h=h0-1,d=15.5, $fn=8);
        }
        union(){
          angle=30;
       move(z=h0/2, rz=angle) move(rx=90) cylinder(h=70,d=2.5, center=true);
        move(z=h0/2+0.5, rz=angle) move(rx=90) move(z=-(d_per/2+2-0.5-3)) cylinder(h=4,d=7, center=true);
        move(z=h0/2+0.5, rz=angle) move(rx=90) move(z=(d_per/2+2-0.5-3)) cylinder(h=4,d=7, center=true);
        }
}
  


  move(z=z0) // Крепеление переходника к шаговому двигателю
    difference(){
      move(x=-L_square/2, y=-L_square/2) cube([L_square,L_square,h_pnd+h_vistup]);
      difference(){
        cylinder(h=h_vistup+h_pnd,d=60);
        cylinder(h=h_vistup+h_pnd,d=r_out1*2);
      }
      difference(){
        cylinder(h=h_pnd,d=60);
        cylinder(h=h_pnd,d=d_out2);
      }
       for(i=[0:1:4]){
        move(rz=(90)*i) move(rz=45, z=-5) move(x=L_otv) cylinder(h=16,d=d_otv);
        }
       for(i=[0:1:4]){
        move(rz=(90)*i) move(rz=45) move(x=L_otv, z=h_pnd+h_vistup-3)  cylinder(h=4,d=7);
        }
        cylinder(h=h_vistup+h_pnd+1,d=d_per);
        
      }
    
      
    
    
    
// move(z=z0*2) difference(){  
//    linear_extrude(h0*2, convexity=10) import("/work/Projects_tech/Project_name/Фасовщик/Reductor_Dorosh_1_15_крепление/Площадка крепления.dxf");
//    move(z=0) cylinder(h=h0+1,d=37);
//     move(z=0) difference(){
//        cylinder(h=h0,d=60);
//      cylinder(h=h0,d=51);
//  }
//  for(i=[0:1:4]){
//    move(rz=(90)*i) move(rz=45) move(x=L03) cylinder(h=h0*2+1,d=d1);
//    
//  }  
//  }
 
 
  
  
  



}

difference(){
kreplenie();
  move(rz=30) move(x=0,y=-50)  cube([100,100,150]);
}
