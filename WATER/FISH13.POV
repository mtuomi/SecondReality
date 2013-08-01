#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

camera {
   location <0 0 -155>
   look_at <15 0 0>
}


fog {
     colour red 0.0 green 0.0 blue 0.2
     200.0
    }


//object { light_source { <200 100 -100> color White } }
object { light_source {  <-200 200 -1500> color White } }


object {
   box { UnitBox scale <256 32 4>}
   texture { color Green }

   texture {
      image_map { <1 -1 0> tga "rgb.tga"    /* x-y oriented bitmap image */
         once
         interpolate 2                          /* Faster interpolation than 4 */
           }

      translate <-0.5 -0.5 0>                /* Center image */
      
      scale <512 64 2>                          /* Scale to fit */
      ambient 1
      diffuse 0.75
           }
   scale < 0.7 0.7  0.0001 >     
   rotate < -40 -80 0 >        
   rotate < 19 0 0 >

   translate < 0 0 -70 >     
}


object {
   sphere { <110.0 40.0 -90.0> 80.0 }
   texture {
      color Blue
      ambient 0.3
      diffuse 0.8
      specular 0.2
      roughness 0.001
      reflection 1.0
      ior 1.5
           }
       }


/*-------------- SWAMP WATER ----------------------------*/
object { 
   intersection { Cube scale <10000 1 500> translate <0 -25 0>  }
   texture {
      color red 0.0 green 0.0 blue 0.4
      reflection 0.8
      ambient 0.5
      ripples 0.5 frequency 0.02
      translate <0 0 0>
      diffuse 1 phong 1.0 phong_size 80
   }
}






