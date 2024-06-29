
// dihedral angle
dA = 116.56505;
p_alpha = 90;
p_beta = 180 - dA;
p_gamma = 90 - p_beta;

pcb_radius = 17.2; // 85 for xhain dodecahedron

thickness = 5;
frame_thickness = 2;
window_thickness = 1.5;
pcb_thickness = 1.6;

radius = pcb_radius + frame_thickness;

pcb_window_radius = pcb_radius-window_thickness;

side_length = 2 * radius * sin(36);
height = side_length/2 * sqrt(5+2*sqrt(5));
inner_radius = side_length / 10 * sqrt(25+10*sqrt(5));

central_radius = (inner_radius + radius) /2;

p_sideA = inner_radius;
p_sideB = cos(p_gamma) * p_sideA;
p_sideC = cos(p_beta) * p_sideA;

p_height = cos(p_gamma) * height;

p_corner_height = cos(90-72) * side_length;
p_lower_height = cos(p_gamma) * p_corner_height;

echo("cube height: ", p_height + p_lower_height);

echo ();
echo (radius, inner_radius, side_length, height);
echo ();
echo (p_alpha, p_beta, p_gamma);
echo (p_sideA, p_sideB, p_sideC);

module pentagonPlate (radius, thickness, inner_radius, cut_edge)
{
	difference() {
	//union() {
		cube_len = 2 * radius;
		cube_width = 2 * thickness;

		cylinder(thickness, radius, radius, $fn=5);

		if (cut_edge) union() {
			for(i = [0:4])
			{
				// debugging color
				color("red", 0.5)

				// move all five pieces to right spot
				rotate(72*i)

				// move cutting cube to inner radius
				translate([-inner_radius, 0, 0])

				// rotate by dihedral angle of dodecahedron
				//	rotate([ 0, dA, 0])
				rotate([ 0, 180-dA/2, 0])
				{
				
					// move cube to negative coordinates for better rotation
					translate([-cube_width, -cube_len/2, -cube_width]) 

					// draw the corner cutting cube
					cube([cube_width, cube_len, cube_width]);
					
					// magnet
					translate([-thickness+.5, 0,-cube_width]) 
					cylinder(40,1.1,1.1, $fn=25);
				}
			}
		}

		// cut out for pcbs
		//*


		internal = false;
		if (internal) {
			color("red", .25) {
				translate([0,0,-1])
				cylinder(
					2,
					pcb_window_radius,
					pcb_window_radius,
					$fn= 5
				);

				translate([0,0,1])
				cylinder(
					thickness,
					pcb_radius,
					pcb_radius,
					$fn=5
				);
			}
		}
		else {
			color("red", .25) {
				
				
				translate([0,0,pcb_thickness])
				cylinder(
					pcb_thickness+thickness,
					pcb_window_radius,
					pcb_window_radius-thickness/2,
					$fn= 5
				);

				translate([0,0,-.2])
				cylinder(
					pcb_thickness+.4,
					pcb_radius-.25,
					pcb_radius+.1,
					$fn=5
				);
			}

		}
	/**/

	}

}
if(0)
translate([0,0,.2])
color("green", .1)
cylinder(
					pcb_thickness,
					pcb_radius,
					pcb_radius,
					$fn=5
				);

module halfshell(){
	union() {
		// color("red", .25)
		pentagonPlate(radius, thickness, inner_radius, true);

		if (true) for (i=[0:4])
		{

			rotate([0,0,72*i])
			{
				if (false)
				{
					
					/* drawing mounting screw holes */
					pole_h = (p_height + p_lower_height)/2;
//					pole_h = 8;
					pole_r1 = 1;
					pole_r2 = 5;

					translate([central_radius+pole_r1, 0, thickness])
					cylinder(pole_h, pole_r1, pole_r2, $fn=20);
				}
				
				/* drawing angled pentagons */
				//*
				translate([
					-(p_sideC + inner_radius),
					0,
					p_sideB
				])
				/**/
				rotate([180, -dA, 0])
					pentagonPlate(radius, thickness, inner_radius, true);
			}
		}
	}
}

module other_halfshell()
{

translate([0,0,(p_height + p_lower_height)])
rotate([180,0,36])
halfshell();
}

module nokia()
{
x=5.7; y=34; z=53.1;
color("blue") translate([-x/2,-y/2,0]) cube([x,y,z]);
}

module b653450()
{
x=6.5; y=34; z=50;
color("blue") translate([-x/2,-y/2,0]) cube([x,y,z]);
}

module b903052()
{
x=9; y=30; z=52;
color("green") translate([-x/2,-y/2,0]) cube([x,y,z]);
}

module b903440()
{
x=9; y=34; z=40;
	echo ("height" , p_height + p_lower_height);
color("green") translate([-x/2,-y/2,(p_height + p_lower_height-z)/2]) cube([x,y,z]);
}

module battery(x,y,z){
	x=x/10;
	translate([-x/2,-y/2,(p_height + p_lower_height-z)/2]) cube([x,y,z]);
}

module round_battery(d,z){
	r=d/2;
	translate([0,0,(p_height + p_lower_height-z)/2]) cylinder(z,r,r);
}

 // pentagonPlate(radius, thickness, inner_radius, true);
  halfshell();

// translate([0,0,20])
  color("green", .3) 

 other_halfshell();

/*
//  battery(40,30,40);
color("green") 
 battery(102,27,31); // 800
color("blue") 
 battery(90,28,31); // 850

color("yellow")
 battery(90,35,35);

round_battery(20,40);

//battery(30,48,55);
//	b903052();
	// b903440();
// nokia();

*/