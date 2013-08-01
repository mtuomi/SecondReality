// Persistence Of Vision Raytracer 
// Standard shapes include file #2.
//
/* Contents:
		   Tetrahedron
		   Octahedron
		   Dodecahedron
		   Icosahedron
		   Rhomboid
		   Hexagon
		   HalfCone_Y
		   Pyramid
		   Pyramid2 
		   Square_X
		   Square_Y
		   Square_Z
*/



// Shapes by Tom Price [75300,620]:
#declare Tetrahedron = intersection {
   plane { <0.0 -1.0 0.0> 1.0 }
   plane { <0.0 0.0 -1.0> 1.0 rotate <19.47 0.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <19.47 -120.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <19.47 120.0 0.0> }
}

#declare Octahedron = intersection {
   plane { <0.0 0.0 1.0> 1.0 rotate <35.26438968275 0.0 0.0> }
   plane { <0.0 0.0 1.0> 1.0 rotate <-35.26438968275 0.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <35.26438968275 0.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <-35.26438968275 0.0 0.0> }

   plane { <1.0 0.0 0.0> 1.0 rotate <0.0 0.0 -35.26438968275> }
   plane { <1.0 0.0 0.0> 1.0 rotate <0.0 0.0 35.26438968275> }
   plane { <-1.0 0.0 0.0> 1.0 rotate <0.0 0.0 -35.26438968275> }
   plane { <-1.0 0.0 0.0> 1.0 rotate <0.0 0.0 35.26438968275> }
}

#declare Dodecahedron = intersection {
   plane { <0.0 0.0 -1.0>  1.0 rotate <-26.56505117708 0.0 0.0> }
   plane { <0.0 0.0 -1.0>  1.0 rotate <-26.56505117708 -72.0 0.0> }
   plane { <0.0 0.0 -1.0>  1.0 rotate <-26.56505117708 -144.0 0.0> }
   plane { <0.0 0.0 -1.0>  1.0 rotate <-26.56505117708 -216.0 0.0> }
   plane { <0.0 0.0 -1.0>  1.0 rotate <-26.56505117708 -288.0 0.0> }

   plane { <0.0 0.0 -1.0>  1.0 rotate <26.56505117708 -36.0 0.0> }
   plane { <0.0 0.0 -1.0>  1.0 rotate <26.56505117708 -108.0 0.0> }
   plane { <0.0 0.0 -1.0>  1.0 rotate <26.56505117708 -180.0 0.0> }
   plane { <0.0 0.0 -1.0>  1.0 rotate <26.56505117708 -252.0 0.0> }
   plane { <0.0 0.0 -1.0>  1.0 rotate <26.56505117708 -324.0 0.0> }

   plane { <0.0 1.0 0.0> 1.0 }
   plane { <0.0 -1.0 0.0> 1.0 }
}

#declare Icosahedron = intersection {

   plane { <0.0 0.0 -1.0> 1.0 rotate <52.6625 0.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <52.6625 -72.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <52.6625 -144.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <52.6625 -216.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <52.6625 -288.0 0.0> }

   plane { <0.0 0.0 -1.0> 1.0 rotate <10.8125 0.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <10.8125 -72.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <10.8125 -144.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <10.8125 -216.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <10.8125 -288.0 0.0> }

   plane { <0.0 0.0 -1.0> 1.0 rotate <-52.6625 -36.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <-52.6625 -108.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <-52.6625 -180.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <-52.6625 -252.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <-52.6625 -324.0 0.0> }

   plane { <0.0 0.0 -1.0> 1.0 rotate <-10.8125 -36.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <-10.8125 -108.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <-10.8125 -180.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <-10.8125 -252.0 0.0> }
   plane { <0.0 0.0 -1.0> 1.0 rotate <-10.8125 -324.0 0.0> }
}

// Shapes by others

// Convenient  finite cone primitive, pointing up in the Y axis
// Cone_Y, X & Z in SHAPES.INC should be used instead of this shape
#declare HalfCone_Y = intersection {
   quadric { // QCone_Y
      <1.0 -1.0 1.0>
      <0.0 0.0 0.0>
      <0.0 0.0 0.0>
       0.0
   }
   plane { <0 1 0 >  0 }
   plane { <0 1 0 > -2 inverse }
   translate <0 1 0>
   scale <0.5 1 0.5>
}

/* Hexagonal Solid, axis along x */
#declare  Hexagon = intersection {
   plane { <0.0 0.0 1.0> 1.0  }  /* Rotate 90 in z axis to stand up */
   plane { <0.0 0.0 1.0> 1.0 rotate <60.0 0.0 0.0> }
   plane { <0.0 0.0 1.0> 1.0 rotate <120.0 0.0 0.0> }
   plane { <0.0 0.0 1.0> 1.0 rotate <180.0 0.0 0.0> }
   plane { <0.0 0.0 1.0> 1.0 rotate <240.0 0.0 0.0> }
   plane { <0.0 0.0 1.0> 1.0 rotate <300.0 0.0 0.0> }
   plane { <1.0 0.0 0.0> 1.0 }
   plane { <1.0 0.0 0.0> -1.0 inverse }
}

/* Three Dimensional 4-Sided Diamond */
#declare Rhomboid = intersection {
   plane { <-1.0  0.0  0.0>  1.0 rotate <0.0 0.0 -30.0> }
   plane { < 1.0  0.0  0.0>  1.0 rotate <0.0 0.0 -30.0> }
   plane { < 0.0  0.0  1.0>  1.0 }
   plane { < 0.0  0.0 -1.0>  1.0 }
   plane { < 0.0  1.0  0.0>  1.0 }
   plane { < 0.0 -1.0  0.0>  1.0 }
}

// Classic four-sided pyramids.
// The first can't be used correctly in CSG, the second can.
#declare Pyramid = intersection {
   union { // This isn't true CSG, it's just used for convenience
      triangle { <-1 0 -1> <+1 0 -1> <0 1 0>  }
      triangle { <+1 0 -1> <+1 0 +1> <0 1 0>  }
      triangle { <-1 0 +1> <+1 0 +1> <0 1 0>  }
      triangle { <-1 0 +1> <-1 0 -1> <0 1 0>  }
   }
   scale <1 2 1>
   translate <0 -1 0>
}
#declare Pyramid2 = intersection {
   plane { <1 0 0> 1  rotate <0 0 40> }
   plane { <-1 0 0> 1 rotate <0 0 -40>}
   plane { <0 0 1> 1  rotate <-40 0 0> }
   plane { <0 0 -1> 1 rotate <40 0 0> }
   plane { <0 -1 0> 0 }
   translate <0 -1 0>
}

// These next three are finite planes.
#declare Square_X =  union {    /* Scale-able plane in x */
   triangle { < 0  1 -1> < 0 -1  1> < 0  1  1>  }
   triangle { < 0  1 -1> < 0 -1  1> < 0 -1 -1>  }
}

#declare Square_Y =  union {    /* Scale-able plane in y */
   triangle { <-1 0 1>   < 1 0 -1>  <1 0 1>  }
   triangle { <-1 0  1>  < 1 0 -1>  <-1 0 -1>}
}

#declare Square_Z =  union {    /* Scale-able plane in z */
   triangle { <-1 1 0.0> <1 -1 0.0> <-1 -1 0.0> }
   triangle { <-1 1 0.0> <1 -1 0.0> <1 1 0.0>   }
}
