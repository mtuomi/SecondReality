// Persistence Of Vision Raytracer version 1.0 sample file.


//----------- fish out of water ----------- 3/10/92 miller

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

camera {
   location <-5 40 -105> direction <0 0 1.5>
   look_at <0 0 0>
}

//-------light
object { light_source { <200 100 -100> color White } }
//-------light
object { light_source {  <1000 500 400> color White } }


/* Now draw the sky */
object {
   sphere { <0.0  0.0  0.0> 800.0 }


   texture {
      gradient <0.0  1.0  0.0>
      colour_map { [0.0 0.8  colour red 1.0 green 0.3 blue 0.0
                           colour red 0.7 green 0.7 blue 1.0]
                 [0.8 1.0 colour red 0.7 green 0.7 blue 1.0
                          colour red 0.7 green 0.7 blue 1.0]
      }


      scale <400.0  400.0  400.0>
      ambient 0.7
      diffuse 0.0 
   }
   colour red 0.2  green 0.2 blue 1.0
}


/*-------------- SWAMP WATER ----------------------------*/
object { 
   intersection { Cube scale <1000 1 500> translate <0 -25 0>  }
   texture {
      color red 0.0 green 0.07 blue 0.0
      reflection 1
      ambient 0.25
      color Blue
      waves 0.6 frequency 0.8
      translate <80 0 0>
      diffuse 1 phong 1.0 phong_size 80
   }
}






