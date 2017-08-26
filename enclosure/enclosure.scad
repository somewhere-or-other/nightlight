include <./Arduino_OpenSCAD_mounting_library_(v2.0)/arduino.scad>

boardType = UNO;
ventHoles = FALSE;
wall = 3;
offset = 2;
heightExtension = 20;
cornerRadius = 10;
mountType = PINS;


union ()	{
	enclosure(boardType, wall, offset, heightExtension, cornerRadius, mountType);

	translate([100,69,5]) {
		rotate([180, 0, 0]) {
			enclosureLid(boardType, wall, offset, cornerRadius, ventHoles);
		}
	}

//	translate([100, 0, 0]) {
//		enclosureLid(boardType, wall, offset, cornerRadius, ventHoles);
//	}

//	translate([0, 0, 10]) {
//		boardShape(boardType);
//	}
}
