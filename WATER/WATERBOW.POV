// Persistence Of Vision raytracer version 1.0 sample file.

// By Various and Sundry
//
// Revision Note:
// Reworked both the declared wood texture (turb and colormap) and
// the application of it on the floor plane.
// Note that wood doesn't really look like much until you get around
// 640x480.  Anti-aliasing helps even more to bring out the detail.  -dmf

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

// a light tan wood with brown rings
#declare New_Tan_Wood = texture {
   wood
   turbulence 0.03
   colour_map {
      [0.0 0.4  colour red 0.6 green 0.45 blue 0.25
      colour red 0.65 green 0.45 blue 0.25]
      [0.4 1.01 colour red 0.6 green 0.4 blue 0.2
      colour red 0.25 green 0.15 blue 0.05]
   }
}

camera {
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.333 0.0 0.0>
   translate <0.0 0.0 -56.0>
}


// A bowl
object {
   intersection {
      sphere { <0.0 0.0 0.0> 1.0 }
      sphere { <0.0 0.0 0.0> 0.9 }
      plane {  <0.0 1.0 0.0> 0.5 }
   }
   bounded_by {
      sphere { <0.0 0.0 0.0> 21.0 }
   }
   scale < 20.0 20.0 20.0 >
   texture {
      colour Red
      ambient 0.2
      diffuse 0.8
      reflection 0.1
   }
   colour Red
}

// Water
object {
   intersection {
      sphere { <0.0 0.0 0.0> 1.0 }
      plane { <0.0 1.0 0.0> 0.49 }
   }
   bounded_by {
      sphere { <0.0 0.0 0.0> 21.0 }
   }
   scale < 19.5 19.5 19.5 >
   texture {
      ripples 0.5
      frequency 100.0
      scale <100.0 100.0 100.0>
      reflection 0.6
      refraction 0.6
      ior 1.2
   }
   colour Grey
}


// Wood floor
object {
   plane { <0.0 1.0 0.0> -20.0 }

   texture { 0.015            // seems to reduce "moire" effect on the grain

      New_Tan_Wood            // Think of a log, facing you...
      scale <2 2 1>           // z is infinite, so 1 is ok...
      rotate <0 90 0>         // turn the "log" to the x axis
      rotate <0.0 0.0 10.0>   // tilt the log just a little bit
      translate <0 -4  0>     // lift it to where the rings are larger
      rotate <5 0 0>          // tip it again, this time on x axis

      ambient 0.15
      diffuse 0.75
      reflection 0.1
   }
   colour Tan
}


// Back wall
object {
   plane { <0.0 0.0 1.0> 100.0 }

   texture {
      Red_Marble
      scale <100.0 100.0 100.0>
      ambient 0.15
      diffuse 0.8
      reflection 0.1
   }
   color Pink
}

// A sky to reflect in the water
object {
   plane { <0.0 1.0 0.0> 150.0 }

   texture {
      colour red 0.5 green 0.5 blue 1.0
      ambient 0.15
      diffuse 0.8
   }
   colour red 0.5 green 0.5 blue 1.0
}

// Light source
object {
   light_source { <100.0  120.0  -130.0>
      colour White
   }
}
