dA = 116.56505;
h = 20;// 1.30902;
n=5;


union() {
	cylinder(h,1,1, $fn=5);
	
	rotate([180,0,180])
	cylinder(h,1,1, $fn=5);
	
	color("green")
for (i=[0:n])
// 	
rotate([0,0,i*360/n])
rotate([0,dA-90,0])
	rotate([0,90,0])
	rotate([0,0,180])
	cylinder(h,1,1, $fn=5);

color("red")
for (i=[0:n])
// 	
rotate([0,0,(i*360/n+180)%360])
rotate([0,90-dA,0])
	rotate([0,90,0])
	cylinder(h,1,1, $fn=5);

}