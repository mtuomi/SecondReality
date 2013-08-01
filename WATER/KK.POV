#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

camera {
   location <10 50 -50>
   look_at <0 0 0>
}


fog {
     colour red 0.0 green 0.0 blue 0.0
     250.0
    }


object { light_source {  <-10 60 30> color White } }

/*
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
   scale < 0.3 0.3  0.0001 >     
   translate < 0 0 0 >
        
   rotate < -10 0 0 >
   rotate < 0 -90 0 >
   rotate < 50 0 0 >

   translate < 0 0 0 >     
}
*/



object {
   plane { <0.0 1.0 0.0> 140.0 }
   texture {
      color red 0 green 0.0 blue 1
      ambient 0.2
      diffuse 0.8
      scale < 20.0 20.0 20000.0 >
      translate <0 0 -10000>
   }
}


object {
//   sphere { <65.0 40.0 -190.0> 60.0 }

   sphere { <33 10 -33> 20 }
   texture {
      color red 0.0 green 0.0 blue 0.5 
      ambient 0.0
      diffuse 0.3
      specular 0
      roughness 0.001
      reflection 1.0
      phong 1.0 phong_size 80
      ior 1.5
           }
       }


object {
   sphere { <22 10 22> 20 }
   texture {
      color red 0.0 green 0.0 blue 0.5 
      ambient 0.0
      diffuse 0.3
      specular 0
      roughness 0.001
      reflection 1.0
      phong 1.0 phong_size 80
      ior 1.5
           }
       }


/*-------------- SWAMP WATER ----------------------------*/
object { 
   intersection { Cube scale <10000 1 500> translate <0 0 0>  }
   texture {
      color MidnightBlue  
      reflection 0.6
      ambient 0.0
      ripples 0.4 frequency 0.04
      translate <0 0 0>
      diffuse 0.6 phong 1.0 phong_size 80
   }
  rotate < 0 0 0 >
}

