// Persistence Of Vision Raytracer 
// Standard shapes include file.

/*

  NOTE:  Some of these shapes, in particular, the "Disk_?" group, have
  been changed in such a way that they will not work correctly with some of
  the old scene files.  The file "shapes.old" is also included in this package
  for exactly this purpose.  The reason for this change was to provide a set
  of shapes that would scale in a consistant fashion.  We apologize for
  any inconvenience that this may cause in the short term, but expect
  that you'll quickly come to agree that this is easier to learn and to
  use.

  The quadric Sphere has been duplicated under the preferred name of
  Ellipsoid to differentiate it from the internal sphere primitive.  Please
  refer to Ellipsoid when you wish to use a quadric sphere.


  Revised shapes.inc
  All primitives scale the same as a sphere with radius=1.
  Changes:
      Disk_X (was X_Disk)
      Disk_Y (was Y_Disk)
      Disk_Z (was Z_Disk)
      QCone_X (was Cone_X)
      QCone_Y (was Cone_Y)
      QCone_Z (was Cone_Z)
  New: 
       UnitBox
       Ellipsoid
       Cone_X (now unit shape intersection instead of infinite quadric)
       Cone_Y (now unit shape intersection instead of infinite quadric)
       Cone_Z (now unit shape intersection instead of infinite quadric)
  Moved: to shapes2.pov
      Square_X
      Square_Y
      Square_Z
      Pyramid
      Tetrahedron
*/

#declare Ellipsoid = quadric {
   <1.0 1.0 1.0>
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   -1.0
}
#declare Sphere = quadric {
   <1.0 1.0 1.0>
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   -1.0
}

#declare Cylinder_X = quadric {
   <0.0 1.0 1.0>
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   -1.0
}

#declare Cylinder_Y = quadric {
   <1.0 0.0 1.0>
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   -1.0
}

#declare Cylinder_Z = quadric {
   <1.0 1.0 0.0>
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   -1.0
}

// Infinite cones
#declare QCone_X = quadric {
   <-1.0 1.0 1.0>
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   0.0
}

#declare QCone_Y = quadric {
   <1.0 -1.0 1.0>
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   0.0
}

#declare QCone_Z = quadric {
   <1.0 1.0 -1.0>
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   0.0
}

// Unit cones    
//
// Use: intersection { Cone_X }
//
#declare Cone_X = 
intersection { 
   quadric { QCone_X translate <0 1 0> } 
   box { <-2 -1 -2> <2 1 2> }
   scale <.5 1 .5>
}
#declare Cone_Y = 
intersection { 
   quadric { QCone_Y  translate <0 1 0>} 
   box { <-2 -1 -2> <2 1 2> }
   scale <.5 1 .5>
}
#declare Cone_Z = 
intersection { 
   quadric { QCone_Z translate <0 1 0>} 
   box { <-2 -1 -2> <2 1 2> }
   scale <.5 1 .5>
}

#declare Plane_YZ = quadric {
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   <1.0 0.0 0.0>
   0.0
}

#declare Plane_XZ = quadric {
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   <0.0 1.0 0.0>
   0.0
}

#declare Plane_XY = quadric {
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   <0.0 0.0 1.0>
   0.0
}

/* y^2 + z^2 - x = 0 */
#declare Paraboloid_X = quadric {
   <0.0  1.0  1.0>
   <0.0  0.0  0.0>
   <-1.0 0.0  0.0>
   0.0
}

/* x^2 + z^2 - y = 0 */
#declare Paraboloid_Y = quadric {
   <1.0  0.0  1.0>
   <0.0  0.0  0.0>
   <0.0 -1.0  0.0>
   0.0
}

/* x^2 + y^2 - z = 0 */
#declare Paraboloid_Z = quadric {
   <1.0  1.0  0.0>
   <0.0  0.0  0.0>
   <0.0  0.0 -1.0>
   0.0
}

/* y - x^2 + z^2 = 0 */
#declare Hyperboloid = quadric {
   < -1.0  0.0  1.0>
   <  0.0  0.0  0.0>
   <  0.0  1.0  0.0>
   0.0
}

#declare Hyperboloid_Y = quadric {  /* Vertical hyperboloid */
   <1.0 -1.0  1.0>          /*                      */
   <0.0  0.0  0.0>          /*            \   /     */
   <0.0  0.0  0.0>          /* Like this:  ) (      */
   -1.0                     /*            /   \     */
}


// Cube using the procedural box primitive
#declare UnitBox = box { <-1.0 -1.0 -1.0> <1.0 1.0 1.0> }

// This primitive used to be an intersection of six planes.  For speed, it's now
// an intersection of a box and nothing else.
#declare Cube = intersection { box { <-1 -1 -1> <1 1 1>} }

// The Disk primitives are "capped" cylinders of unit length.
//
// Note: These three shapes have been changed for Version 1.0.
// They are now "unit" size, the same as a sphere with a radius of 1.
// They will now scale evenly in all directions.
// Old files may need to be modified to work with these shapes, but
// I think you'll find them to be more consistant with the rest of the
// POV-Ray primitives and easier for novices to understand.

#declare Disk_X = intersection {       /* Capped cylinder, Length in x axis */
   quadric { Cylinder_X  }
   plane { <1.0 0.0 0.0> -1 inverse  }
   plane { <1.0 0.0 0.0>  1  }
}

#declare Disk_Y = intersection {       /* Capped cylinder, Length in y axis */
   quadric { Cylinder_Y  }
   plane { <0.0 1.0 0.0> -1 inverse }
   plane { <0.0 1.0 0.0>  1 }
}

#declare Disk_Z = intersection {       /* Capped cylinder, Length in z axis */
   quadric { Cylinder_Z  }
   plane { <0.0 0.0 1.0> -1 inverse  }
   plane { <0.0 0.0 1.0>  1  }
}
